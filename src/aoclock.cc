// Copyright 2017-2018 SeetaTech

#include "include/aoclock.h"

AocLock::AocLock() { pthread_mutex_init(&mutex_, nullptr); }

AocLock::~AocLock() { pthread_mutex_destroy(&mutex_); }

void AocLock::lock() { pthread_mutex_lock(&mutex_); }

void AocLock::unlock() { pthread_mutex_unlock(&mutex_); }
