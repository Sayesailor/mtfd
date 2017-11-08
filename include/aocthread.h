/* Copyright 2017-2018 SeetaTech
 *
 * why aoc prefix? cause there are many thread frameworks,
 * so prefix aoc be added, distinguish from the standard ones,
 * it comes from LOGO of a led-display at frond of me.
 */

#ifndef INCLUDE_AOCTHREAD_H_
#define INCLUDE_AOCTHREAD_H_

#include <pthread.h>
#include <signal.h>

class AocThread {
   public:
    AocThread();
    virtual ~AocThread();

    pthread_t get_pid() { return pid_; }

    virtual bool start();
    virtual void pause();
    virtual void continues();
    virtual bool stop();
    virtual void reset() = 0;
    virtual bool is_started();

   protected:
    void set_mask_SIGUSR1();
    static void *aoc_func(void *arg);
    virtual void run() = 0;

   protected:
    pthread_t pid_;
    sigset_t waitsig_;
    bool is_operate_;
    bool is_exit_;
};

#endif  // INCLUDE_AOCTHREAD_H_
