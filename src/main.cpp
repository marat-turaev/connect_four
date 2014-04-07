
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
  static const int SIZE = WIDTH * HEIGHT;
  static const int H1 = HEIGHT + 1;
  static const int H2 = HEIGHT + 2;
  static const int SIZE1 = H1 * WIDTH;
  static const int64_t BOTTOM = ((1LL << SIZE1) - 1LL) / ((1 << H1) - 1);
  static const int64_t TOP = BOTTOM << HEIGHT;

  int64_t color[2] {};
  int moves[SIZE] {};
  int step_number = 0;
  char height[WIDTH] {};

  bool has_four(int64_t board) {
    return has_combination(board, 1);
  }

  bool has_three(int64_t board) {
    return has_combination(board, 0);
  }

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
    reset();
  }

  void reset() {
    step_number = 0;
    color[0] = color[1] = 0ll;
    for (int i = 0; i < WIDTH; i++) {
      height[i] = (char) (H1 * i);
    }
  }

  int64_t positioncode() {
    return 2 * color[0] + color[1] + BOTTOM;
    // color[0] + color[1] + BOTTOM forms bitmap of heights
    // so that positioncode() is a complete board encoding
  }

  int64_t white_position() {
    return color[0];
  }

  int64_t black_position() {
    return color[1];
  }

  std::string toString() {
    std::string buf;
    for (int i = 0; i < step_number; i++) {
      buf.push_back(1 + moves[i]);
    }
    buf.push_back('\n');
    for (int w = 0; w < WIDTH; w++) {
      buf.push_back(' ');
      buf.append(std::to_string(w + 1));
    }
    buf.push_back('\n');
    for (int h = HEIGHT - 1; h >= 0; h--) {
      for (int w = h; w < SIZE1; w += H1) {
        long mask = 1LL << w;
        buf.append((color[0] & mask) != 0 ? " @" : (color[1] & mask) != 0 ? " 0" : " .");
      }
      buf.push_back('\n');
    }
    if (has_four(color[0])) {
      buf.append("@ won\n");
    }
    if (has_four(color[1])) {
      buf.append("O won\n");
    }
    if (has_three(color[0])) {
      buf.append("@ has three\n");
    }
    if (has_three(color[1])) {
      buf.append("O has three\n");
    }
    return buf;
  }

  bool isplayable(int col) {
    return islegal(color[step_number & 1] | (1LL << height[col]));
  }

  // return whether newboard lacks overflowing column
  bool islegal(int64_t newboard) {
    return (newboard & TOP) == 0;
  }

  // return whether newboard is legal and includes a win
  bool islegalhaswon(int64_t newboard) {
    return islegal(newboard) && has_four(newboard);
  }

  bool is_draw() {
    int64_t top_minus_one = BOTTOM << (HEIGHT - 1);
    return ((color[1] | color[0]) & top_minus_one) == top_minus_one;
  }

  bool first_won() {
    return has_four(color[0]);
  }

  bool second_won() {
    return has_four(color[1]);
  }

  bool first_has_three() {
    return has_three(color[0]);
  }

  bool second_has_three() {
    return has_three(color[1]);
  }

  void backmove() {
    int n;
    n = moves[--step_number];
    color[step_number & 1] ^= 1LL << --height[n];
  }

  void makemove(int n) {
    color[step_number & 1] ^= 1LL << height[n]++;
    moves[step_number++] = n;
  }

  std::pair<int, int> recommend_move(int recursion) {
    int turn = step_number & 1; // 0 -- first, 1 -- second
    int max_function;
    int max_move = 0;

    if (turn == 0) {
      max_function = -100;
    } else {
      max_function = 100;
    }

    if (recursion != 0) {
      for (int i = 0; i < WIDTH; ++i) {
        if (!isplayable(i)) {
          continue;
        }
        makemove(i);
        int function = recommend_move(recursion - 1).second;
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
        if (first_won()) {
          function += constants::I_WON;
        }
        if (second_won()) {
          function += constants::HE_WON;
        }
        if (first_has_three()) {
          function += constants::I_HAVE_THREE;
        }
        if (second_has_three()) {
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
  c4.reset();

  for (; ; ) {
    std::cout << c4.toString() << std::endl;
    std::cout << "I recommend this: " << c4.recommend_move(6).first << std::endl;
    int step;
    std::cin >> step;
    if (step == -1) {
      c4.backmove();
      continue;
    }
    c4.makemove(step);
  }
}