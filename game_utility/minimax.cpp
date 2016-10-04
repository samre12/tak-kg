#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include <math.h>

#define depth_initial 4

#include "Game.h"
#include "Game.cpp"

class features{
public:
  static int diff_flats(vector<vector<pair<int, char>>> state, int player) {
    int counter = 0;
    for (auto iter = state.begin(); iter != state.end(); iter++) {
      if ((*iter).size() > 0) {
        if ((*iter).back().second == 'F') {
          if ((*iter).back().first == player) {
            counter++;
          } else {
            counter--;
          }
        }
      }
    }
    return counter;
  }
};

class minimax{
public:
  int player;
  int n;
  int time_left;
  std::vector<vector<pair<int, char>>> state;
  std::vector<Player> players;
  Game game;
  vector<int> x;
  vector<int> y;
  vector<bool> has_left;
  vector<bool> has_right;
  vector<bool> has_down;
  vector<bool> has_up;
  int min_x, max_x, min_y, max_y;

  minimax() {
    int player, n, time_left;
    cin >> player >> n >> time_left;
    this->player = player - 1;
    this->n = n;
    this->time_left = time_left;
    game.initialize_game(this->n);
    this->initialize_flood_fill();
    this->play();
  }

  void play() {
    double alpha_initial = -1 * std::numeric_limits<double>::infinity();
    double beta_initial = std::numeric_limits<double>::infinity();
    int moves = 0;

    for (int i = 0; i < this->game.total_squares; i++) {
      vector<pair<int, char>> initial;
      state.push_back(initial);
    }
    players.resize(2);
    players[0].flats = 21;
    players[0].capstones = 1;

    players[1].flats = 21;
    players[1].capstones = 1;
    while (true) {
      if (moves == 0) {
        if (this->player == 0) {
          double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        } else {
          string oponent_move;
          cin >> oponent_move;
          std::cerr << "Oponent Move : " << oponent_move << std::endl;
          //cerr << "Executing Move" << endl;
          state = this->game.execute_move(this->player, oponent_move, state, &players);
          moves++;
          double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        }
      } else {
        string oponent_move;
        cin >> oponent_move;
        std::cerr << "Oponent Move : " << oponent_move << std::endl;
        //cerr << "Executing Move" << endl;
        state = this->game.execute_move(this->player, oponent_move, state, &players);
        moves++;
        double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, false, moves);
      }
      moves++;
    }
  }

  void initialize_flood_fill() {
    x.resize(n * n);
    y.resize(n * n);
    has_left.resize(n * n);
    has_right.resize(n * n);
    has_up.resize(n * n);
    has_down.resize(n * n);

    for(int i = 0; i < n; i++)
  	{
  		for(int j = 0; j < n; j++)
  		{
  			x[i + n*(j-1)] = i+1;
  			y[i + n*(j-1)] = j+1;

  			if (i > 1){
  				has_left[i+n*(j-1)] = true;
  			}
  			if( i == 1){
  				has_left[i+n*(j-1)] = false;
  			}
  			if (i < n){
  				has_right[i+n*(j-1)] = true;
  			}
  			if( i == n){
  				has_left[i+n*(j-1)] = false;
  			}
        if (j > 1){
  				has_down[i+n*(j-1)] = true;
  			}
  			if( j == 1){
  				has_down[i+n*(j-1)] = false;
  			}
  			if (j < n){
  				has_up[i+n*(j-1)] = true;
  			}
  			if( j == n){
  				has_up[i+n*(j-1)] = false;
  			}
  		}
  	}
  }

  int flood_fill(int j, int player, vector<vector<pair<int, char>>> state, vector<bool>* explored)
  {
  	int sum;
  	if (!(*explored)[j] && (state[j].back().first == player) && (state[j].back().second == 'F' || state[j].back().second == 'C'))
  	{
  		(*explored)[j] = true;
  		sum = sum + 1;

  		min_x = std::min(x[j],min_x);
  		max_x = std::max(x[j],max_x);
  		min_y = std::min(y[j],min_y);
  		max_y = std::max(y[j],max_y);

  		if (has_left[j] && !(*explored)[j - 1]){
  			sum += flood_fill(j - 1, player, state, explored);
  		}

  		if (has_right[j] && !(*explored)[j + 1]){
  			sum += flood_fill(j + 1, player, state, explored);
  		}

  		if (has_down[j] && !(*explored)[j - n]){
  			sum += flood_fill(j - n, player, state, explored);
  		}

  		if (has_up[j] && !(*explored)[j + n]){
  			sum += flood_fill(j + n, player, state, explored);
  		}
  	}
    return sum;
  }


  pair<bool, double> evaluation_function(vector<vector<pair<int, char>>> state, vector<Player> players, int moves) {
    int size = n * n;
    int dim1 = 0;
  	int dim2 = 0;
  	int dimsum1 = 0;
  	int dimsum2 = 0;
  	vector<int> p1_isles;
  	vector<int> p2_isles;
    vector<bool> explored(n * n);
  	double strength_player1 = 0;
    double strength_player2 = 0;
    bool p1_rw, p2_rw;

  	for (int i = 0; i < size; i++)
  	{
  		if (!exploredc[i])
  		{
  			if ((state[i].back().first == player) && (state[i].back().second == 'F' || state[i].back().second == 'C'))
  			{
  				min_x = x[i];
  				max_x = x[i];
  				min_y = y[i];
  				max_y = y[i];
          int sum = flood_fill(i, this->player, state, &explored);
  				p1_isles.push_back(sum);
  				strength_player1 = strength_player1 + pow(sum, 1.1);
  				dim1 = std::max(max_y - min_y, dim1);
  				dim1 = std::max(max_x - min_x, dim1);
  				dimsum1 = dimsum1 + dim1;
  			}
  			if ((state[i].back().first == (1 - player)) && (state[i].back().second == 'F' || state[i].back().second == 'C'))
  			{
  				min_x = x[i];
  				max_x = x[i];
  				min_y = y[i];
  				max_y = y[i];
  				int sum = flood_fill(i, 1 - this->player, state, &explored);
  				p2_isles.push_back(sum);
  				strength_player2 = strength_player2 + pow(sum, 1.1);
  				dim2 = std::max(max_y - min_y, dim2);
  				dim2 = std::max(max_x - min_x, dim2);
  				dimsum2 = dimsum2 + dim2;
  			}
  		}
  	}
  	double eval1 = strength_player1 + 3*players[this->player].flats + 6*players[this->player].capstones - 8*players[this->player].capstones;
  	double eval2 = strength_player2 + 3*players[1 - this->player].flats + 6*players[this->player].capstones - 8*players[1 - this->player].capstones;

  	if(dim1 == n - 1)
  	{
  		p1_rw = true;
  		eval1 = 400 - moves;
  		eval2 = 0;
  	}
  	if(dim2 == n - 1)
  	{
  		p2_rw = true;
  		eval2 = 400 - moves;
  		eval1 = 0;
  	}
    //need to check flat wins and draw positions
    if (p1_rw || p2_rw) {
      return make_pair(true, eval1 - eval2);
    } else {
      return make_pair(false, eval1 - eval2);
    }

  }

  double minval(vector<vector<pair<int, char>>> state, vector<Player> players, double alpha, double beta, int depth,
                bool first, int moves) {
    auto result = evaluation_function(state, players, moves);
    cerr << "Evaluation function computed" << endl;
    if (result.first || depth == 0) {
      return result.second;
    }
    double best_child = std::numeric_limits<double>::infinity();
    if (first) {
      vector<string> all_moves = this->game.generate_all_moves(this->player, true, state, players);
      for (auto iter = all_moves.begin(); iter != all_moves.end(); iter++) {
        vector<Player> current_players = players;
        auto current_state = this->game.execute_move(this->player, *iter, state, &current_players);
        double child = maxval(current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
        beta = min(beta, child);
        if (alpha >= beta) {
          return child;
        }
        best_child = min(best_child, child);
      }

    } else {
      vector<string> all_moves = this->game.generate_all_moves(1 - this->player, false, state, players);
      for (auto iter = all_moves.begin(); iter != all_moves.end(); iter++) {
        vector<Player> current_players = players;
        auto current_state = this->game.execute_move(1 - this->player, *iter, state, &current_players);
        double child = maxval(current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
        beta = min(beta, child);
        if (alpha >= beta) {
          return child;
        }
        best_child = min(best_child, child);
      }
    }
    return best_child;
  }

  double maxval(vector<vector<pair<int, char>>> state, vector<Player> players, double alpha, double beta, int depth,
                bool first, int moves) {
    auto result = evaluation_function(state, players, moves);
    cerr << "Evaluation function computed" << endl;
    if (result.first || depth == 0) {
      return result.second;
    }
    double best_child = -1 * std::numeric_limits<double>::infinity();
    string best_move;
    if (first) {
      vector<string> all_moves = this->game.generate_all_moves(1 - this->player, true, state, players);
      for (auto iter = all_moves.begin(); iter != all_moves.end(); iter++) {
        vector<Player> current_players = players;
        auto current_state = this->game.execute_move(1 - this->player, *iter, state, &current_players);
        double child;
        if (this->player == 0) {
          child = minval(current_state, current_players, alpha, beta, depth - 1, true, moves + 1);
        } else {
          child = minval(current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
        }
        alpha = max(alpha, child);
        if (alpha >= beta) {
          if (depth == depth_initial) {
            this->execute_optimal_move(*iter);
          }
          return child;
        }
        if (best_child < child) {
          best_child = child;
          best_move = *iter;
        }
      }
    } else {
      vector<string> all_moves = game.generate_all_moves(this->player, false, state, players);
      for (auto iter = all_moves.begin(); iter != all_moves.end(); iter++) {
        vector<Player> current_players = players;
        auto current_state = this->game.execute_move(this->player, *iter, state, &current_players);
        double child = minval(current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
        alpha = max(alpha, child);
        if (alpha >= beta) {
          if (depth == depth_initial) {
            this->execute_optimal_move(*iter);
          }
          return child;
        }
        if (best_child < child) {
          best_child = child;
          best_move = *iter;
        }
      }
    }
    if (depth == depth_initial) {
      this->execute_optimal_move(best_move);
    }
    return best_child;

  }

  void execute_optimal_move(string move) {
    //output optimal strategy and execute on the current state
    cerr << "Chosen Move : " << move << endl;
    //cerr << "Executing Move" << endl;
    this->state = this->game.execute_move(this->player, move, this->state, &(this->players));
    move = move + '\n';
    cout << move;
  }
};

int main() {
  static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");
  minimax player;
  return 0;
}
