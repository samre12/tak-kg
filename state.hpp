#ifndef state_h
#define state_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <ctype.h>
#include <cmath>

#include "variables.hpp"
#include "utility_functions.h"

struct Player{
  int flats;
  int capstones;
};

class state {
public:
  std::vector<vector<pair<int, char>>> board;
  int min_x, max_x, min_y, max_y;

  state() {};

  void initialize_state();

  bool check_valid(int, char, vector<int>);

  vector<string> generate_stack_moves(int square);

  vector<string> generate_all_moves(int, bool, vector<Player>);

  void execute_move(int, string, vector<Player> *);

  bool are_remaining_pieces(vector<Player> players);

  double position_strength(int i);

  int diff_flats(int player);

  int flood_fill(int j, int player, vector<bool>* explored);

  vector<int> surrounding_influencer(int s, int i, int player);

  vector<int> rough_influence_measure(int i, int player);

  pair<double, double> evaluate_stack_strength(int i);

  pair<bool, double> evaluation_function1(vector<Player> players, int moves);

  // pair<bool, double> evaluation_function2(vector<Player> players, int moves);
};

#endif
