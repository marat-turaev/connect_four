#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <cstdint>

class field {
private:
  static const int WIDTH = 7;
  static const int HEIGHT = 6;
  static const int SIZE = WIDTH *HEIGHT;
  static const int H1 = HEIGHT + 1;
  static const int H2 = HEIGHT + 2;
  static const int SIZE1 = H1 *WIDTH;
  static const int64_t BOTTOM = ((1LL << SIZE1) - 1LL) / ((1 << H1) - 1);
  static const int64_t TOP = BOTTOM << HEIGHT;

  int64_t color[2] = {};
  int move[SIZE] = {};
  char height[WIDTH] = {};
  int step_number = 0;
  bool has_combination(int64_t board, int shift);
  bool is_playable(int col);
  bool is_valid(int64_t newboard);
  std::pair<int, int> recommend_move_rec(int recursion, int begin_from);

public:
  field();
  bool is_draw();
  bool has_won(int id);
  bool has_three(int id);
  void back_move();
  void make_move(int n);
  std::pair<int, int> recommend_move(int recursion);
};

#endif
