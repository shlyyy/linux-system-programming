#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

std::string toHex(pthread_t tid)
{
    char hex[64];
    snprintf(hex, sizeof(hex), "%p", (void *)tid);
    return hex;
}

void *threadRoutine(void *args)
{
    while (true)
    {
        cout << "thread id: " << toHex(pthread_self()) << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread 1");

    cout << "main thread create thead done, new thread id : " << toHex(tid) << endl;
    pthread_join(tid, nullptr);

    return 0;
}

// class Request
// {
// public:
//     Request(int start, int end, const string &threadname)
//         : start_(start), end_(end), threadname_(threadname)
//     {
//     }

// public:
//     int start_;
//     int end_;
//     string threadname_;
// };

// class Response
// {
// public:
//     Response(int result, int exitcode) : result_(result), exitcode_(exitcode)
//     {
//     }

// public:
//     int result_;   // 计算结果
//     int exitcode_; // 计算结果是否可靠
// };

// void *sumCount(void *args) // 线程的参数和返回值，不仅仅可以用来进行传递一般参数，也可以传递对象！！
// {
//     Request *rq = static_cast<Request *>(args); //  Request *rq = (Request*)args
//     Response *rsp = new Response(0, 0);
//     for (int i = rq->start_; i <= rq->end_; i++)
//     {
//         cout << rq->threadname_ << " is runing, caling..., " << i << endl;
//         rsp->result_ += i;
//         usleep(100000);
//     }
//     delete rq;
//     return rsp;
// }

// int main()
// {
//     pthread_t tid;
//     Request *rq = new Request(1, 100, "thread 1");
//     pthread_create(&tid, nullptr, sumCount, rq);

//     void *ret;
//     pthread_join(tid, &ret);
//     Response *rsp = static_cast<Response *>(ret);
//     cout << "rsp->result: " << rsp->result_ << ", exitcode: " << rsp->exitcode_ << endl;
//     delete rsp;
//     return 0;
// }

// void *threadRoutine(void *args)
// {
//     const char *name = (const char *)args;
//     int cnt = 5;
//     while (true)
//     {
//         printf("%s,  pid: %d\n", name, getpid());
//         sleep(1);
//         cnt--;
//         if (cnt == 0)
//             break;
//     }
//     // pthread_exit((void*)100);
//     // exit(11); // 直接调用exit, exit是用来终止进程的！！，不能用来直接终止进程！
//     return (void *)100; // 走到这里默认线程退出了！
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadRoutine, (void *)"Thread 1");

//     void *retval;
//     pthread_join(tid, &retval);

//     cout << "main thread quit ..., ret: " << (long long int)retval << endl;
// }