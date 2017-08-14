#ifndef MTFD_SRC_FDTHREAD_H_
#define MTFD_SRC_FDTHREAD_H_

#include <opencv2/imgproc/imgproc.hpp>
#include "workbasethread.h"
#include "detectionresult.h"
#include "fd.h"
#include "face_crop.h"
#include "config.h"

class FdThread : public WorkBaseThread {
   public:
    explicit FdThread(int id);
    virtual ~FdThread();

    int process_task();

    void set_imgfilenames(std::vector<std::string> ifns) {
        imgfilenames_ = ifns;
    }
    void output();

   private:
    int do_face_detect();
    void deal_faces_for_each_img(const cv::Mat &mat, const cv::Mat &mat_gray,
                                 const std::vector<seeta::FaceInfo> &faces,
                                 const std::string &fn);
    void do_face_crop(const VIPLImageData &img,
                      const std::vector<VIPLPoint> &pts, const std::string &fn);

   private:
    std::vector<std::string> imgfilenames_;
    std::vector<DetectionResult> drs_;
    Fd fd_;
    int id_;
    int cropid_;
    Config *pcfg_;
};

#endif
