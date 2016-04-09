#include<iostream>
#include "game.h"


int main() {
  SquareBase* s=new(SquareBase);
  std::cout << (new SquareBase)->type();
  return 0;
}
