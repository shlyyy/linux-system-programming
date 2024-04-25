#include "TcpServer.hpp"
#include <unistd.h>
// #include "Daemon.hpp"

static void Usage(const std::string &proc)
{
    std::cout << "\nUsage: " << proc << " port\n"
              << std::endl;
}

// ./servercal 8080
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }
    uint16_t port = std::stoi(argv[1]);
    ServerCal cal;
    TcpServer *tsvp = new TcpServer(port,
                                    std::bind(&ServerCal::Calculator, &cal, std::placeholders::_1));
    tsvp->InitServer();
    // Daemon();
    daemon(0, 0);
    tsvp->Start();

    // Response resp(1000, 0);
    // std::string content;
    // resp.Serialize(&content);
    // std::cout << content << std::endl;
    // std::string package = Encode(content);
    // std::cout << package;

    // content = "";
    // bool r = Decode(package, &content);
    // std::cout << content << std::endl;

    // Response temp;
    // temp.Deserialize(content);

    // std::cout << temp.result << std::endl;
    // std::cout << temp.code << std::endl;

    // Request req(12364566, 43454356, '+');
    // std::string s;
    // req.Serialize(&s);
    // s = Encode(s);
    // std::cout << s;

    // std::string content;
    // bool r = Decode(s, &content);
    // std::cout << content << std::endl;
    // Request temp;
    // temp.Deserialize(content);

    // std::cout << temp.x << std::endl;
    // std::cout << temp.op << std::endl;
    // std::cout << temp.y << std::endl;
    return 0;
}