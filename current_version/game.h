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

enum Direction { TOP, BOTTOM, RIGHT, LEFT, TOPRIGHT, TOPLEFT, BOTTOMRIGHT, BOTTOMLEFT  };
enum SquareType { HIDDEN, WATER, FIELD, JUNGLE, DESERT, BOG, MOUNTAIN, ARROW, HOARSE, ICE, CROCODILE, BALOON, GUN, CANNIBAL, FORTRESS, ABORIGINE };


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


class Action {};

class Player {

};

class Square {
private :
  size_t gold_;
  size_t num_of_steps_;
  SquareType type_;
public :
  size_t gold() {
    return gold;
  }
  size_t num_of_steps() {
    return num_of_steps_;
  }
  Action effect(Pirate &pirate) {
  }
  SquareType type {
    return type_;
  }


};

typedef vector<vector<Square> > Map;
typedef std::function<Map(int)> MapCreaterFunction;

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

/// классы

/// pirate
class Pirate {
private :
  bool gold_;
  Point coordinate_;
  size_t position_on_square_;
  bool dead_;

public :
  bool gold() {
    return gold;
  }
  Point coordinate() {
    return coordinate_;
  }
  size_t position_on_square() {
    return position_on_square_;
  }

}
/// ship



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

