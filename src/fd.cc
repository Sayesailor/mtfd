// Copyright 2017-2018 SeetaTech
#include "include/fd.h"
#include <VIPLFaceDetector.h>
#include <unistd.h>
#include <string>
#include <memory>
#include "include/config.h"
#include "include/eupulogger4system.h"

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

        fd_detector_.reset(new VIPLFaceDetector(fd_model_filename.c_str()));
        if (!fd_detector_) {
            LOG(_ERROR_, "Fd::init() new VIPLFaceDetector(%s) failed",
                fd_model_filename.c_str());
            break;
        }

        fd_detector_->SetMinFaceSize(60);
        fd_detector_->SetScoreThresh(0.7f, 0.7f, 0.85f);
        fd_detector_->SetImagePyramidScaleFactor(1.414f);

        if (access(pt_model_filename.c_str(), R_OK) == -1) {
            LOG(_ERROR_, "Fd::init() access(%s) return -1",
                pt_model_filename.c_str());
            break;
        }

        pt_detector_.reset(new VIPLPointDetector(pt_model_filename.c_str()));
        if (!pt_detector_) {
            LOG(_ERROR_, "Fd::init() new VIPLPointDetector(%s) failed",
                pt_model_filename.c_str());
            break;
        }

        ret = true;
    } while (0);

    return ret;
}
