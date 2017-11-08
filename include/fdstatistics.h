/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_FDSTATISTICS_H_
#define INCLUDE_FDSTATISTICS_H_

#include "include/aocsingleton.h"

class FdStatistics : public IAocSingleton<FdStatistics> {
   public:
    void output();
    void clear();
    void inc_src_cnt(int n = 1) { src_cnt_ += n; }
    void inc_dest_cnt(int n = 1) { dest_cnt_ += n; }
    void inc_multi_face_cnt(int n = 1) { multi_face_cnt_ += n; }
    void inc_err_cnt(int n = 1) { err_cnt_ += n; }

    int get_src_cnt() { return src_cnt_; }
    int get_dest_cnt() { return dest_cnt_; }

   private:
    FdStatistics();
    virtual ~FdStatistics();
    FdStatistics(const FdStatistics &);
    FdStatistics &operator=(const FdStatistics &);
    friend IAocSingleton<FdStatistics>;

   private:
    int src_cnt_;
    int dest_cnt_;
    int multi_face_cnt_;
    int err_cnt_;
};

#endif  // INCLUDE_FDSTATISTICS_H_
