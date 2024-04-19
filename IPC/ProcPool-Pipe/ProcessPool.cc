#include "Task.hpp"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

const int processnum = 10;
std::vector<task_t> tasks;

// 先描述
class channel
{
public:
    channel(int cmdfd, int slaverid, const std::string &processname)
        : _cmdfd(cmdfd), _slaverid(slaverid), _processname(processname)
    {
    }

public:
    int _cmdfd;               // 发送任务的文件描述符
    pid_t _slaverid;          // 子进程的PID
    std::string _processname; // 子进程的名字 -- 方便我们打印日志
    // int _cmdcnt;
};

void slaver()
{
    // read(0)
    while (true)
    {
        int cmdcode = 0;
        int n = read(0, &cmdcode, sizeof(int)); // 如果父进程不给子进程发送数据呢？？阻塞等待！
        if (n == sizeof(int))
        {
            // 执行cmdcode对应的任务列表
            std::cout << "slaver say@ get a command: " << getpid() << " : cmdcode: " << cmdcode << std::endl;
            if (cmdcode >= 0 && cmdcode < tasks.size())
                tasks[cmdcode]();
        }
        if (n == 0)
            break;
    }
}
// 输入：const &
// 输出：*
// 输入输出：&
void InitProcessPool(std::vector<channel> *channels)
{
    // version 2: 确保每一个子进程都只有一个写端
    std::vector<int> oldfds;
    for (int i = 0; i < processnum; i++)
    {
        int pipefd[2]; // 临时空间
        int n = pipe(pipefd);
        assert(!n); // 演示就可以
        (void)n;

        pid_t id = fork();
        if (id == 0) // child
        {
            std::cout << "child: " << getpid() << " close history fd: ";
            for (auto fd : oldfds)
            {
                std::cout << fd << " ";
                close(fd);
            }
            std::cout << "\n";

            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            slaver();
            std::cout << "process : " << getpid() << " quit" << std::endl;
            // slaver(pipefd[0]);
            exit(0);
        }
        // father
        close(pipefd[0]);

        // 添加channel字段了
        std::string name = "process-" + std::to_string(i);
        channels->push_back(channel(pipefd[1], id, name));
        oldfds.push_back(pipefd[1]);

        sleep(1);
    }
}

void Debug(const std::vector<channel> &channels)
{
    // test
    for (const auto &c : channels)
    {
        std::cout << c._cmdfd << " " << c._slaverid << " " << c._processname << std::endl;
    }
}

void Menu()
{
    std::cout << "################################################" << std::endl;
    std::cout << "# 1. 刷新日志             2. 刷新出来野怪        #" << std::endl;
    std::cout << "# 3. 检测软件是否更新      4. 更新用的血量和蓝量  #" << std::endl;
    std::cout << "#                         0. 退出               #" << std::endl;
    std::cout << "#################################################" << std::endl;
}

void ctrlSlaver(const std::vector<channel> &channels)
{
    int which = 0;
    // int cnt = 5;
    while (true)
    {
        int select = 0;
        Menu();
        std::cout << "Please Enter@ ";
        std::cin >> select;

        if (select <= 0 || select >= 5)
            break;
        // select > 0&& select < 5
        // 1. 选择任务
        // int cmdcode = rand()%tasks.size();
        int cmdcode = select - 1;

        // 2. 选择进程
        // int processpos = rand()%channels.size();

        std::cout << "father say: "
                  << " cmdcode: " << cmdcode << " already sendto " << channels[which]._slaverid << " process name: "
                  << channels[which]._processname << std::endl;
        // 3. 发送任务
        write(channels[which]._cmdfd, &cmdcode, sizeof(cmdcode));

        which++;
        which %= channels.size();

        // cnt--;
        // sleep(1);
    }
}

void QuitProcess(const std::vector<channel> &channels)
{
    for (const auto &c : channels)
    {
        close(c._cmdfd);
        waitpid(c._slaverid, nullptr, 0);
    }
    // version1
    // int last = channels.size()-1;
    // for(int i = last; i >= 0; i--)
    // {
    //     close(channels[i]._cmdfd);
    //     waitpid(channels[i]._slaverid, nullptr, 0);
    // }

    // for(const auto &c : channels) close(c._cmdfd);
    // // sleep(5);
    // for(const auto &c : channels) waitpid(c._slaverid, nullptr, 0);
    // // sleep(5);
}
int main()
{
    LoadTask(&tasks);

    srand(time(nullptr) ^ getpid() ^ 1023); // 种一个随机数种子
    // 在组织
    std::vector<channel> channels;
    // 1. 初始化 --- bug?? -- 找一下这个问题在哪里？然后提出一些解决方案！
    InitProcessPool(&channels);
    // Debug(channels);

    // 2. 开始控制子进程
    ctrlSlaver(channels);

    // 3. 清理收尾
    QuitProcess(channels);
    return 0;
}