#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void handler(int signo)
{
    pid_t rid;
    while ((rid = waitpid(-1, nullptr, WNOHANG)) > 0)
    {
        cout << "I am proccess: " << getpid() << " catch a signo: " << signo << "child process quit: " << rid << endl;
    }
}

int main()
{
    signal(17, handler);

    for (int i = 0; i < 10; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            while (true)
            {
                cout << "I am child process: " << getpid() << ", ppid: " << getppid() << endl;
                sleep(5);
                break;
            }
            cout << "child quit!!!" << endl;
            exit(0);
        }
        // sleep(rand()%5+3);
        sleep(1);
    }

    // father
    while (true)
    {
        cout << "I am father process: " << getpid() << endl;
        sleep(1);
    }

    return 0;
}

// volatile int flag = 0;

// void handler(int signo)
// {
//     cout << "catch a signal: " << signo << endl;
//     flag = 1;
// }

// int main(int argc, char *argv[])
// {
//     signal(2, handler);
//     // 在优化条件下，flag变量可能被直接优化到CPU内的寄存器中
//     while (!flag)
//         ;

//     cout << "process quit normal" << endl;
//     return 0;
// }

// void MyPrintPending()
// {
//     sigset_t pending;
//     int n = sigpending(&pending);
//     for (int signo = 31; signo >= 1; signo--)
//     {
//         if (sigismember(&pending, signo))
//         {
//             cout << "1";
//         }
//         else
//         {
//             cout << "0";
//         }
//     }
//     cout << "\n\n";
// }

// void handler(int signo)
// {
//     cout << "get a sig, number: " << signo << endl;
//     while (true)
//     {
//         MyPrintPending();
//         sleep(1);
//     }
// }

// int main(int argc, char *argv[])
// {
//     struct sigaction act, oact;
//     memset(&act, 0, sizeof(act));
//     memset(&oact, 0, sizeof(oact));

//     sigemptyset(&act.sa_mask);
//     sigaddset(&act.sa_mask, 1);
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 4);
//     act.sa_handler = handler; // SIG_IGN SIG_DFL
//     sigaction(2, &act, &oact);

//     while (true)
//     {
//         cout << "I am a process: " << getpid() << endl;
//         sleep(1);
//     }

//     return 0;
// }

// void PrintPending(sigset_t &pending)
// {
//     for (int signo = 31; signo >= 1; signo--)
//     {
//         if (sigismember(&pending, signo))
//         {
//             cout << "1";
//         }
//         else
//         {
//             cout << "0";
//         }
//     }
//     cout << "\n\n";
// }
// int main3(int argc, char *argv[])
// {
//     // 1.对2号新号屏蔽
//     sigset_t bset, oset;
//     sigemptyset(&bset);
//     sigemptyset(&oset);
//     sigaddset(&bset, SIGINT);
//     sigprocmask(SIG_SETMASK, &bset, &oset);

//     // 2.重复打印当前进程的pending
//     int cnt = 0;
//     while (true)
//     {
//         sigset_t pending;
//         int n = sigpending(&pending);
//         if (n < 0)
//             continue;
//         PrintPending(pending);

//         sleep(1);
//         cnt++;
//         // 3.解除阻塞
//         if (cnt == 20)
//         {
//             cout << "unblock 2 signo" << endl;
//             sigprocmask(SIG_SETMASK, &oset, nullptr);
//         }
//     }
//     return 0;
// }

int main2(int argc, char *argv[])
{
    signal(SIGINT, SIG_IGN);
    return 0;
}

int main1(int argc, char *argv[])
{
    pid_t id = fork();
    if (id == 0)
    {
        // child
        int cnt = 500;
        while (cnt)
        {
            cout << "i am a child process, pid: " << getpid() << "cnt: " << cnt << endl;
            sleep(1);
            cnt--;
        }

        exit(0);
    }

    // father
    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if (rid == id)
    {
        cout << "child quit info, rid: " << rid << " exit code: " << ((status >> 8) & 0xFF) << " exit signal: " << (status & 0x7F) << " core dump: " << ((status >> 7) & 1) << endl; // ? & (0000 0000 ... 0001)
    }
    return 0;
}

/*
int main(int argc, char *argv[])
{
    int a = 10;
    int b = 0;

    a /= b;
    return 0;
}
*/
/*
void work()
{
    cout << "print log..." << endl;
}

// 信号为什么会一直被触发？？
void handler(int signo)
{
    // work();
    cout << "...get a sig, number: " << signo << endl; // 我什么都没干，我只是打印了消息
    // exit(1);
    int n = alarm(5);
    cout << "剩余时间：" << n << endl;
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, handler);
    int n = alarm(5);

    while (1)
    {
        cout << "proc is running ..." << endl;
        sleep(1);
    }
    return 0;
}
*/

/*
void handler(int signo)
{
    cout << "process get a signal: " << signo << endl;
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, handler);
    cout << "pointer error before" << endl;

    int *p = nullptr;
    *p = 100; // Segmentation fault

    cout << "pointer error after" << endl;
    return 0;
}
*/
/*
void Usage(string proc)
{
    cout << "Usage:\n\t" << proc << " signum pid\n\n";
}

void myhandler(int signo)
{
    cout << "process get a signal: " << signo << endl;
    // exit(1);
}

// mykill signum pid
int main(int argc, char *argv[])
{
    // signal(2, myhandler);

    signal(SIGABRT, myhandler);
    int cnt = 0;
    while (true)
    {
        cout << "I am a process, pid: " << getpid() << endl;
        sleep(1);
        cnt++;
        if (cnt % 2 == 0)
        {
            // kill(getpid(), 2);
            // raise(2);
            // abort();
            //  kill(getpid(), 6);
        }
    }

    // if (argc != 3)
    // {
    //     Usage(argv[0]);
    //     exit(1);
    // }
    // int signum = stoi(argv[1]);
    // pid_t pid = stoi(argv[2]);

    // int n = kill(pid, signum);
    // if (n == -1)
    // {
    //     perror("kill");
    //     exit(2);
    // }

    return 0;
}
*/

// void myhandler(int signo)
// {
//     cout << "process get a signal: " << signo << endl;
//     // exit(1);
// }

// int main()
// {
//     signal(SIGINT, myhandler); // 只需要设置一次，往后都有效！
//     signal(3, myhandler);
//     signal(19, myhandler);
//     // 信号的产生和我们自己的代码的运行时异步的

//     for (int i = 1; i <= 31; i++)
//     {
//         signal(i, myhandler);
//     }

//     while (true)
//     {
//         cout << "I am a crazy process : " << getpid() << endl;
//         sleep(1);
//     }
// }