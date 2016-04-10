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
enum Direction { TOP, BOTTOM, RIGHT, LEFT, TOPRIGHT, TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT  };
enum SquareType { UNEXPLORED, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAIN, SINGLEARROW, ARROWS, HOARSE, ICE,
                  CROCODILE, BALOON, GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP }; /// Эдик! Стрелочек тоже 7 видов, плохо их -- одним элементом:(
/// пока нет : самолёт, яма, ром, их заменяют обычные клетки field
                  /// предлагаю сделать клетку "YOU_SHALL_NOT_PASS", в которых нет ничего и туда нельзя,
                  /// очень не хочется заморачиваться с угловыми
                  /// Ed: угловые будут проверятся той же функцией, которая проверяет выход за пределы поля.
                  /// Ed: Яму не тяжело будет добавить. Смолет думаю тоже.
                  /// Ed: Все стрелочки, кроме, может быть, одиннарных - один тип со списком направлений.
                  /// В рисовалке может быть увеличим количество типов.
enum EffectOfSquare { STOP, GOON, ASK, DEAD };
enum ActionTypes { LEAVEGOLD, MOVE };
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

  virtual EffectOfSquare effect(Pirate &pirate) {
    std::cout << "error: effect of unexplored";
  }

  SquareType type() {
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
  size_t gold() {
    return gold_;
  }

  size_t num_of_steps() {
    return num_of_steps_;
  }

  virtual EffectOfSquare effect(Pirate &pirate) {
    return STOP;
  }

  SquareType type() {
    return type_;
  }


};

class Ship {
public:
  Ship(Point coord) {

  }
};

class Action {

};

class Player {
public:
  string id;
  std::vector<Pirate> pirates;
  Ship* ship;
  /// Данные для связи

  Player(string id, Point ship_coord)
    : id(id)
    , pirates(0)
    , ship(ship_coord) {
      for (size_t i=0; i < numberOfPirates; ++i){
        pirates.push_back(Pirate(ship_coord));
      }
      std::cout << "Player constructor" << std::endl;
    }

  Action getAction() {
    int i;
    std::cin >> i;
    return
  }

};

class Ship: public SquareBase {
private:
  Point coordinate_;// or store it in player?
public:
  Point coordinate() {
    return coordinate_;
  }

};

typedef std::pair<Pirate*, Point> Move;

/// TODO: Factory!

class Map: public vector<vector<SquareBase*> > {
public:
  Map(size_t size = sizeOfIsland + 2)
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
      for (int i = 0; i < size; ++i) {
        push_back(std::vector<SquareBase*>(size, new SquareBase));
      }
      std::cout << "Map constructor" << std::endl;
    }

};


class GameHolder {
public:
  GameHolder(size_t size = sizeOfIsland):
    map_(size) {
    std::cout << "Field constructor" << std::endl;
  }

  SquareBase* operator[](Point p) {
    return map_[p.x][p.y];
  }
  /*
  Square& operator[](const Player &p) {
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
  int size() {  // name should be lowercase.
    return map_.size();
  }

private:
  Map map_;  // field_[0][0] is a Left Bottom corner.
  std::vector<Player> players;
};


class RequestQuery {};
// class ResponseQuery {};



#endif // GAME_H_INCLUDED

