#ifndef MTFD_INCLUDE_AOCLOCK_H_
#define MTFD_INCLUDE_AOCLOCK_H_

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

#endif
