#ifndef weights_h
#define weights_h

#include <vector>
#include <utility>

struct eval_weights {
  double eval_max;
  double eval_min;

  vector<double> flat_threshold;
  vector<double> flat_stones;
  double standing_stone;
  double cap_stone;

  vector<double> hard_flats;
  vector<double> soft_flats;

  vector<double> groups;

  vector<double> influence;

  eval_weights() {
    eval_max = 100000;
    eval_min = -1 * eval_max;
    flat_threshold = {0, 0, 0, 5, 8, 10, 15, 20, 25};
    flat_stones = {400, 800};
    standing_stone = 200;
    cap_stone = 300;

    hard_flats = {125, 125, 150};
    soft_flats = {-75, -50, -25};

    influence = {20, 15, -5};

    switch (n) {
      case 5:
        groups = {0, 0, 100, 200, 400, 600};
        break;

      case 6:
        groups = {0, 0, 100, 200, 400, 600, 800};
        break;

      case 7:
        groups = {0, 0, 100, 200, 400, 600, 800, 0};
        break;
    }
  }

  // original weights
  // eval_weights() {
  //   eval_max = 100000;
  //   eval_min = -1 * eval_max;
  //   win_threshold = 990000;
  //
  //   flat_threshold = {0, 0, 0, 5, 8, 10, 15, 20, 25};
  //   flat_stones = {400, 800};
  //   standing_stone = 200;
  //   cap_stone = 300;
  //
  //   hard_flats = {125, 125, 150};
  //   soft_flats = {-75, -50, -25};
  //
  //   threat = 200;
  //   groups = {0, 0, 100, 200, 400, 600, 0, 0};
  //   influence = {20, 15, -5};
  //
  // }

};

#endif
