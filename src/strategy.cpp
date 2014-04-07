#include "strategy.h"
#include <climits>

std::pair<int, int> strategy::recommend_move_rec(int recursion, int begin_from) {
  int is_enemy_turn = (field_.current_step_number() & 1) ^ begin_from;

  if (field_.has_won(begin_from)) {
    return std::make_pair(-1, 10);
  }
  if (field_.has_won(begin_from ^ 1)) {
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
    for (int i = 0; i < field_.width(); ++i) {
      if (!field_.is_playable(i)) {
        continue;
      }
      field_.make_move(i);
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
      field_.back_move();
    }
  }

  if (recursion == 0) {
    for (int i = 0; i < field_.width(); ++i) {
      if (!field_.is_playable(i)) {
        continue;
      }
      field_.make_move(i);
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
      field_.back_move();
    }
  }

  return std::make_pair(optimal_move, optimal_function);
}

int strategy::heuristic(int begin_from) {
  int resutl = 0;
  if (field_.has_won(begin_from)) {
    resutl += 10;
  }
  if (field_.has_won(begin_from ^ 1)) {
    resutl += -10;
  }
  if (field_.has_three(begin_from)) {
    resutl += 5;
  }
  if (field_.has_three(begin_from ^ 1)) {
    resutl += -5;
  }
  if (field_.is_draw()) {
    resutl += 7;
  }
  return resutl;
}