#ifndef STRATEGY_H
#define STRATEGY_H

#include "field.h"

class strategy {
public:
  strategy(field &field): field_(field) { }
  int recommend_move(int recursion) {
    return recommend_move_rec(recursion, field_.current_step_number() & 1).first;
  }

private:
  std::pair<int, int> recommend_move_rec(int recursion, int begin_from);
  int heuristic(int begin_from);
  field &field_;

};

#endif