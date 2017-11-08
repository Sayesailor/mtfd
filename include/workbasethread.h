/*
 * Copyright 2017-2018 SeetaTech
 */

#ifndef INCLUDE_WORKBASETHREAD_H_
#define INCLUDE_WORKBASETHREAD_H_

#include "include/aocthread.h"

class WorkBaseThread : public AocThread {
   public:
    WorkBaseThread();
    virtual ~WorkBaseThread();

    virtual void reset();
    bool start();
    virtual bool stop();

   protected:
    void run();
    virtual int process_task() = 0;
};

#endif  // INCLUDE_WORKBASETHREAD_H_
