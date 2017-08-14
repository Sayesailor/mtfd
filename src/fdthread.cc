#include <unistd.h>
#include <sys/time.h>
#include <eupulogger4system.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <uuid/uuid.h>
#include <iostream>
#include <fstream>
#include "workbasethread.h"
#include "fdthread.h"
#include "fd.h"
#include "face_crop.h"
#include "fdstatistics.h"

FdThread::FdThread(int id) : WorkBaseThread(), id_(id), pcfg_(NULL) {
    LOG(_INFO_, "FdThread::FdThread(%d)", id_);
    if (!fd_.init("/seeta_fd_frontal_v1.0.bin", "/seeta_fa_v1.1.bin"))
        LOG(_ERROR_, "FdThread::FdThread() fd_.init() failed");
    pcfg_ = Config::get_instance();
}

FdThread::~FdThread() { LOG(_INFO_, "FdThread::~FdThread() id = %d", id_); }

int FdThread::process_task() {
    LOG(_INFO_, "FdThread::process_task() id = %d", id_);
    int nret = do_face_detect();
    is_operate_ = false;
    return nret;
}

int FdThread::do_face_detect() {
    drs_.clear();
    seeta::FaceDetection *pfd = fd_.get_fd_detector();
    if (!pfd) {
        LOG(_ERROR_,
            "FdThread::do_face_detect() fd_.get_fd_detector() failed, id = %d",
            id_);
        return 0;
    }

    LOG(_INFO_, "FdThread::do_face_detect() imgfilenames_ cnt %d, id = %d",
        imgfilenames_.size(), id_);

    for (auto it_fn : imgfilenames_) {
        do {
            if (access(it_fn.c_str(), R_OK) == -1) break;

            cv::Mat mat;
            cv::Mat mat_gray;
            try {
                mat = cv::imread(it_fn.c_str(), cv::IMREAD_UNCHANGED);
                if (!mat.data) {
                    LOG(_ERROR_,
                        "FdThread::do_face_detect() imgfilename_ %s is invalid",
                        it_fn.c_str());
                    continue;
                }

                cv::cvtColor(mat, mat_gray, cv::COLOR_BGR2GRAY);
            } catch (...) {
                LOG(_ERROR_,
                    "FdThread::do_face_detect() cv::imread() EXCEPTION, image "
                    "%s",
                    it_fn.c_str());
                continue;
            }

            seeta::ImageData seeta_img_gray(mat_gray.cols, mat_gray.rows,
                                            mat_gray.channels());
            seeta_img_gray.data = mat_gray.data;

            std::vector<seeta::FaceInfo> faces = pfd->Detect(seeta_img_gray);

            if (faces.size() > 1)
                FdStatistics::get_instance()->inc_multi_face_cnt();
            if (faces.size() >= 1)
                FdStatistics::get_instance()->inc_dest_cnt();
            else
                FdStatistics::get_instance()->inc_err_cnt();

            if (faces.size() > 1 || faces.size() == 0)
                LOG(_INFO_,
                    "FdThread::do_face_detect() image %s, faces cnt %d, id "
                    "= "
                    "%d",
                    it_fn.c_str(), faces.size(), id_);

            if (faces.size() > 1 && pcfg_ && pcfg_->get_multi_face() == 0)
                continue;

            std::string fn = it_fn;
            deal_faces_for_each_img(mat, mat_gray, faces, fn);

        } while (0);

        if (!is_started()) break;
    }

    output();

    return 0;
}

void FdThread::deal_faces_for_each_img(
    const cv::Mat &mat, const cv::Mat &mat_gray,
    const std::vector<seeta::FaceInfo> &faces, const std::string &fn) {
    VIPLImageData vipl_img(mat.cols, mat.rows, mat.channels());
    vipl_img.data = mat.data;
    seeta::ImageData seeta_img_gray(mat_gray.cols, mat_gray.rows,
                                    mat_gray.channels());
    seeta_img_gray.data = mat_gray.data;

    seeta::FaceAlignment *pfa = fd_.get_pt_detector();
    if (!pfa) {
        LOG(_ERROR_,
            "FdThread::do_face_detect() fd_.get_pt_detector() failed, id = %d",
            id_);
        return;
    }

    cropid_ = 0;
    for (auto it_face : faces) {
        std::vector<seeta::FacialLandmark> flmks(DetectionResult::FLMKS_CNT);
        pfa->PointDetectLandmarks(seeta_img_gray, it_face, &flmks[0]);

        if (flmks.size() == 0) continue;

        if (pcfg_ && pcfg_->get_face_crop() == 1) {
            std::vector<VIPLPoint> vipl_points;
            for (auto it : flmks) {
                VIPLPoint pt;
                pt.x = it.x;
                pt.y = it.y;
                vipl_points.push_back(pt);
            }
            do_face_crop(vipl_img, vipl_points, fn);
        }

        DetectionResult dr;
        dr.set_fi(it_face);
        dr.set_flmks(flmks);
        dr.set_filename(fn);
        drs_.push_back(dr);
    }
}

void FdThread::do_face_crop(const VIPLImageData &vipl_img,
                            const std::vector<VIPLPoint> &vipl_points,
                            const std::string &fn) {
    std::vector<VIPLPoint> mean_shape(DetectionResult::FLMKS_CNT);
    int mean_shape_size;

    VIPL::FaceMeanShape(&mean_shape[0], DetectionResult::FLMKS_CNT,
                        &mean_shape_size, 0);

    int crop_size = pcfg_->get_mean_shape();
    VIPL::ResizeMeanShape(&mean_shape[0], DetectionResult::FLMKS_CNT,
                          static_cast<double>(crop_size) / mean_shape_size);
    mean_shape_size = crop_size;

    int final_size = pcfg_->get_final_sz();
    cv::Mat mat_crop(final_size, final_size, CV_8UC(vipl_img.channels));
    VIPLImageData image_crop(mat_crop.cols, mat_crop.rows, mat_crop.channels());
    image_crop.data = mat_crop.data;

    if (!VIPL::FaceCrop(vipl_img, image_crop, &vipl_points[0],
                        DetectionResult::FLMKS_CNT, &mean_shape[0],
                        mean_shape_size, VIPL::BY_LINEAR, nullptr, final_size))
        LOG(_ERROR_, "VIPL::FaceCrop() failed");

    std::string src_path = pcfg_->get_src_path();
    std::string crop_path = pcfg_->get_crop_path();
    std::string crop_fn = fn;
    crop_fn.replace(crop_fn.find(src_path), src_path.length(), crop_path);

    char buf[PATH_MAX] = {0};
    if (pcfg_->get_multi_face() == 1)
        sprintf(buf, "%s_%02d.%s",
                crop_fn.substr(0, crop_fn.rfind(".")).c_str(), cropid_++,
                pcfg_->get_img_suffix().c_str());
    else
        sprintf(buf, "%s", crop_fn.c_str());
    cv::imwrite(buf, mat_crop);
}

void FdThread::output() {
    if (drs_.size() == 0) return;

    std::string dest_path = pcfg_->get_dest_path();
    std::ofstream outfile;
    if (!dest_path.empty()) {
        char tmpbuf[256];
        uuid_t uu;
        uuid_generate(uu);
        uuid_unparse(uu, tmpbuf);
        std::string pathname = dest_path + "/";
        pathname += tmpbuf;
        outfile.open(pathname.c_str());
        if (!outfile.is_open()) {
            LOG(_ERROR_, "FdThread::output(%s)", pathname.c_str());
            return;
        }
    } else {
        return;
    }
    for (auto it : drs_) it.output(outfile);
    outfile.close();
}
