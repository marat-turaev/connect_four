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

  int optimal_function = is_enemy_turn ? INT_MAX : INT_MIN;
  int optimal_move = 0;

  for (int i = 0; i < field_.width(); ++i) {
    if (!field_.is_playable(i)) {
      continue;
    }
    field_.make_move(i);
    int function = recursion == 0 ? heuristic(begin_from) : recommend_move_rec(recursion - 1, begin_from).second;
    if (is_enemy_turn) {
      if (function < optimal_function) {
        optimal_function = function;
        optimal_move = i;
      }
    } else {
      if (function > optimal_function) {
        optimal_function = function;
        optimal_move = i;
      }
    }
    field_.back_move();
  }

  return std::make_pair(optimal_move, optimal_function);
}

int strategy::heuristic(int begin_from) {
  int result = 0;
  if (field_.has_won(begin_from)) {
    result += 10;
  }
  if (field_.has_won(begin_from ^ 1)) {
    result += -10;
  }
  if (field_.has_three(begin_from)) {
    result += 5;
  }
  if (field_.has_three(begin_from ^ 1)) {
    result += -5;
  }
  if (field_.is_draw()) {
    result += 7;
  }
  return result;
}