#include <iostream>
#include <ctime>
#include "ThreadPool.hpp"
#include "Task.hpp"

pthread_spinlock_t slock;

int main()
{
    // pthread_spin_init(&slock, 0);
    // pthread_spin_destroy(&slock);

    // 如果获取单例对象的时候，也是多线程获取的呢？
    std::cout << "process runn..." << std::endl;
    sleep(3);
    // ThreadPool<Task> *tp = new ThreadPool<Task>(5);
    ThreadPool<Task>::GetInstance()->Start();
    srand(time(nullptr) ^ getpid());

    while (true)
    {
        // 1. 构建任务
        int x = rand() % 10 + 1;
        usleep(10);
        int y = rand() % 5;
        char op = opers[rand() % opers.size()];

        Task t(x, y, op);
        ThreadPool<Task>::GetInstance()->Push(t);
        // 2. 交给线程池处理
        std::cout << "main thread make task: " << t.GetTask() << std::endl;

        sleep(1);
    }
}