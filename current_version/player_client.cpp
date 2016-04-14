#include <iostream>
#include <thread>
#include "socket.h"
#include "game.h"


string CreateRequest() {
  /// зависит от графики и способа обозначения хода :(
}

void SendRequest() {
  TSocket s;
  s.Connect("127.0.0.1", 6655442);
  for (; ;) {
    std::string word;
    std::getline(std::cin, word);
    s.Send(word.c_str(), word.size());
  }
}

int main() {
  SendRequest();
  return 0;
}
