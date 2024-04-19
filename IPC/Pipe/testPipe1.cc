#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib> //stdlib.h
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 2
#define NUM 1024

using namespace std;

// child
void Writer(int wfd)
{
    string s = "hello, I am child";
    pid_t self = getpid();
    int number = 0;

    char buffer[NUM];
    while (true)
    {
        sleep(1);
        // 构建发送字符串
        buffer[0] = 0; // 字符串清空, 只是为了提醒阅读代码的人，我把这个数组当做字符串了
        snprintf(buffer, sizeof(buffer), "%s-%d-%d", s.c_str(), self, number++);
        // cout << buffer << endl;
        // 发送/写入给父进程, system call
        write(wfd, buffer, strlen(buffer)); // strlen(buffer) + 1???
        // char c = 'c';
        // write(wfd, &c, 1); // strlen(buffer) + 1???
        // number++;
        // cout << number << endl;

        // if(number >= 5) break;
    }
}

// father
void Reader(int rfd)
{
    char buffer[NUM];
    int cnt = 0;
    while (true)
    {
        buffer[0] = 0;
        // system call
        ssize_t n = read(rfd, buffer, sizeof(buffer)); // sizeof != strlen
        if (n > 0)
        {
            buffer[n] = 0; // 0 == '\0'
            cout << "father get a message[" << getpid() << "]# " << buffer << endl;
        }
        else if (n == 0)
        {
            printf("father read file done!\n");
            break;
        }
        else
            break;

        cnt++;
        if (cnt > 5)
            break;
        // cout << "n: " << n << endl;
    }
}

int main()
{
    int pipefd[N] = {0};
    int n = pipe(pipefd);
    if (n < 0)
        return 1;

    // cout << "pipefd[0]: " << pipefd[0] << " , pipefd[1]: " << pipefd[1] << endl;

    // child -> w, father->r
    pid_t id = fork();
    if (id < 0)
        return 2;
    if (id == 0)
    {
        // child
        close(pipefd[0]);

        // IPC code
        Writer(pipefd[1]);

        close(pipefd[1]);
        exit(0);
    }
    // father
    close(pipefd[1]);

    // IPC code
    Reader(pipefd[0]); // 读取5s
    close(pipefd[0]);
    cout << "father close read fd: " << pipefd[0] << endl;
    sleep(5); // 为了观察僵尸

    int status = 0;
    pid_t rid = waitpid(id, &status, 0);
    if (rid < 0)
        return 3;

    cout << "wait child success: " << rid << " exit code: " << ((status >> 8) & 0xFF) << " exit signal: " << (status & 0x7F) << endl;

    sleep(5);

    cout << "father quit" << endl;
    return 0;
}