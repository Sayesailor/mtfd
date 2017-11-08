// Copyright 2017-2018 SeetaTech

#include "include/workbasethread.h"
#include <eupulogger4system.h>
#include <unistd.h>
#include "include/fd.h"

WorkBaseThread::WorkBaseThread() : AocThread() {
    LOG(_INFO_, "WorkBaseThread::WorkBaseThread()");
}

WorkBaseThread::~WorkBaseThread() {
    LOG(_INFO_, "WorkBaseThread::~WorkBaseThread()");
}

void WorkBaseThread::reset() {}

void WorkBaseThread::run() {
    LOG(_INFO_, "WorkBaseThread::run()");
    pause();

    is_exit_ = false;
    int nret = 0;
    while (is_operate_) {
        nret = process_task();
        if (nret < 0) {
            LOG(_ERROR_, "WordBaseThread::run() process_task() return %d",
                nret);
            is_operate_ = false;
        }
    }
    is_exit_ = true;
}

bool WorkBaseThread::start() {
    LOG(_INFO_, "WorkBaseThread::start()");
    if (!AocThread::start()) return false;

    continues();
    return true;
}

bool WorkBaseThread::stop() {
    LOG(_INFO_, "WorkBaseThread::stop()");
    is_operate_ = false;
    return true;
}
