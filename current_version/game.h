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

/// If you change enum, don't forget to update functions.
enum Direction { TOP, BOTTOM, RIGHT, LEFT, TOPRIGHT, TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT  };
enum SquareType { HIDDEN, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAIN, ARROW, HOARSE, ICE,
                  CROCODILE, BALOON, GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP }; /// Эдик! Стрелочек тоже 7 видов, плохо их -- одним элементом:(
/// пока нет : самолёт, яма, ром, их заменяют обычные клетки field
                  /// предлагаю сделать клетку "YOU_SHALL_NOT_PASS", в которых нет ничего и туда нельзя,
                  /// очень не хочется заморачиваться с угловыми

bool isMovingCellType(SquareType type) {
  switch (type){
    case ARROW:
    case GUN:
    case HOARSE:
    case BALOON:
    case ICE:
    case CROCODILE:/// возвращает тебя на клетку, с которой ты ходил
      return true;
    default:
      return false;
  }
}


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
  bool gold() {
    return gold_;
  }
  Point coordinate() {
    return coordinate_;
  }
  size_t position_on_square() {
    return position_on_square_;
  }

};


class Action {

};

class Player {

};

class SquareBase {
private :
  static SquareType type_;
public :
  SquareBase() {
  }

  virtual Action effect(Pirate &pirate) {
    std::cout << "error";
  }

  SquareType type() {
    return type_;
  }

};
SquareType SquareBase::type_ = HIDDEN;
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

  virtual Action effect(Pirate &pirate) {
  }

  SquareType type() {
    return type_;
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


typedef vector<vector<Square> > Map;
typedef std::function<Map(int)> MapCreaterFunction;

/// вектор, из которого рандомайзер для каждой клетки erase-ит рандомное значение,
/// при вытягивании : ARROW , GUN -- нас ещё должна волновать ориентация (!)
std::vector<SquareType> MapFieldCreaterVector;
insert (MapCreaterVector.end(), 64, FIELD);
insert (MapCreaterVector.end(), 5, JUNGLE);
insert (MapCreaterVector.end(), 4, DESERT);
insert (MapCreaterVector.end(), 2, BOG);
insert (MapCreaterVector.end(), 1, MOUNTAIN);
insert (MapCreaterVector.end(), 2, BALOON);
insert (MapCreaterVector.end(), 4, CROCODILE);
insert (MapCreaterVector.end(), 6, ICE);
insert (MapCreaterVector.end(), 2, HOARSE);
insert (MapCreaterVector.end(), 2, FORTRESS);
insert (MapCreaterVector.end(), 1, ABORIGINE);
insert (MapCreaterVector.end(), 1, CANNIBAL);
insert (MapCreaterVector.end(), 2, GUN); /// ориентация
insert (MapCreaterVector.end(), 21, ARROW); /// ориентация и вариации (7х3)


class Field {
public:
  Field(int size = 6) {
    // creates empty field.
    for (int i = 0; i < size; ++i) {
      map_.push_back(vector<Square>(size, Square()));
    }
  }

  Field(MapCreaterFunction mcf, int size = 6) {
    map_ = mcf(size);
  }

  Square& operator[](Point p) {
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
};




/// water
/// field
/// jungle
/// desert
/// bog
/// mountain

/// arrow
/// hoarse
/// ice
/// crocodile

/// baloon
/// gun

// after_all : hole
// after_all : rum
/// cannibal

/// fortress
/// aborigine


class RequestQuery {};
// class ResponseQuery {};



#endif // GAME_H_INCLUDED

