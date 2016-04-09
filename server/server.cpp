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
enum SquareType { NORMAL, RIVER, PORTAL, ARSENAL, CLINIC };

/// классы :

/// pirate
class Pirate {
private :
  bool gold_;

public :
  bool gold() {
    return gold;
  }

}
/// ship

/// water
/// arrow
/// hoarse
/// ice
/// crocodile

/// baloon
/// gun

/// field
/// jungle
/// desert
/// bog
/// mountain

/// hole
/// rum
/// cannibal

/// fortress
/// aborigine

/// gold





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




class Player {
};

class Square {

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

class RequestQuery {};
// class ResponseQuery {};
class Action {};



int main() {

  return 0;
}
