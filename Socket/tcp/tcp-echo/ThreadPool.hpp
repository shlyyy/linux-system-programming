#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <pthread.h>
#include <unistd.h>

struct ThreadInfo
{
    pthread_t tid;
    std::string name;
};

static const int defalutnum = 10;

template <class T>
class ThreadPool
{
public:
    void Lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    void Wakeup()
    {
        pthread_cond_signal(&cond_);
    }
    void ThreadSleep()
    {
        pthread_cond_wait(&cond_, &mutex_);
    }
    bool IsQueueEmpty()
    {
        return tasks_.empty();
    }
    std::string GetThreadName(pthread_t tid)
    {
        for (const auto &ti : threads_)
        {
            if (ti.tid == tid)
                return ti.name;
        }
        return "None";
    }

public:
    static void *HandlerTask(void *args)
    {
        ThreadPool<T> *tp = static_cast<ThreadPool<T> *>(args);
        std::string name = tp->GetThreadName(pthread_self());
        while (true)
        {
            tp->Lock();

            while (tp->IsQueueEmpty())
            {
                tp->ThreadSleep();
            }
            T t = tp->Pop();
            tp->Unlock();

            t();
        }
    }
    void Start()
    {
        int num = threads_.size();
        for (int i = 0; i < num; i++)
        {
            threads_[i].name = "thread-" + std::to_string(i + 1);
            pthread_create(&(threads_[i].tid), nullptr, HandlerTask, this);
        }
    }
    T Pop()
    {
        T t = tasks_.front();
        tasks_.pop();
        return t;
    }
    void Push(const T &t)
    {
        Lock();
        tasks_.push(t);
        Wakeup();
        Unlock();
    }
    static ThreadPool<T> *GetInstance()
    {
        if (nullptr == tp_) // ???
        {
            pthread_mutex_lock(&lock_);
            if (nullptr == tp_)
            {
                std::cout << "log: singleton create done first!" << std::endl;
                tp_ = new ThreadPool<T>();
            }
            pthread_mutex_unlock(&lock_);
        }

        return tp_;
    }

private:
    ThreadPool(int num = defalutnum) : threads_(num)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&cond_, nullptr);
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
    ThreadPool(const ThreadPool<T> &) = delete;
    const ThreadPool<T> &operator=(const ThreadPool<T> &) = delete; // a=b=c
private:
    std::vector<ThreadInfo> threads_;
    std::queue<T> tasks_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;

    static ThreadPool<T> *tp_;
    static pthread_mutex_t lock_;
};

template <class T>
ThreadPool<T> *ThreadPool<T>::tp_ = nullptr;

template <class T>
pthread_mutex_t ThreadPool<T>::lock_ = PTHREAD_MUTEX_INITIALIZER;