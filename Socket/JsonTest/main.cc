#include <iostream>
#include "jsoncpp/json/json.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "hello json" << endl;

    Json::Value part1;
    part1["haha"] = "haha";
    part1["hehe"] = "hehe";

    Json::Value root;
    root["x"] = 100;
    root["y"] = 200;
    root["op"] = '+';
    root["desc"] = "this is a + oper";
    root["test"] = part1;

    Json::FastWriter w;
    // Json::StyledWriter w;
    std::string res = w.write(root);

    std::cout << res << std::endl;

    cout << "=============================" << endl;
    
    Json::Value v;
    Json::Reader r;
    r.parse(res, v);

    int x = v["x"].asInt();
    int y = v["y"].asInt();
    char op = v["op"].asInt();
    std::string desc = v["desc"].asString();
    Json::Value temp = v["test"];
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << op << std::endl;
    std::cout << desc << std::endl;
    return 0;
}
