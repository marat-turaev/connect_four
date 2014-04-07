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
