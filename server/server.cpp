#include <iostream>
#include <string>
#include <vector>
using std::string;
enum WallType {NO, INTERIOR, OUTSIDE, EXIT};
enum Direction {NORTH, EAST, SOUTH, WEST};


class Treasure {
public:
  int x, y;
  bool hold;
  // bool true_treasure
  Treasure(int x, int y):
    x(x), y(y),
    hold(false) { };
  ~Treasure() {}
};


class Player {
public:
  const string nickname;
  int x; // стоит ли добавлять классс координат?
  int y;
  int health;
  int bullets;
  int bombs;
  bool has_treasure;
  //Treasure& treasure;
  Player(string nickname, int x, int y):
    nickname(nickname),
    x(x), y(y),
    health(2),
    bullets(3),
    bombs(3),
    has_treasure(false)
  {
    // сообщить текущую клетку
  }

};

class BasicSquare {
public:
  BasicSquare() {}
  virtual void effect(Player &player) { return; }
  virtual void move_effect(int& x, int& y) { return; };
  virtual ~BasicSquare(){}
};


class ExampleSquare: public BasicSquare {
public:
  ExampleSquare() {}
  virtual void effect(Player &player) {
    move_effect(player.x, player.y); // if not empty
    std::cout << player.nickname << " moved to (57, 57)" << std::endl; // or return something?
  }
  virtual void move_effect(int& x, int& y) { // to process treasure movements
    x = 57;
    y = 57;
  };
};

class Map {
private:
  std::vector<std::vector<BasicSquare*>> squares;
  std::vector<std::vector<bool>> walls_vertical;
  std::vector<std::vector<bool>> walls_horizontal;
  std::pair<Direction, int> exit;
public:
  const int X;
  const int Y;
  Map(const int X, const int Y):
    X(X), Y(Y),
    squares(X),
    walls_vertical(X - 1),
    walls_horizontal(X),
    exit(NORTH, 0) {
      for (std::vector<BasicSquare*>& column: (this->squares)){
        column.resize(Y);
        for (auto& square: column) {
          square = new(BasicSquare);
        }
      }
      for (auto& column: (this->walls_vertical)){
        column.resize(Y);
        for (auto wall: column) {
          wall = false;
        }
      }
      for (auto& column: (this->walls_horizontal)){
        column.resize(Y - 1);
        for (auto wall: column) {
          wall = false;
        }
      }
    }

  ~Map() {};
  BasicSquare* get_square(int x, int y) {
    return (this->squares)[x][y];
  }

  WallType wall_type(int x, int y, Direction dir) {
    return NO;
  }

  bool bomb(int x, int y, Direction dir) {
    if (this->wall_type(x, y, dir) != OUTSIDE) {
      // remove wall
      return true;
    }
    return false;
  }
};





int main() {
  Player player1("player1", 1, 1);
  ExampleSquare square1;
  Map map(12, 12);
  //map.squares[3][5] = &square1;
  //map.squares[3][5]->effect(player1);
  //map.squares[2][1] = new(BasicSquare);
  //map.squares[2][1]->effect(player1);
  //map.get_square(3, 5) = new(ExampleSquare);
  map.get_square(3, 5) -> effect(player1);
  std::cout << player1.x << player1.y;
 // std::cout << map.walls_horisontal[8][7];

  return 0;

}
