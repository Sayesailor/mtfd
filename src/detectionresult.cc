// Copyright 2017-2018 SeetaTech

#include "include/detectionresult.h"
#include <fstream>
#include <iostream>

void DetectionResult::set_fi(const VIPLFaceInfo &fi) { fi_ = fi; }

VIPLFaceInfo DetectionResult::get_fi() { return fi_; }

void DetectionResult::set_flmks(const std::vector<VIPLPoint> &flmks) {
    flmks_ = flmks;
}

std::vector<VIPLPoint> DetectionResult::get_flmks() { return flmks_; }

void DetectionResult::set_filename(const std::string &filename) {
    filename_ = filename;
}

std::string DetectionResult::get_filename() { return filename_; }

void DetectionResult::output(std::ofstream &outfile) {
    outfile << filename_.c_str() << " " << fi_.x << " " << fi_.y << " "
            << fi_.width << " " << fi_.height << " ";

    for (auto it_flmks : flmks_)
        outfile << "(" << it_flmks.x << "," << it_flmks.y << ")";

    outfile << std::endl;
}
