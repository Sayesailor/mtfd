#include "detectionresult.h"
#include <iostream>
#include <fstream>

void DetectionResult::set_fi(const seeta::FaceInfo &fi) { fi_ = fi; }

seeta::FaceInfo DetectionResult::get_fi() { return fi_; }

void DetectionResult::set_flmks(
    const std::vector<seeta::FacialLandmark> &flmks) {
    flmks_ = flmks;
}

std::vector<seeta::FacialLandmark> DetectionResult::get_flmks() {
    return flmks_;
}

void DetectionResult::set_filename(const std::string &filename) {
    filename_ = filename;
}

std::string DetectionResult::get_filename() { return filename_; }

void DetectionResult::output(std::ofstream &outfile) {
    outfile << filename_.c_str() << " " << fi_.bbox.x << " " << fi_.bbox.y
            << " " << fi_.bbox.width << " " << fi_.bbox.height << " ";

    for (auto it_flmks : flmks_)
        outfile << "(" << it_flmks.x << "," << it_flmks.y << ")";

    outfile << std::endl;
}
