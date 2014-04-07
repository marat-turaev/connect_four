#include <climits>
#include "field.h"

bool field::has_combination(int64_t board, int shift) {
  int64_t y = board & (board >> HEIGHT);
  if ((y & (y >> (HEIGHT << shift))) != 0) {
    return true;
  }
  y = board & (board >> H1);
  if ((y & (y >> (H1 << shift))) != 0) {
    return true;
  }
  y = board & (board >> H2);
  if ((y & (y >> (H2 << shift))) != 0) {
    return true;
  }
  y = board & (board >> 1);
  return (y & (y >> (1 + shift))) != 0;
}

bool field::is_playable(int col) {
  return is_valid(color[step_number & 1] | (1LL << height[col]));
}

bool field::is_valid(int64_t newboard) {
  return (newboard & TOP) == 0;
}

std::pair<int, int> field::recommend_move_rec(int recursion, int begin_from) {
  int is_enemy_turn = (step_number & 1) ^ begin_from;

  if (has_won(begin_from)) {
    return std::make_pair(-1, 10);
  }
  if (has_won(begin_from ^ 1)) {
    return std::make_pair(-1, -10);
  }

  int optimal_function;
  int optimal_move = 0;

  if (is_enemy_turn == 0) {
    optimal_function = INT_MIN;
  } else {
    optimal_function = INT_MAX;
  }

  if (recursion != 0) {
    for (int i = 0; i < WIDTH; ++i) {
      if (!is_playable(i)) {
        continue;
      }
      make_move(i);
      int function = recommend_move_rec(recursion - 1, begin_from).second;
      if (is_enemy_turn == 0) {
        if (function > optimal_function) {
          optimal_function = function;
          optimal_move = i;
        }
      } else {
        if (function < optimal_function) {
          optimal_function = function;
          optimal_move = i;
        }
      }
      back_move();
    }
  }

  if (recursion == 0) {
    for (int i = 0; i < WIDTH; ++i) {
      if (!is_playable(i)) {
        continue;
      }
      make_move(i);
      int function = heuristic(begin_from);
      if (is_enemy_turn == 0) {
        if (function > optimal_function) {
          optimal_function = function;
          optimal_move = i;
        }
      } else {
        if (function < optimal_function) {
          optimal_function = function;
          optimal_move = i;
        }
      }
      back_move();
    }
  }

  return std::make_pair(optimal_move, optimal_function);
}

int field::heuristic(int begin_from) {
  int resutl = 0;
  if (has_won(begin_from)) {
    resutl += 10;
  }
  if (has_won(begin_from ^ 1)) {
    resutl += -10;
  }
  if (has_three(begin_from)) {
    resutl += 5;
  }
  if (has_three(begin_from ^ 1)) {
    resutl += -5;
  }
  if (is_draw()) {
    resutl += 7;
  }
  return resutl;
}

field::field() {
  for (int i = 0; i < WIDTH; i++) {
    height[i] = (char) (H1 * i);
  }
}

bool field::is_draw() {
  int64_t top_minus_one = BOTTOM << (HEIGHT - 1);
  return ((color[1] | color[0]) & top_minus_one) == top_minus_one;
}

bool field::has_won(int id) {
  return has_combination(color[id], 1);
}

bool field::has_three(int id) {
  return has_combination(color[id], 0);
}

void field::back_move() {
  int n = move[--step_number];
  color[step_number & 1] ^= 1LL << --height[n];
}

void field::make_move(int n) {
  color[step_number & 1] ^= 1LL << height[n]++;
  move[step_number++] = n;
}

std::pair<int, int> field::recommend_move(int recursion) {
  return recommend_move_rec(recursion, step_number & 1);
}
