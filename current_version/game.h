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

const size_t sizeOfIsland = 11; // без воды
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
enum EffectOfSquare: char { STOP, GOON, ASK, DEAD };
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


  bool IsCorrectPoint(int size) { // rewrite
    return (x >= 0 && x < size && y >= 0 && y < size);
  }

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
    }
  bool gold() {
    return gold_;
  }
  Point coordinate() {
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
private :
  static SquareType type_;
  bool explored_; /// чтобы сервер мог следить за циклами
public :
  SquareBase()
    : explored_(false) {
      }

  virtual EffectOfSquare effectType(Pirate* pirate) const {
    return STOP;
  }

  virtual SquareType type() const {
    return type_;
  }

};
SquareType SquareBase::type_ = UNEXPLORED;
typedef SquareBase HiddenSquare;


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


};

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

struct Request {
  EventType type;
  Pirate* pirate;
  Point destination;
  size_t position_on_square;
  Request(EventType type, Pirate* pirate, Point destination, size_t position_on_square)
    :type(type)
    , pirate(pirate)
    , destination(destination)
    , position_on_square(position_on_square)
     {}

  Request() {}
};

class Event: public Request {
public:
  SquareType square_type;

  Event(EventType type, Pirate* pirate, Point destination, size_t position_on_square, SquareType square_type)
    : Request(type, pirate, destination, position_on_square)
    , square_type(square_type)
     {}

  Event(Request req, SquareType square_type)
    : Request(req)
    , square_type(square_type)
      {}
};

class Player {
public:
  string id;
  std::vector<Pirate> pirates;
  Ship* ship;
  /// Данные для связи

  Player(string id, Point ship_coord)
    : id(id)
    , pirates()
    , ship(new Ship(ship_coord)) {
      for (size_t i=0; i < numberOfPirates; ++i){
        pirates.push_back(Pirate(ship_coord));
      }
      std::cout << "Player constructor" << std::endl;
    }

  void send(string str) {
    std::cout << id << " get string:   " << str << std::endl;
  }

  void send(Event event) {
    std::cout << id << " get event:   " << event.type << std::endl;
  }


  Request get_event_request() {
    std::cout << id << " enter request: " << std::endl;
    return Request(EventType::MOVE, &pirates[0], Point(0, 0), 0);
  }

  void ban() {
    send("you are baned");
  }
};


/// TODO: Factory!

class GameMap: public vector<vector<SquareBase*> > {
public:
  GameMap(size_t size = sizeOfIsland + 2)
    : vector<vector<SquareBase*> >(0)
    {
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
      for (size_t i = 0; i < size; ++i) {
        push_back(std::vector<SquareBase*>(size, new SquareBase));
      }
      std::cout << "Map constructor" << std::endl;
    }

};


class GameHolder {
public:
  GameHolder(size_t size = sizeOfIsland)
    : map_(size)
    , players_() {
        players_.push_back(Player("A", Point((sizeOfIsland + 1) / 2, sizeOfIsland + 1)));
        players_.push_back(Player("B", Point(sizeOfIsland + 1, (sizeOfIsland + 1)/2)));
        players_.push_back(Player("C", Point((sizeOfIsland + 1)/2, 0)));
        players_.push_back(Player("D", Point(0, (sizeOfIsland + 1)/2)));
        std::cout << "Game constructor" << std::endl;
      }

  const SquareBase* get_square(Point p) {
    return map_[p.x][p.y];
  }



  bool possible_event(Request& event) const {
    return true;
  }

  bool accept(Request& event) {
    if (not possible_event(event)) {
      return false;
    }
    /// Do something
    return true;
    /// здесь прописываются все изменения на карте: с пиратом, и с клеткой под ним
    /// на которого указывает событие (перемещение или бросок монетки или смерть) но не вся цепочка событий
    /// в клиенте происходит просто accept всех событий отправленных сервером
  }

  Request generate_request(Pirate* pirate) {
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

  void make_turn(Player& player); // объявить удаленным в клиенте

private:
  GameMap map_;  // field_[0][0] is a Left Bottom corner.
public:
  std::vector<Player> players_;

  void send_to_all(Event event) {
    for (Player player: players_) {
      player.send(event);
    }
  }

  void send_to_all(Request req) {
    Event event(req.type, req.pirate, req.destination, req.position_on_square, get_square(req.destination)->type());
    send_to_all(event);
  }

};


class RequestQuery {};
// class ResponseQuery {};



#endif // GAME_H_INCLUDED

