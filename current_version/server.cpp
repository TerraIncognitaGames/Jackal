#include<iostream>
#include "game.h"

void GameHolder::make_turn(Player& player) { /// нужно куда-то добавить проверку что хоть какой-то ход возможен
  bool flag = false;
  Request request;
  size_t counter = 0;
  static size_t max_requests_without_move = 8;
  while (not flag) {
    if (counter > max_requests_without_move) {
      player.ban(); /// переключаем на бота?
    }
    request = player.get_event_request();
    switch (request.type)
      case DEATH: {
        ++counter;
        player.send("Wrong request");
        break;
      case DROPGOLD:
        ++counter;
        if (accept(request)) {
          send_to_all(request);
        } else {
          player.send("Wrong request");
        }
      case MOVE:
        if (accept(request)) {
          send_to_all(request);
          flag = true;
          break;
        } else {
          ++counter;
          player.send("Wrong request");
          break;
        }
    }
  }
  Pirate* moving_pirate = request.pirate;
  /// А здесь надо что-то делать с бесконечными циклами.
  flag = false;
  while (not flag) {
    switch(get_square(request.destination)->effectType(request.pirate)){
      case STOP:
        flag = true;
        break;
      case ASK:
        request = player.get_event_request();
        while (request.pirate != moving_pirate or ~accept(request)) {
          ++counter;
          if (counter > max_requests_without_move) {
            player.ban();
          }
          player.send("Wrong request");
          request = player.get_event_request();
        }
        send_to_all(request);
        break;
      case GOON:
        request = generate_request(moving_pirate);
        if (accept(request)) {
          send_to_all(request);
        } else {
          flag = true;
          send_to_all(request);
          request = Request(EventType::DEATH, moving_pirate, request.destination, 0);
        }
    }
  }
}

int main() {
  GameHolder game;
  // std::cout << game.get_square(Point(1, 1))->type();
  game.make_turn(game.players_[0]);
  return 0;
}
