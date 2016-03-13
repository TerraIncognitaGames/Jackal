#include <iostream>
#include <string>
using std::string;


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

class BasicCell {
public:
  Cell() {}
  virtual void effect(Player &player) {  }
  virtual void move_effect(int& x, int& y) {  };
};


class ExampleCell: public Cell {
public:
  ExampleCell() {}
  virtual void effect(Player &player) {
    //
    move_effect(player.x, player.y); // if not empty
    std::cout << player.nickname << " moved to (57, 57)" << std::endl;
  }
  virtual void move_effect(int& x, int& y) {
    x = 57;
    y = 57;
  };
};



int main() {
  Player player1("player1", 1, 1);
  ExampleCell cell1;
  cell1.effect(player1);
  std::cout << player1.x << player1.y;
  return 0;
}
