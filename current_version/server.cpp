#include<iostream>
#include "game.h"

class ServerPlayer: public Player { // TODO: rename
// информация о подключении
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

  virtual ~ServerPlayer() {
    // std::cout << "Server player destructor" << std::endl;
  }

  ServerPlayer(size_t id, string login, Point ship_coord)
    : Player(id, login, ship_coord){
      for (size_t i=0; i < numberOfPirates; ++i){
        pirates.push_back(Pirate(ship_coord));
      }
      std::cout << "Player constructor" << std::endl;
    }
};

void GameMap::init(size_t size) {
  /// вектор, из которого рандомайзер для каждой клетки erase-ит рандомное значение,
  /// при вытягивании : ARROW , GUN -- нас ещё должна волновать ориентация (!)
  std::vector<SquareType> SquareTypesForMapCreation;
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 64, FIELD);
  /// на некоторых клетках должно сразу валяться золото (1 :5карт, 2 :5карт, 3 :3карты, 4 :2карты, 5 :1карта)
  /// в конструктор клетки
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 5, JUNGLE);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 4, DESERT);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 2, BOG);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 1, MOUNTAIN);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 2, BALOON);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 4, CROCODILE);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 6, ICE);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 2, HOARSE);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 2, FORTRESS);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 1, ABORIGINE);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 1, CANNIBAL);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 2, GUN);
  /// ориентация будет выбрана в конструкторе клетки
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 6, SINGLEARROW);
  SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 15, ARROWS);
  /// ориентация и вариации (7х3) -//-
  /// эти настройки нужно будет вынести из этой функции в другое место
  vector<SquareBase*> new_column;
  for (size_t i = 0; i < size; ++i) {
    new_column.resize(0);
    for (size_t i = 0; i < size; ++i){
      new_column.push_back(new SquareWater);
    }
    push_back(new_column);
  }
}

class ServerGameHolder: public GameHolder {
public:
  ServerGameHolder(vector<Player*> players_)
    : GameHolder(players_) {
  }

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
    /// А здесь надо что-то делать с бесконечными циклами.
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
  vector<Player*> players;
  players.push_back(new ServerPlayer(0, "A", Point((sizeOfIsland + 1) / 2, sizeOfIsland + 1)));
  players.push_back(new ServerPlayer(1, "B", Point(sizeOfIsland + 1, (sizeOfIsland + 1)/2)));
  players.push_back(new ServerPlayer(2, "C", Point((sizeOfIsland + 1)/2, 0)));
  players.push_back(new ServerPlayer(3, "D", Point(0, (sizeOfIsland + 1)/2)));
  ServerGameHolder game(players);
  // std::cout << game.get_square(Point(1, 1))->type();
  game.make_turn(game.players_[0]);
  game.make_turn(game.players_[1]);
  return 0;
}
