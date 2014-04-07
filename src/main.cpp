#include <iostream>
#include <string>
#include "field.h"

int main() {
  field field;
  std::string input;
  for (; ;) {
    std::cin >> input;
    if (input == "Go") {
      field.make_move(3);
      std::cout << 3 << std::endl;
      continue;
    }
    int his_move = atoi(input.c_str());
    field.make_move(his_move);
    int my_move = field.recommend_move(3).first;
    field.make_move(my_move);
    std::cout << my_move << std::endl;
  }
}