/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_ALIGNMENT_H_
#define INCLUDE_ALIGNMENT_H_
#include <cstdint>

enum SAMPLING_TYPE { LINEAR, BICUBIC };

bool face_crop_core(const uint8_t *image_data, int image_width,
                    int image_height, int image_channels, uint8_t *crop_data,
                    int crop_width, int crop_height, const float *points,
                    int points_num, const float *mean_shape,
                    int mean_shape_width, int mean_shape_height,
                    int pad_top = 0, int pad_bottom = 0, int pad_left = 0,
                    int pad_right = 0, float *final_points = nullptr,
                    SAMPLING_TYPE type = LINEAR);

#endif  // INCLUDE_ALIGNMENT_H_
