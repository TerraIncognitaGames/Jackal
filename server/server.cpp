#include <iostream>
#include <string>
using std::string;


class Player {
public:
  const string nickname;
  int x; // ����� �� ��������� ������ ���������?
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
    // �������� ������� ������
  }

};



int main() {
  return 0;
}
