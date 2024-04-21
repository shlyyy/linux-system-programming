#pragma once
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <pthread.h>

const static int defaultcap = 5;

template <class T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        sem_wait(&sem);
    }
    void V(sem_t &sem)
    {
        sem_post(&sem);
    }
    void Lock(pthread_mutex_t &mutex)
    {
        pthread_mutex_lock(&mutex);
    }
    void Unlock(pthread_mutex_t &mutex)
    {
        pthread_mutex_unlock(&mutex);
    }

public:
    RingQueue(int cap = defaultcap)
        : ringqueue_(cap), cap_(cap), c_step_(0), p_step_(0)
    {

        // 初始化 最后一个参数指定初始值
        sem_init(&cdata_sem_, 0, 0);
        sem_init(&pspace_sem_, 0, cap);

        pthread_mutex_init(&c_mutex_, nullptr);
        pthread_mutex_init(&p_mutex_, nullptr);
    }
    void Push(const T &in) // 生产
    {
        P(pspace_sem_);

        Lock(p_mutex_); // ?
        ringqueue_[p_step_] = in;
        // 位置后移，维持环形特性
        p_step_++;
        p_step_ %= cap_;
        Unlock(p_mutex_);

        V(cdata_sem_);
    }
    void Pop(T *out) // 消费
    {
        P(cdata_sem_);

        Lock(c_mutex_); // ?
        *out = ringqueue_[c_step_];
        // 位置后移，维持环形特性
        c_step_++;
        c_step_ %= cap_;
        Unlock(c_mutex_);

        V(pspace_sem_);
    }
    ~RingQueue()
    {
        sem_destroy(&cdata_sem_);
        sem_destroy(&pspace_sem_);

        pthread_mutex_destroy(&c_mutex_);
        pthread_mutex_destroy(&p_mutex_);
    }

private:
    std::vector<T> ringqueue_;
    int cap_;

    int c_step_; // 消费者下标
    int p_step_; // 生产者下标

    sem_t cdata_sem_;  // 消费者关注的数据资源
    sem_t pspace_sem_; // 生产者关注的空间资源

    pthread_mutex_t c_mutex_; // 多个消费者互斥访问队列
    pthread_mutex_t p_mutex_; // 多个生产者互斥访问队列
};