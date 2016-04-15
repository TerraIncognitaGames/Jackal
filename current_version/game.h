#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <functional>
#include <time.h>


using std::map;
using std::string;
using std::vector;

const size_t sizeOfIsland = 3; // без воды
const size_t numberOfPirates = 3;

/// If you change enum, don't forget to update functions.
enum Direction: char { TOP, BOTTOM, RIGHT, LEFT, TOPRIGHT, TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT  };
enum SquareType: char { UNEXPLORED, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAIN, SINGLEARROW, ARROWS, HOARSE, ICE,
                  CROCODILE, BALOON, GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP }; /// Эдик! Стрелочек тоже 7 видов, плохо их -- одним элементом:(
/// пока нет : самолёт, яма, ром, их заменяют обычные клетки field
                  /// предлагаю сделать клетку "YOU_SHALL_NOT_PASS", в которых нет ничего и туда нельзя,
                  /// очень не хочется заморачиваться с угловыми
                  /// Ed: угловые будут проверятся той же функцией, которая проверяет выход за пределы поля.
                  /// Ed: Яму не тяжело будет добавить. Смолет думаю тоже.
                  /// Ed: Все стрелочки, кроме, может быть, одиннарных - один тип со списком направлений.
                  /// В рисовалке может быть увеличим количество типов.
enum EffectOfSquare: char { STOP, GOON, ASK, KILL };
enum EventType: char { DROPGOLD, MOVE, DEATH };

/*
EffectOfSquare effectOfCellType(SquareType type) {
  switch (type){
    case SINGLEARROW:
    case ARROWS:
    case GUN:
    case HOARSE:
    case BALOON:
    case ICE:
    case CROCODILE:
      return true;
    default:
      return false;
  }
}*/


class Point {
public:
  int x, y;
  Point(int x_coord = 0, int y_coord = 0)
    : x(x_coord), y(y_coord) {}


  bool operator== (const Point &other) const {
    return ((other.x == x) && (other.y == y));
  }

  Point GetPointToThe(Direction dir) {
    switch (dir) {
      case TOP:
        return Point(x, y + 1);
      case BOTTOM:
        return Point(x, y - 1);
      case RIGHT:
        return Point(x + 1, y);
      case LEFT:
        return Point(x - 1, y);
      case TOPRIGHT:
        return Point(x + 1, y + 1);
      case TOPLEFT:
        return Point(x - 1, y + 1);
      case BOTTOMLEFT:
        return Point(x - 1, y - 1);
      case BOTTOMRIGHT:
        return Point(x + 1, y - 1);
    }
  }


  bool IsCorrectPoint(int size) { /// Иначе лезут warningи
    return (x >= 0 && x < size && y >= 0 && y < size && ~((x == size||x == 0) and (y == size || y == 0)));
  }

};

class Pirate;

struct Request {
  EventType type;
  size_t player_id;
  size_t pirate_num;
  Point destination;
  size_t position_on_square;
  Request(EventType type, size_t player_id, size_t pirate_num, Point destination, size_t position_on_square)
    :type(type)
    , player_id(player_id)
    , pirate_num(pirate_num)
    , destination(destination)
    , position_on_square(position_on_square)
     {}

  Request() {}
};

class Pirate {
private :
  bool gold_;
  Point coordinate_;
  size_t position_on_square_;
  bool dead_;

public :
  Pirate(Point coord)
    : gold_(0)
    , coordinate_(coord)
    , position_on_square_(0)
    , dead_(false) {
      std::cout << "Pirate created" << std::endl;
      /// не могу понять, почему каждый пират при создании дублируется \(О.о)/
      /// см. строку в server.cpp players.push_back(new ServerPlayer(0, "A", Point((sizeOfIsland + 1) / 2, sizeOfIsland + 1)));
    }

  bool gold() const {
    return gold_;
  }
  Point coordinate() const {
    return coordinate_;
  }
  size_t position_on_square() {
    return position_on_square_;
  }

  void kill() {
    dead_ = true;
    gold_ = false;
    position_on_square_ = 0;
  }

};

class SquareBase {
private:
  SquareType type_;
  bool explored_; /// чтобы сервер мог следить за циклами
public :
  SquareBase()
    : type_(UNEXPLORED)
    , explored_(false){
      }

  SquareBase(SquareType type, bool explored)
    : type_(type)
    , explored_(explored){
      }

  virtual EffectOfSquare effectType(Pirate* pirate) const {
    return STOP;
  }

  virtual SquareType type() const {
    return type_;
  }

  virtual ~SquareBase() {
    // std::cout << "SqBase destructor" << std::endl;
  }

  virtual string info() const{
    string result;
    result.push_back(char(type()));
    return result;
  }
};

typedef SquareBase SquareUnexplored;

class SquareStop: public SquareBase { // на таких клетках может кончатся ход
public:
  SquareStop(SquareType type, bool explored)
    : SquareBase(type, explored){}

  SquareStop()
    : SquareBase(WATER, true) {}

  virtual EffectOfSquare effectType(Pirate* pirate) const {
    return STOP;
  }

  virtual ~SquareStop() { };
};

typedef SquareStop SquareWater;

/*
class Square {
private :
  size_t gold_;
  size_t num_of_steps_;
  SquareType type_;
public :
  size_t gold() const {
    return gold_;
  }

  size_t num_of_steps() const {
    return num_of_steps_;
  }

  virtual EffectOfSquare effect() const {
    return STOP;
  }

  SquareType type() const {
    return type_;
  }
};*/

class Ship {
public:
  Ship(Point coord)
  : coordinate_(coord){

  }

  Point coordinate() const {
    return coordinate_;
  }
private:
  Point coordinate_;// or store it in player?

};

class FactoryForSquares {
  /// сделать фабрику!!!
};



class Event: public Request {
public:
  string square_info;

  Event(EventType type, size_t player_id, size_t pirate_num, Point destination, size_t position_on_square, string square_info)
    : Request(type, player_id, pirate_num, destination, position_on_square)
    , square_info(square_info)
     {}

  Event(Request req, string square_info)
    : Request(req)
    , square_info(square_info)
      {}
};

class Player {
public:
  size_t id;
  string login;
  std::vector<Pirate> pirates;
  Ship* ship;

  Player(size_t id, string login, Point ship_coord)
    : id(id)
    , login(login)
    , pirates()
    , ship(new Ship(ship_coord)) {
      for (size_t i=0; i < numberOfPirates; ++i){
        pirates.push_back(Pirate(ship_coord));
      }
      std::cout << "Player constructor" << std::endl;
    }

  virtual ~Player() {
  }
};



class GameMap: public vector<vector<SquareBase*> > {
public:
  GameMap(size_t size = (sizeOfIsland + 2))
    : vector<vector<SquareBase*> >(0)
    {
      init(size);
      std::cout << "Map constructor" << std::endl;
    }

    void init(size_t size); /// Эта функция реализуется по разному для сервера и клиента

    ~GameMap() {
      for (auto i:*this) {
        for (SquareBase* j: i){
          delete j;
        }
      }
    }

};


class GameHolder {
public:
  GameHolder(vector<Player*>& players, size_t size = sizeOfIsland + 2)
    : map_(size)
    , players_(players) {
        std::cout << "Game constructor" << std::endl;
      }

  SquareType get_square_type(Point p) const {
    return map_[p.x][p.y]->type();
  }

  string get_square_info(Point p) const {
    return map_[p.x][p.y]->info();
  }

  vector<Pirate*> getPiratesAtPoint(Point coor) const {
    vector<Pirate*> result;
    for (Player* player: players_) {
      for (Pirate pirate: player->pirates) {
        if (pirate.coordinate() == coor) {
          result.push_back(&pirate);
        }
      }
    }
    return result;
  }

  bool possible_req(Request& req) const {
    return true;
  }
  bool accept(Request& req) {
    if (not possible_req(req)) {
      return false;
    }
    /// Do something
    return true;
    /// здесь прописываются все изменения на карте: с пиратом, и с клеткой под ним
    /// на которого указывает событие (перемещение или бросок монетки или смерть) но не вся цепочка событий
    /// в клиенте происходит просто accept всех событий отправленных сервером
  }

  Request generate_request(size_t player_id, size_t pirate_num) {
    // Будет определять, что происходит с пиратом когда он оказался на клетке которая не дает выбора
  }

  /*SquareBase* operator[](const Player &p) {
    return map_[p.coord.x][p.coord.y];
  }
  void Bash(Point coord, Direction direction) {
    int size = map_.size();
    (*this)[coord][direction] = false;
    Point adjacent_point = GetPointToThe(coord, direction);
    if (IsCorrectPoint(adjacent_point, map_.size())) {
      (*this)[coord][GetOppositeDirection(direction)] = false;
    }
  }*/
  int size() const {  // name should be lowercase.
    return map_.size();
  }

  vector<Request> attack(const Player* player, Point coor) const {
    std::cout << player->login << " attacks " << coor.x <<", "<< coor.y << std::endl;
    vector<Request> result(0);
    for (Player* other_player: players_) {
      if (player->id == other_player->id) {
      } else {
        for (size_t i=0; i<numberOfPirates; ++i) {
          if (player->pirates[i].coordinate() == coor) {
            if (get_square_type(coor) == WATER || get_square_type(coor) == SHIP) {
              result.push_back(Request(EventType::DEATH, other_player->id, i, coor, 0));
            } else {
              result.push_back(Request(EventType::MOVE, other_player->id, i, other_player->ship->coordinate(), 0));
            }
          }
        }
      }
    }
    return result;
  }

  ~GameHolder() {
    for (Player* player:players_) {
      delete(player);
    }
    std::cout << "Game holder destructor" << std::endl;
  }

protected:
  GameMap map_;  // field_[0][0] is a Left Bottom corner.
public:
  std::vector<Player*> players_;
};


class RequestQuery {};
// class ResponseQuery {};



#endif // GAME_H_INCLUDED

