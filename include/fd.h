#ifndef MTFD_INCLUDE_FD_H_
#define MTFD_INCLUDE_FD_H_

#include <face_detection.h>
#include <face_alignment.h>

class Fd {
   public:
    Fd() : fd_detector_(NULL), pt_detector_(NULL) {}
    virtual ~Fd() {
        if (fd_detector_) delete fd_detector_;
        if (pt_detector_) delete pt_detector_;
    }

    bool init(const std::string &fd_model_filename,
              const std::string &pt_model_filename);
    seeta::FaceDetection *get_fd_detector() { return fd_detector_; }
    seeta::FaceAlignment *get_pt_detector() { return pt_detector_; }

   private:
    seeta::FaceDetection *fd_detector_;
    seeta::FaceAlignment *pt_detector_;
};

#endif
