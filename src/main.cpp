#include <iostream>
#include <string>
#include <cstdint>

class constants {
public:
  static const int DRAW = 7;
  static const int I_WON = 10;
  static const int HE_WON = -10;
  static const int I_HAVE_THREE = 5;
  static const int HE_HAS_THREE = -5;
  static const int NONE = 0;
};

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

  bool has_combination(int64_t board, int shift) {
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

public:
  field() {
    step_number = 0;
    color[0] = color[1] = 0LL;
    for (int i = 0; i < WIDTH; i++) {
      height[i] = (char) (H1 * i);
    }
  }


  std::string to_string() {
    std::string buf;
    for (int i = 0; i < step_number; i++) {
      buf.append(std::to_string(move[i]));
    }
    buf.push_back('\n');
    for (int w = 0; w < WIDTH; w++) {
      buf.append(" " + std::to_string(w));
    }
    buf.push_back('\n');
    for (int h = HEIGHT - 1; h >= 0; h--) {
      for (int w = h; w < SIZE1; w += H1) {
        long mask = 1LL << w;
        buf.append((color[0] & mask) != 0 ? " 0" : (color[1] & mask) != 0 ? " 1" : " .");
      }
      buf.push_back('\n');
    }
    if (has_won(0)) {
      buf.append("0 won\n");
    }
    if (has_won(1)) {
      buf.append("1 won\n");
    }
    if (has_three(0)) {
      buf.append("0 has three\n");
    }
    if (has_three(1)) {
      buf.append("1 has three\n");
    }
    return buf;
  }

  bool isplayable(int col) {
    return islegal(color[step_number & 1] | (1LL << height[col]));
  }

  bool islegal(int64_t newboard) {
    return (newboard & TOP) == 0;
  }

  bool is_draw() {
    int64_t top_minus_one = BOTTOM << (HEIGHT - 1);
    return ((color[1] | color[0]) & top_minus_one) == top_minus_one;
  }

  bool has_won(int id) {
    return has_combination(color[id], 1);
  }

  bool has_three(int id) {
    return has_combination(color[id], 0);
  }

  void backmove() {
    int n = move[--step_number];
    color[step_number & 1] ^= 1LL << --height[n];
  }

  void makemove(int n) {
    color[step_number & 1] ^= 1LL << height[n]++;
    move[step_number++] = n;
  }

  std::pair<int, int> recommend_move(int recursion) {
    return recommend_move_rec(recursion, step_number & 1);
  }

  std::pair<int, int> recommend_move_rec(int recursion, int begin_from) {
    int turn = (step_number & 1) ^ begin_from;

    if (has_won(begin_from)) {
      return std::make_pair(-1, 10);
    }
    if (has_won(begin_from ^ 1)) {
      return std::make_pair(-1, -10);
    }

    int max_function;
    int max_move = 0;

    if (turn == 0) {
      max_function = INT_MIN;
    } else {
      max_function = INT_MAX;
    }

    if (recursion != 0) {
      for (int i = 0; i < WIDTH; ++i) {
        if (!isplayable(i)) { // ?
          continue;
        }
        makemove(i);
        int function = recommend_move_rec(recursion - 1, begin_from).second;
        if (turn == 0) {
          if (function > max_function) {
            max_function = function;
            max_move = i;
          }
        } else {
          if (function < max_function) {
            max_function = function;
            max_move = i;
          }
        }
        backmove();
      }
    }

    if (recursion == 0) {
      for (int i = 0; i < WIDTH; ++i) {
        if (!isplayable(i)) {
          continue;
        }
        makemove(i);
        int function = constants::NONE;
        if (has_won(begin_from)) {
          function += constants::I_WON;
        }
        if (has_won(begin_from ^ 1)) {
          function += constants::HE_WON;
        }
        if (has_three(begin_from)) {
          function += constants::I_HAVE_THREE;
        }
        if (has_three(begin_from ^ 1)) {
          function += constants::HE_HAS_THREE;
        }
        if (is_draw()) {
          function += constants::DRAW;
        }
        if (turn == 0) {
          if (function > max_function) {
            max_function = function;
            max_move = i;
          }
        } else {
          if (function < max_function) {
            max_function = function;
            max_move = i;
          }
        }
        backmove();
      }
    }
    return std::make_pair(max_move, max_function);
  }
};


int main() {
  field c4;

  // for (; ; ) {
  //   std::cout << c4.to_string() << std::endl;
  //   std::cout << "I recommend this: " << c4.recommend_move(1).first << std::endl;
  //   int step;
  //   std::cin >> step;
  //   if (step == -1) {
  //     c4.backmove();
  //     continue;
  //   }
  //   c4.makemove(step);
  // }

  std::string input;
  for (; ;) {
    std::cin >> input;
    if (input == "Go") {
      c4.makemove(3);
      std::cout << 3;
      continue;
    }
    int his_move = atoi(input.c_str());
    c4.makemove(his_move);
    int my_move = c4.recommend_move(1).first;
    std::cout << my_move;
  }
}