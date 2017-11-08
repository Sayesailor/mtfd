/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_AOCLOCK_H_
#define INCLUDE_AOCLOCK_H_

#include <pthread.h>

class AocLock {
   public:
    AocLock();
    virtual ~AocLock();
    void lock();
    void unlock();

   private:
    pthread_mutex_t mutex_;
};

#endif  // INCLUDE_AOCLOCK_H_
