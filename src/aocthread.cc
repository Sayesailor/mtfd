// Copyright 2017-2018 SeetaTech

#include "include/aocthread.h"
#include <cstring>
#include "include/eupulogger4system.h"

AocThread::AocThread() : pid_(0), is_operate_(false), is_exit_(true) {
    LOG(_INFO_, "AocThread::AocThread()");
    set_mask_SIGUSR1();
    sigemptyset(&waitsig_);
    sigaddset(&waitsig_, SIGUSR1);
}

AocThread::~AocThread() {
    LOG(_INFO_, "AocThread::~AocThread() pid = %lu", pid_);
    is_operate_ = false;
    is_exit_ = false;
}

void *AocThread::aoc_func(void *arg) {
    AocThread *pthread = reinterpret_cast<AocThread *>(arg);
    LOG(_INFO_, "AocThread::aoc_func() pid = %lu", pthread->get_pid());
    pthread->run();
    return nullptr;
}

void AocThread::set_mask_SIGUSR1() {
    LOG(_INFO_, "AocThread::set_mask_SIGUSR1()");
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &sig, nullptr);
}

bool AocThread::start() {
    int nret = pthread_create(&pid_, nullptr, aoc_func, this);
    LOG(_INFO_, "AocThread::start() pthread_create() pid_ = %lu", pid_);

    if (0 == nret) {
        is_operate_ = true;
        is_exit_ = false;
        LOG(_INFO_, "AocThread::start() pid = %lu", pid_);
        return true;
    } else {
        LOG(_ERROR_, "AocThread::start() failed");
        return false;
    }
}

bool AocThread::stop() {
    LOG(_INFO_, "AocThread::stop() pid_ = %lu", pid_);
    is_operate_ = false;
    return true;
}

void AocThread::pause() {
    LOG(_INFO_, "AocThread::pause() pid = %lu", pid_);
    int sig;
    sigwait(&waitsig_, &sig);
}

void AocThread::continues() {
    LOG(_INFO_, "AocThread::continues() pid = %lu", pid_);
    pthread_kill(pid_, SIGUSR1);
}

bool AocThread::is_started() { return is_operate_; }
