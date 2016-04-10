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
enum SquareType { HIDDEN, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAIN, SINGLEARROW, ARROWS, HOARSE, ICE,
                  CROCODILE, BALOON, GUN, CANNIBAL, FORTRESS, ABORIGINE, SHIP }; /// Эдик! Стрелочек тоже 7 видов, плохо их -- одним элементом:(
/// пока нет : самолёт, яма, ром, их заменяют обычные клетки field
                  /// предлагаю сделать клетку "YOU_SHALL_NOT_PASS", в которых нет ничего и туда нельзя,
                  /// очень не хочется заморачиваться с угловыми
                  /// Ed: угловые будут проверятся той же функцией, которая проверяет выход за пределы поля.
                  /// Ed: Яму не тяжело будет добавить. Смолет думаю тоже.
                  /// Ed: Все стрелочки, кроме, может быть, одиннарных - один тип со списком направлений.
                  /// В рисовалке может быть увеличим количество типов.


bool isMovingCellType(SquareType type) {
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


class Map: public vector<vector<SquareBase*> > {

  Map()
    : vector<vector<SquareBase*> >(0){
      /// вектор, из которого рандомайзер для каждой клетки erase-ит рандомное значение,
      /// при вытягивании : ARROW , GUN -- нас ещё должна волновать ориентация (!)
      std::vector<SquareType> SquareTypesForMapCreation;
      SquareTypesForMapCreation.insert(SquareTypesForMapCreation.end(), 64, FIELD);
      /// на некоторых клетках должно сразу валяться золото (1 :5карт, 2 :5карт, 3 :3карты, 4 :2карты, 5 :1карта) /// в конструктор клетки
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
  }
};


class Field {
public:
  Field(int size = 6) {
    // creates empty field.
    for (int i = 0; i < size; ++i) {
      map_.push_back(vector<SquareBase*>(size, new BaseSquare));
    }
  }

  Field(int size = 6):
    map(size) {

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
  std::vector<Player> players;
};


class RequestQuery {};
// class ResponseQuery {};



#endif // GAME_H_INCLUDED

