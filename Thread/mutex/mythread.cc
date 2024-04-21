#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include "LockGuard.hpp"
using namespace std;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define NUM 4

class threadData
{
public:
    threadData(int number)
    {
        threadname = "thread-" + to_string(number);
    }

public:
    string threadname;
};

int tickets = 1000; // 用多线程，模拟一轮抢票

void *getTicket(void *args)
{
    threadData *td = static_cast<threadData *>(args);
    const char *name = td->threadname.c_str();
    while (true)
    {
        {
            LockGuard lockguard(&lock); // 临时的LockGuard对象， RAII风格的锁！
            if (tickets > 0)
            {
                usleep(1000);
                printf("who=%s, get a ticket: %d\n", name, tickets); // ?
                tickets--;
            }
            else
                break;
        }
        usleep(13); // 防止其他线程饥饿
    }
    printf("%s ... quit\n", name);
    return nullptr;
}

int main()
{
    vector<pthread_t> tids;
    vector<threadData *> thread_datas;
    for (int i = 1; i <= NUM; i++)
    {
        pthread_t tid;
        threadData *td = new threadData(i);
        thread_datas.push_back(td);
        pthread_create(&tid, nullptr, getTicket, thread_datas[i - 1]);
        tids.push_back(tid);
    }

    for (auto thread : tids)
    {
        pthread_join(thread, nullptr);
    }

    for (auto td : thread_datas)
    {
        delete td;
    }
    return 0;
}