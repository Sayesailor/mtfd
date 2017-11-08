/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_FACE_CROP_H_
#define INCLUDE_FACE_CROP_H_

#include <VIPLStruct.h>

namespace VIPL {
enum CROP_METHOD { BY_LINEAR, BY_BICUBIC };

VIPL_API bool FaceMeanShape(VIPLPoint *mean_shape, int num,
                            int *mean_shape_size, int id = 0);

VIPL_API void ResizeMeanShape(VIPLPoint *mean_shape, int num, double scaler);

VIPL_API bool FaceCrop(const VIPLImageData &src_img, VIPLImageData &dst_img,
                       const VIPLPoint *points, int num,
                       const VIPLPoint *mean_shape, int mean_shape_size,
                       CROP_METHOD method = BY_LINEAR,
                       VIPLPoint *final_points = nullptr, int final_size = -1);
}  // namespace VIPL

#endif  // INCLUDE_FACE_CROP_H_
