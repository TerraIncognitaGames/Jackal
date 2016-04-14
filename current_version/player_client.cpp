#include <iostream>
#include <thread>
#include "socket.h"
#include "game.h"


class ServerPlayer: public Player {

  string CreateRequest() {
  /// зависит от графики и способа обозначения хода :(
  }
};
/// что должно быть на самом деле : получать данные с сервера, парсить их, хранить известную карту, изменять карту и положения участников, \
ждать запроса от сервера, делать строку-запрос, проверять запрос, ждать даные с сервера

/// ----------------------- к сокетам :

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
