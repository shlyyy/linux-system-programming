#include <iostream>
#include "contacts.pb.h"

using namespace std;

int main()
{
    // 序列化
    Contacts::PersonInfo p1;
    p1.set_name("Jack");
    p1.set_age(22);
    string p1_str;
    if (!p1.SerializeToString(&p1_str))
        cout << "Serialize Failed!" << endl;
    cout << "Serialize Success!" << endl;
    cout << "Serialized: " << p1_str << endl;

    // 反序列化
    Contacts::PersonInfo p2;
    if (!p2.ParseFromString(p1_str))
        cout << "Parse Failed!" << endl;
    cout << "Parse Success!" << endl;
    cout << "Parsed: " << p2.name() << " " << p2.age() << endl;

    return 0;
}