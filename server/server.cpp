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

enum Direction { TOP, BOTTOM, RIGHT, LEFT };
enum SquareType { NORMAL, RIVER, PORTAL, ARSENAL, CLINIC };

Direction GetOppositeDirection(Direction dir) {
  switch (dir) {
  case TOP:
    return BOTTOM;
  case BOTTOM:
    return TOP;
  case RIGHT:
    return LEFT;
  case LEFT:
    return RIGHT;
  }
}

struct Point {
public:
  int x, y;
  Point(int x_coord = 0, int y_coord = 0)
    : x(x_coord), y(y_coord) {}
  bool operator== (const Point &other) const {
    return ((other.x == x) && (other.y == y));
  }
};

Point GetRandomPoint(int size) {
  // most likely, it's a temp function. now it's used for setting coords to Players.
  srand(time(NULL));
  int x = (rand() % size);
  int y = (rand() % size);
  return Point(x, y);
}

Point GetPointToThe(Point p, Direction dir) {
  switch (dir) {
  case TOP:
    return Point(p.x, p.y + 1);
  case BOTTOM:
    return Point(p.x, p.y - 1);
  case RIGHT:
    return Point(p.x + 1, p.y);
  case LEFT:
    return Point(p.x - 1, p.y);
  }
}

bool IsCorrectPoint(Point p, int size) {
  return (p.x >= 0 && p.x < size && p.y >= 0 && p.y < size);
}

class Player {
public:
  Point coord;
  const string nickname;
  int health, bullets, bombs;
  bool has_treasure;
  Player(Point coord, string nickname)
    : nickname(nickname),
    coord(coord),
    health(2),
    bullets(3),
    bombs(3),
    has_treasure(false) {}
  bool is_alive() {
    return health > 0;
  }
  bool operator==(const Player &other) const {
    return other.nickname == nickname;
  }
  bool operator!=(const Player &other) const {
    return other.nickname != nickname;
  }
};

class Square {
public:
  map<Direction, bool> has_wall_to_the;
  bool has_treasure;
  Point next; // stores next square position for river and portal squares.
  SquareType type;
  Square(SquareType type = NORMAL, Point next = Point(), bool has_treasure = false)
    : has_treasure(has_treasure),
    next(next),
    type(type) {
    for (auto it = has_wall_to_the.begin(); it != has_wall_to_the.end(); ++it) {
      it->second = false;
    }
  }
  bool &operator[](Direction dir) {
    return has_wall_to_the[dir];
  }
};

typedef vector<vector<Square>> Map;
typedef std::function<Map(int)> MapCreaterFunction;

class Field {
public:
  Field(int size = 6) {
    // creates empty field.
    for (int i = 0; i < size; ++i) {
      field_.push_back(vector<Square>(size, Square()));
    }
  }
  Field(MapCreaterFunction mcf, int size = 6) {
    field_ = mcf(size);
  }
  Square& operator[](Point p) {
    return field_[p.x][p.y];
  }
  Square& operator[](const Player &p) {
    return field_[p.coord.x][p.coord.y];
  }
  void Bash(Point coord, Direction direction) {
    int size = field_.size();
    (*this)[coord][direction] = false;
    Point adjacent_point = GetPointToThe(coord, direction);
    if (IsCorrectPoint(adjacent_point, field_.size())) {
      (*this)[coord][GetOppositeDirection(direction)] = false;
    }
  }
  int size() {  // name should be lowercase.
    return field_.size();
  }

private:
  Map field_;  // field_[0][0] is a Left Bottom corner.
};

class RequestQuery {};
class ResponseQuery {};
class Action {};

class GameServer {
public:
  GameServer(int number_of_players = 2, int size = 6) {
    Initialize(number_of_players, size);
  }
  void Initialize(int number_of_players = 2, int size = 6) {
    field_ = Field(size);
    players_ = vector<Player>();
    for (int i = 0; i < number_of_players; ++i) {
      string player_name = "Player" + std::to_string(i);
      players_.push_back(Player(GetRandomPoint(size), player_name));
    }
    turn_ = 0;
  }
  ResponseQuery MakeRequest(RequestQuery query) {
    // there we should unpack request and do proper actions.
    return ResponseQuery();
  }

private:
  Field field_;
  vector<Player> players_;
  int turn_;
  void Shoot(int player_index, Direction direction) {

  }
  bool DestroyWall(int player_index, Direction direction) {
    Player &player = players_[player_index];
    if (player.bombs == 0) {
      return false;
    }
    Point p = player.coord;
    field_.Bash(p, direction);
    player.bombs -= 1;
    return true;
  }
  bool MovePlayer(int player_index, Direction direction) {
    Player& player = players_[player_index];
    Point player_coord = player.coord;
    if (field_[player_coord][direction]) {
      return false; // met wall
    } else {
      Point destination_point = GetPointToThe(player_coord, direction);
      if (IsCorrectPoint(destination_point, field_.size())) {
        player.coord = destination_point;
        if (field_[player].type == RIVER || field_[player].type == PORTAL) {
          player.coord = field_[player].next;
        }
      } else {
        // Player is about to escape. does he carry the treasure?
        if (player.has_treasure) {
          // Player player wins. IDK how to write it best yet. ==========================
        } else {
          // We have to stop him, right? ================================================
        }
      }
      return true;
    }
  }
  void AttackSelfSquare(int player_index) {
    Player &player = players_[player_index];
    for (auto& pl : players_) {
      if ((player.coord == pl.coord) && player != pl) {
        pl.health -= 1;
      }
    }
  }
};



int main() {

  return 0;
}
