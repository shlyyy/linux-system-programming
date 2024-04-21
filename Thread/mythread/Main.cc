#include <iostream>
#include <unistd.h>
#include <vector>
#include "Thread.hpp"

using namespace std;

void Print()
{
    while (true)
    {
        printf("haha, 我是一个封装的线程...\n");
        sleep(1);
    }
}

int main()
{
    std::vector<Thread> threads;

    for (int i = 0; i < 10; i++)
    {
        threads.push_back(Thread(Print));
    }

    for (auto &t : threads)
    {
        t.Run();
    }

    for (auto &t : threads)
    {
        t.Join();
    }
    // Thread t(Print);
    // t.Run();

    // cout << "是否启动成功: " << t.IsRunning() << endl;
    // cout << "启动成功时间戳: " << t.StartTimestamp() << endl;
    // cout << "线程的名字: " << t.Name() << endl;

    // t.Join();

    return 0;
}