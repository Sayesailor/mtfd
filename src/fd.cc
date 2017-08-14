#include <unistd.h>
#include <string>
#include <eupulogger4system.h>
#include <face_detection.h>
#include "fd.h"
#include "config.h"

bool Fd::init(const std::string &fd_mod_file, const std::string &pt_mod_file) {
    bool ret = false;
    do {
        std::string model_path = Config::get_instance()->get_model_path();
        std::string fd_model_filename = model_path + fd_mod_file;
        std::string pt_model_filename = model_path + pt_mod_file;
        if (access(fd_model_filename.c_str(), R_OK) == -1) {
            LOG(_ERROR_, "Fd::init() access(%s) return -1",
                fd_model_filename.c_str());
            break;
        }

        if (fd_detector_) delete fd_detector_;
        fd_detector_ = NULL;
        fd_detector_ = new seeta::FaceDetection(fd_model_filename.c_str());
        if (!fd_detector_) {
            LOG(_ERROR_, "Fd::init() new seeta::FaceDetection(%s) failed",
                fd_model_filename.c_str());
            break;
        }

        fd_detector_->SetMinFaceSize(40);
        fd_detector_->SetScoreThresh(2.0f);
        fd_detector_->SetImagePyramidScaleFactor(0.8f);
        fd_detector_->SetWindowStep(4, 4);

        if (access(pt_model_filename.c_str(), R_OK) == -1) {
            LOG(_ERROR_, "Fd::init() access(%s) return -1",
                pt_model_filename.c_str());
            break;
        }

        if (pt_detector_) delete pt_detector_;
        pt_detector_ = NULL;
        pt_detector_ = new seeta::FaceAlignment(pt_model_filename.c_str());
        if (!pt_detector_) {
            LOG(_ERROR_, "Fd::init() new seeta::FaceDetection(%s) failed",
                pt_model_filename.c_str());
            break;
        }

        ret = true;
    } while (0);

    if (ret == false) {
        if (fd_detector_) delete fd_detector_;
        if (pt_detector_) delete pt_detector_;
        fd_detector_ = NULL;
        pt_detector_ = NULL;
    }

    return ret;
}
