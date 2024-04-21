#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>
#include "RingQueue.hpp"
#include "Task.hpp"

using namespace std;

struct ThreadData
{
    RingQueue<Task> *rq;
    std::string threadname;
};

void *Productor(void *args)
{
    // sleep(3);
    ThreadData *td = static_cast<ThreadData *>(args);
    RingQueue<Task> *rq = td->rq;
    std::string name = td->threadname;
    int len = opers.size();
    while (true)
    {
        // 1. 获取数据
        int data1 = rand() % 10 + 1;
        usleep(10);
        int data2 = rand() % 10;
        char op = opers[rand() % len];
        Task t(data1, data2, op);

        // 2. 生产数据
        rq->Push(t);
        cout << "Productor task done, task is : " << t.GetTask() << " who: " << name << endl;

        sleep(1);
    }
    return nullptr;
}

void *Consumer(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);
    RingQueue<Task> *rq = td->rq;
    std::string name = td->threadname;

    while (true)
    {
        // 1. 消费数据
        Task t;
        rq->Pop(&t);

        // 2. 处理数据
        t();
        cout << "Consumer get task, task is : " << t.GetTask() << " who: " << name << " result: " << t.GetResult() << endl;
        // sleep(1);
    }
    return nullptr;
}

int main()
{
    srand(time(nullptr) ^ getpid());
    RingQueue<Task> *rq = new RingQueue<Task>(50);

    pthread_t c[5], p[3];

    for (int i = 0; i < 1; i++)
    {
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Productor-" + std::to_string(i);

        pthread_create(p + i, nullptr, Productor, td);
    }
    for (int i = 0; i < 1; i++)
    {
        ThreadData *td = new ThreadData();
        td->rq = rq;
        td->threadname = "Consumer-" + std::to_string(i);

        pthread_create(c + i, nullptr, Consumer, td);
    }

    for (int i = 0; i < 1; i++)
    {
        pthread_join(p[i], nullptr);
    }
    for (int i = 0; i < 1; i++)
    {
        pthread_join(c[i], nullptr);
    }

    return 0;
}