#include <iostream>
#include <thread>
#include "socket.h"
#include "game.h"


using std::map;
using std::string;
using std::vector;


class TDataHandler {
    public:
        bool ProcessReceivedData(const char *data, size_t sz) const {
            for (; sz > 0; --sz, ++data)
                std::cout << *data;
            return false;
        }
};

void FetchHTTPData() {
    TSocket s;
    s.Connect("acm.timus.ru", 80);
    TDataHandler handler;
    std::thread t([&s, &handler] () {
        s.RecvLoop(handler);
    });
    std::string data = "GET / HTTP/1.1\r\nHost: acm.timus.ru\r\n\r\n";
    s.Send(data.c_str(), data.size() + 1);
    t.join();
}

void SendData() {
    TSocket s;
    s.Connect("127.0.0.1", 6654433);
    for (; ;) {
        std::string word;
        std::getline(std::cin, word);
        s.Send(word.c_str(), word.size());
    }
}



int main() {
  SendData();
  return 0;
}
