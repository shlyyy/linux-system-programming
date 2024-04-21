#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>

template <class T>
class BlockQueue
{
    static const int defaultnum = 20;

public:
    BlockQueue(int maxcap = defaultnum) : maxcap_(maxcap)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&c_cond_, nullptr);
        pthread_cond_init(&p_cond_, nullptr);
    }

    T pop()
    {
        pthread_mutex_lock(&mutex_);

        // 1.确保消费条件满足
        while (q_.size() == 0)
        {
            // 调用的时候，自动释放锁，因为signal唤醒而返回的时候，重新持有锁
            pthread_cond_wait(&c_cond_, &mutex_);
        }

        // 消费：1.队列有资源就消费 2.被唤醒而消费
        T out = q_.front();
        q_.pop();

        // if(q_.size()<low_water_) pthread_cond_signal(&p_cond_);
        pthread_cond_signal(&p_cond_); // pthread_cond_broadcast
        pthread_mutex_unlock(&mutex_);

        return out;
    }

    void push(const T &in)
    {
        pthread_mutex_lock(&mutex_);

        // 1.确保生产条件满足
        while (q_.size() == maxcap_)
        {
            // 调用的时候，自动释放锁，因为signal唤醒而返回的时候，重新持有锁
            pthread_cond_wait(&p_cond_, &mutex_);
        }

        // 生产：1. 队列没满就生产 2.被唤醒而生产
        q_.push(in);
        // if(q_.size() > high_water_) pthread_cond_signal(&c_cond_);
        pthread_cond_signal(&c_cond_);
        pthread_mutex_unlock(&mutex_);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&c_cond_);
        pthread_cond_destroy(&p_cond_);
    }

private:
    // 共享资源, q被当做整体使用的，q只有一份，加锁。但是共享资源也可以被看做多份！
    std::queue<T> q_;

    // 队列中最大任务数
    int maxcap_;

    // 访问共享资源的锁
    pthread_mutex_t mutex_;

    // 消费者可以消费条件变量
    pthread_cond_t c_cond_;

    // 生产者可以生产条件变量
    pthread_cond_t p_cond_;

    // int low_water_;
    // int high_water_;
};