#ifndef MTFD_INCLUDE_DETECTIONRESULT_H_
#define MTFD_INCLUDE_DETECTIONRESULT_H_

#include <string>
#include <fstream>
#include <face_detection.h>

class DetectionResult {
   public:
    void set_fi(const seeta::FaceInfo& fi);
    seeta::FaceInfo get_fi();
    void set_flmks(const std::vector<seeta::FacialLandmark>& flmks);
    std::vector<seeta::FacialLandmark> get_flmks();
    void set_filename(const std::string& filename);
    std::string get_filename();

    void output(std::ofstream& outfile);

   public:
    static const int FLMKS_CNT = 5;

   private:
    seeta::FaceInfo fi_;
    std::vector<seeta::FacialLandmark> flmks_;
    std::string filename_;
};

#endif
