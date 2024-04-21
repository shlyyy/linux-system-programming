#pragma once

#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *lock) : lock_(lock)
    {
    }
    void Lock()
    {
        pthread_mutex_lock(lock_);
    }
    void Unlock()
    {
        pthread_mutex_unlock(lock_);
    }
    ~Mutex()
    {
    }

private:
    pthread_mutex_t *lock_;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *lock) : mutex_(lock)
    {
        mutex_.Lock();
    }
    ~LockGuard()
    {
        mutex_.Unlock();
    }

private:
    Mutex mutex_;
};