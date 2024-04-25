#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unordered_map>

#include "Socket.hpp"
#include "Log.hpp"

const std::string wwwroot = "./wwwroot"; // web 根目录
const std::string sep = "\r\n";
const std::string homepage = "index.html";

static const int defaultport = 8082;

class HttpServer;

class ThreadData
{
public:
    ThreadData(int fd, HttpServer *s) : sockfd(fd), svr(s)
    {
    }

public:
    int sockfd;
    HttpServer *svr;
};

class HttpRequest
{
public:
    void Deserialize(std::string req)
    {
        while (true)
        {
            std::size_t pos = req.find(sep);
            if (pos == std::string::npos)
                break;
            std::string temp = req.substr(0, pos);
            if (temp.empty())
                break;
            req_header.push_back(temp);
            req.erase(0, pos + sep.size());
        }
        text = req;
    }
    // .png:image/png
    void Parse()
    {
        std::stringstream ss(req_header[0]);
        ss >> method >> url >> http_version;
        file_path = wwwroot; // ./wwwroot
        if (url == "/" || url == "/index.html")
        {
            file_path += "/";
            file_path += homepage; // ./wwwroot/index.html
        }
        else
            file_path += url; // /a/b/c/d.html->./wwwroot/a/b/c/d.html

        auto pos = file_path.rfind(".");
        if (pos == std::string::npos)
            suffix = ".html";
        else
            suffix = file_path.substr(pos);
    }
    void DebugPrint()
    {
        for (auto &line : req_header)
        {
            std::cout << "--------------------------------" << std::endl;
            std::cout << line << "\n\n";
        }

        std::cout << "method: " << method << std::endl;
        std::cout << "url: " << url << std::endl;
        std::cout << "http_version: " << http_version << std::endl;
        std::cout << "file_path: " << file_path << std::endl;
        std::cout << text << std::endl;
    }

public:
    std::vector<std::string> req_header;
    std::string text;

    // 解析之后的结果
    std::string method;
    std::string url;
    std::string http_version;
    std::string file_path; // ./wwwroot/a/b/c.html 2.png

    std::string suffix;
};

class HttpServer
{
public:
    HttpServer(uint16_t port = defaultport) : port_(port)
    {
        content_type.insert({".html", "text/html"});
        content_type.insert({".png", "image/png"});
    }
    bool Start()
    {
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        for (;;)
        {
            std::string clientip;
            uint16_t clientport;
            int sockfd = listensock_.Accept(&clientip, &clientport);
            if (sockfd < 0)
                continue;
            lg(Info, "get a new connect, sockfd: %d", sockfd);
            pthread_t tid;
            ThreadData *td = new ThreadData(sockfd, this);
            pthread_create(&tid, nullptr, ThreadRun, td);
        }
    }
    static std::string ReadHtmlContent(const std::string &htmlpath)
    {
        // 坑
        std::ifstream in(htmlpath, std::ios::binary);
        if (!in.is_open())
            return "";

        in.seekg(0, std::ios_base::end);
        auto len = in.tellg();
        in.seekg(0, std::ios_base::beg);

        std::string content;
        content.resize(len);

        in.read((char *)content.c_str(), content.size());
        // std::string content;
        // std::string line;
        // while(std::getline(in, line))
        //{
        //     content += line;
        // }

        in.close();

        return content;
    }
    std::string SuffixToDesc(const std::string &suffix)
    {
        auto iter = content_type.find(suffix);
        if (iter == content_type.end())
            return content_type[".html"];
        else
            return content_type[suffix];
    }
    void HandlerHttp(int sockfd)
    {
        char buffer[10240];
        ssize_t n = recv(sockfd, buffer, sizeof(buffer) - 1, 0); // bug
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << buffer << std::endl; // 假设我们读取到的就是一个完整的，独立的http 请求
            HttpRequest req;
            req.Deserialize(buffer);
            req.Parse();
            // req.DebugPrint();

            // std::string path = wwwroot;
            // path += url; // wwwroot/a/a/b/index.html

            // 返回响应的过程
            std::string text;
            bool ok = true;
            text = ReadHtmlContent(req.file_path); // 失败？
            if (text.empty())
            {
                ok = false;
                std::string err_html = wwwroot;
                err_html += "/";
                err_html += "err.html";
                text = ReadHtmlContent(err_html);
            }

            std::string response_line;
            if (ok)
                response_line = "HTTP/1.0 200 OK\r\n";
            else
                response_line = "HTTP/1.0 404 Not Found\r\n";

            // response_line = "HTTP/1.0 302 Found\r\n";
            std::string response_header = "Content-Length: ";
            response_header += std::to_string(text.size()); // Content-Length: 11
            response_header += "\r\n";
            response_header += "Content-Type: ";
            response_header += SuffixToDesc(req.suffix);
            response_header += "\r\n";
            response_header += "Set-Cookie: name=haha&&passwd=12345";
            response_header += "\r\n";

            // response_header += "Location: https://www.qq.com\r\n";
            std::string blank_line = "\r\n"; // \n

            std::string response = response_line;
            response += response_header;
            response += blank_line;
            response += text;

            send(sockfd, response.c_str(), response.size(), 0);
        }
        close(sockfd);
    }
    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->svr->HandlerHttp(td->sockfd);
        delete td;
        return nullptr;
    }
    ~HttpServer()
    {
    }

private:
    Sock listensock_;
    uint16_t port_;
    std::unordered_map<std::string, std::string> content_type;
};