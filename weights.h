#ifndef weights_h
#define weights_h

#include <vector>
#include <utility>

struct weights {
  double eval_max;
  double eval_min;
  double win_threshold;

  vector<double> flat_threshold;
  vector<double> flatstone;
  double standing_stone;
  double capstone;

  vector<double> hard_flats;
  vector<double> soft_flats;

  vector<double> groups;

  double threat;

  weights() {
    eval_max = 100000;
    eval_min = -1 * eval_max;
    win_threshold = 990000;

    flat_threshold = {400, 800};
    standing_stone = 200;
    capstone = 300;

    hard_flats = {125, 125, 150};
    soft_flats = {-75, -50, -25};

    threat = 200;
    group = {0, 0, 100, 200, 400, 600, 0, 0};
  }

};

#endif
