/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_FD_H_
#define INCLUDE_FD_H_

#include <VIPLFaceDetector.h>
#include <VIPLPointDetector.h>
#include <VIPLStruct.h>
#include <memory>
#include <string>

class Fd {
   public:
    Fd() {}
    virtual ~Fd() {}

    bool init(const std::string &fd_model_filename,
              const std::string &pt_model_filename);
    VIPLFaceDetector *get_fd_detector() { return fd_detector_.get(); }
    VIPLPointDetector *get_pt_detector() { return pt_detector_.get(); }

   private:
    std::unique_ptr<VIPLFaceDetector> fd_detector_;
    std::unique_ptr<VIPLPointDetector> pt_detector_;
};

#endif  // INCLUDE_FD_H_
