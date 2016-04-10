#include<iostream>
#include "game.h"

void GameHolder::make_turn(Player& player) { /// нужно куда-то добавить проверку что хоть какой-то ход возможен
  bool flag = false;
  Event request;
  size_t counter = 0;
  static size_t max_requests_without_move = 8;
  while (~flag) {
    if (counter > max_requests_without_move) {
      player.ban(); /// переключаем на бота?
    }
    request = player.get_event_request();
    switch (request.type) {
      case DEATH: {
        ++counter;
        player.send("Wrong request");
        break;
      }
      case DROPGOLD: {
        ++counter;
        if (accept(request)) {
          send_to_all(request);
        } else {
          player.send("Wrong request");
        }
      }
      case MOVE: {
        if (accept(request)) {
          send_to_all(request);
          flag = true;
          break;
        } else {
          player.send("Wrong request");
          break;
        }
      }
    }
  }
  /// А здесь надо что-то делать с бесконечными циклами.

}

int main() {
  GameHolder x;
  std::cout << x[Point(1, 1)]->type();
  return 0;
}
