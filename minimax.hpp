#ifndef minimax_h
#define minimax_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <math.h>
#include <sys/time.h>
#include <random>
#include <unordered_map>

#include "variables.hpp"
#include "utility_functions.h"
#include "state.hpp"

struct move_ordering {
  string move;
  double evaluation;

  move_ordering(string move_string, double eval) : move(move_string), evaluation(eval) {}
};

struct lesser_than_key {
  inline bool operator() (const move_ordering& move1, const move_ordering& move2)
    {
        return (move1.evaluation < move2.evaluation);
    }
};

struct greater_than_key {
  inline bool operator() (const move_ordering& move1, const move_ordering& move2)
    {
        return (move1.evaluation > move2.evaluation);
    }
};

struct state_player {
  state game_state;
  vector<Player> players;
};


class minimax {
public:
  struct timeval start;
  struct timeval finish;
  double time_limit;
  double time_remaining;
  int depth_initial;
  state game_state;
  std::vector<Player> players;

  minimax(int time_input);
  void play();
  int depth(int moves);
  double minval(state* new_state, vector<Player> players, double alpha, double beta, int depth,
                bool first, int moves);
  double maxval(state* new_state, vector<Player> players, double alpha, double beta, int depth,
                bool first, int moves);
  void execute_optimal_move(string move, bool first);
};

#endif
