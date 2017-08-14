#ifndef MTFD_INCLUDE_WORKBASETHREAD_H_
#define MTFD_INCLUDE_WORKBASETHREAD_H_

#include "aocthread.h"

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

#endif
