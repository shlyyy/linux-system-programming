#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define PORT 8888

int main()
{
    int sockfd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr, client_addr;
    fd_set readfds;
    int max_clients = MAX_CLIENTS;
    int max_sd, activity, i, valread;
    char buffer[1024] = {0};

    // 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定地址和端口
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接
    if (listen(sockfd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections on port %d...\n", PORT);

    // 初始化客户端套接字数组
    for (i = 0; i < max_clients; i++)
    {
        client_sockets[i] = 0;
    }

    while (1)
    {
        // 清空文件描述符集合
        FD_ZERO(&readfds);

        // 添加主套接字到集合
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        // 添加客户端套接字到集合
        for (i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0)
            {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        // 等待文件描述符就绪
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // 如果主套接字有新连接
        if (FD_ISSET(sockfd, &readfds))
        {
            if ((new_socket = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, IP is : %s, port : %d\n", new_socket,
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // 将新连接加入到客户端套接字数组
            for (i = 0; i < max_clients; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // 处理客户端请求
        for (i = 0; i < max_clients; i++)
        {
            int sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds))
            {
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // 客户端断开连接
                    printf("Client disconnected, socket fd is %d\n", sd);
                    close(sd);
                    client_sockets[i] = 0;
                }
                else
                {
                    // 处理客户端消息
                    buffer[valread] = '\0';
                    printf("Received message from client: %s\n", buffer);

                    // 回复客户端
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}
