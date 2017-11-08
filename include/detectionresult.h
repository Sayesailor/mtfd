/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_DETECTIONRESULT_H_
#define INCLUDE_DETECTIONRESULT_H_

#include <VIPLFaceDetector.h>
#include <fstream>
#include <string>
#include <vector>

class DetectionResult {
   public:
    void set_fi(const VIPLFaceInfo& fi);
    VIPLFaceInfo get_fi();
    void set_flmks(const std::vector<VIPLPoint>& flmks);
    std::vector<VIPLPoint> get_flmks();
    void set_filename(const std::string& filename);
    std::string get_filename();

    void output(std::ofstream& outfile);

   public:
    static const int FLMKS_CNT = 5;

   private:
    VIPLFaceInfo fi_;
    std::vector<VIPLPoint> flmks_;
    std::string filename_;
};

#endif  // INCLUDE_DETECTIONRESULT_H_
