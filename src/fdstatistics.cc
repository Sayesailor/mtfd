// Copyright 2017-2018 SeetaTech

#include "include/fdstatistics.h"
#include <cstdio>
#include <cstdlib>
#include "include/config.h"

FdStatistics::FdStatistics()
    : src_cnt_(0), dest_cnt_(0), multi_face_cnt_(0), err_cnt_(0) {}

FdStatistics::~FdStatistics() {}

void FdStatistics::output() {
    printf("<pre>\n");
    Config *pcfg = Config::get_instance();
    pcfg->output();
    printf("-------------------\n");
    printf("total image count %d\n", src_cnt_);
    printf("total detected count %d\n", dest_cnt_);
    printf("total multi faces count %d\n", multi_face_cnt_);
    printf("total error count %d\n", err_cnt_);
    printf("</pre>\n");
}

void FdStatistics::clear() {
    src_cnt_ = 0;
    dest_cnt_ = 0;
    multi_face_cnt_ = 0;
    err_cnt_ = 0;
}
