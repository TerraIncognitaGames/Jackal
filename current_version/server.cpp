#include<iostream>
#include "game.h"

class ServerPlayer: public Player { // TODO: rename
// информаци€ о подключении
public:
  void send(string str) {
    std::cout << login << " get string:   " << str << std::endl;
  }

  void send(Event event) {
    std::cout << login << " get event:   " << event.type << std::endl;
  }


  Request get_event_request() {
    std::cout << login << " enter request: " << std::endl;
    return Request(EventType::MOVE, id, 0, Point(0, 0), 0);
  }

  void ban() {
    send("you are baned");
  }


};

class ServerGameHolder: public GameHolder {
public:
  ServerGameHolder() {
  };

  void make_turn(Player* player_) { /// нужно куда-то добавить проверку что хоть какой-то ход возможен
    ServerPlayer* player = static_cast<ServerPlayer*>(player_);
    bool flag = false;
    Request request;
    size_t counter = 0;
    static size_t max_wrong_requests = 8;
    while (not flag) {
      if (counter > max_wrong_requests) {
        player->ban(); /// переключаем на бота?
      }
      request = player->get_event_request();
      switch (request.type)
        case DEATH: {
          ++counter;
          player->send("Wrong request");
          break;
        case DROPGOLD:
          if (accept(request)) {
            send_to_all(request);
          } else {
            ++counter;
            player->send("Wrong request");
          }
        case MOVE:
          if (accept(request)) {
            send_to_all(request);
            flag = true;
            break;
          } else {
            ++counter;
            player->send("Wrong request");
            break;
          }
      }
    }
    size_t moving_pirate_num = request.pirate_num;
    /// ј здесь надо что-то делать с бесконечными циклами.
    flag = false;
    while (not flag) {
      switch(map_[request.destination.x][request.destination.y]->
             effectType(&players_[request.player_id]->pirates[request.pirate_num])){
        case STOP:
          flag = true;
          for (Request req:attack(player, request.destination)){
            accept_and_send(req);
          }
          break;
        case ASK:
          request = player->get_event_request();
          while (request.pirate_num != moving_pirate_num or ~accept(request)) {
            ++counter;
            if (counter > max_wrong_requests) {
              player->ban();
            }
            player->send("Wrong request");
            request = player->get_event_request();
          }
          send_to_all(request);
          break;
        case GOON:
          request = generate_request(player->id, moving_pirate_num);
          if (accept(request)) {
            send_to_all(request);
          } else {
            flag = true;
            send_to_all(request);
            request = Request(EventType::DEATH, player->id, moving_pirate_num, request.destination, 0);
            accept_and_send(request);
          }
        case KILL:
          flag = true;
          request = generate_request(player->id, moving_pirate_num);
          accept_and_send(request);
      }
    }
    /// подбор монетки происходит автоматически в accept
  }

  ~ServerGameHolder() {}
private:
  void send_to_all(Event event) {
    for (Player* player_: players_) {
      ServerPlayer* player = static_cast<ServerPlayer*>(player_);
      player->send(event);
    }
  }

   void send_to_all(Request req) {
    Event event(req.type, req.player_id, req.pirate_num, req.destination,
                 req.position_on_square, get_square_info(req.destination));
    send_to_all(event);
  }

  void accept_and_send(Request req) {
    accept(req);
    send_to_all(req);
  }

};

int main() {
  ServerGameHolder game;
  // std::cout << game.get_square(Point(1, 1))->type();
  game.make_turn(game.players_[0]);
  game.make_turn(game.players_[1]);
  return 0;
}
