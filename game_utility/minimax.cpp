#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>

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

  minimax() {
    int player, n, time_left;
    cin >> player >> n >> time_left;
    this->player = player - 1;
    this->n = n;
    this->time_left = time_left;
    game.initialize_game(this->n);
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
          double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, true);
        } else {
          string oponent_move;
          cin >> oponent_move;
          std::cerr << "Oponent Move : " << oponent_move << std::endl;
          //cerr << "Executing Move" << endl;
          state = this->game.execute_move(this->player, oponent_move, state, &players);
          double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, true);
        }
      } else {
        string oponent_move;
        cin >> oponent_move;
        std::cerr << "Oponent Move : " << oponent_move << std::endl;
        //cerr << "Executing Move" << endl;
        state = this->game.execute_move(this->player, oponent_move, state, &players);
        double value = maxval(state, players, alpha_initial, beta_initial, depth_initial, false);
      }
      moves++;
    }
  }

  double evaluation_function(vector<vector<pair<int, char>>> state) {
    return features::diff_flats(state, this->player);
  }

  double minval(vector<vector<pair<int, char>>> state, vector<Player> players, double alpha, double beta, int depth,
                bool first) {
    if (depth == 0) {
      //cerr << "Reached evaluation function " << endl;
      double evaluation = evaluation_function(state);
      //cerr << "Evaluation : " << evaluation << endl;
      return evaluation;
    }
    double best_child = std::numeric_limits<double>::infinity();
    if (first) {
      vector<string> all_moves = this->game.generate_all_moves(this->player, true, state, players);
      for (auto iter = all_moves.begin(); iter != all_moves.end(); iter++) {
        vector<Player> current_players = players;
        auto current_state = this->game.execute_move(this->player, *iter, state, &current_players);
        double child = maxval(current_state, current_players, alpha, beta, depth - 1, false);
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
        double child = maxval(current_state, current_players, alpha, beta, depth - 1, false);
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
                bool first) {
    if (depth == 0) {
      //cerr << "Reached evaluation function " << endl;
      double evaluation = evaluation_function(state);
      //cerr << "Evaluation : " << evaluation << endl;
      return evaluation;
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
          child = minval(current_state, current_players, alpha, beta, depth - 1, true);
        } else {
          child = minval(current_state, current_players, alpha, beta, depth - 1, false);
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
        double child = minval(current_state, current_players, alpha, beta, depth - 1, false);
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
