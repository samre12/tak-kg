#include "minimax.hpp"

minimax::minimax(int time_input) {
  this->time_limit = time_input;
  this->time_remaining = time_input;
  game_state.initialize_state();
  this->play();
}

void minimax::play() {
  double alpha_initial = -1 * std::numeric_limits<double>::infinity();
  double beta_initial = std::numeric_limits<double>::infinity();
  int moves = 0;
  std::cerr << "Player : " << player << " n : " << n << '\n';
  players.resize(2);

  switch (n) {
    case 5:
      players[0].flats = 21;
      players[0].capstones = 1;

      players[1].flats = 21;
      players[1].capstones = 1;
      break;

    case 6:
      players[0].flats = 30;
      players[0].capstones = 1;

      players[1].flats = 30;
      players[1].capstones = 1;
      break;

    case 7:
      players[0].flats = 40;
      players[0].capstones = 1;

      players[1].flats = 21;
      players[1].capstones = 1;
      break;
  }

  while (true) {
    if (moves == 0) {
      if (player == 0) {
        gettimeofday(&start, NULL);
        depth_initial = depth(moves);
        double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        moves++;
      } else {
        string oponent_move;
        cin >> oponent_move;
        gettimeofday(&start, NULL);
        //std::cerr << "Oponent Move : " << oponent_move << std::endl;
        //std::cerr << "Time Remaining : " << time_remaining << std::endl;
        //cerr << "Executing Move" << endl;
        game_state.execute_move(player, oponent_move, &players);
        moves++;
        depth_initial = depth(moves);
        double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        moves++;
      }
    } else if (moves == 1) {
      string oponent_move;
      cin >> oponent_move;
      gettimeofday(&start, NULL);
      //std::cerr << "Oponent Move : " << oponent_move << std::endl;
      //std::cerr << "Time Remaining : " << time_remaining << std::endl;
      //cerr << "Executing Move" << endl;
      game_state.execute_move(player, oponent_move, &players);
      moves++;
      depth_initial = depth(moves);
      double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, false, moves);
      moves++;
    } else {
      string oponent_move;
      cin >> oponent_move;
      gettimeofday(&start, NULL);
      //std::cerr << "Oponent Move : " << oponent_move << std::endl;
      //std::cerr << "Time Remaining : " << time_remaining << std::endl;
      //cerr << "Executing Move" << endl;
      game_state.execute_move(1 - player, oponent_move, &players);
      moves++;
      //cerr << "Evaluation Function on state" << endl;
      //pair<bool, double> result = evaluation_function1(state, players, moves);
      depth_initial = depth(moves);
      double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, false, moves);
      moves++;
    }
  }
}

int minimax::depth(int moves) {
  int output;
  if (this->time_remaining < ((time_limit * 40) / 100) && this->time_remaining > ((time_limit * 5) / 100)) {
    output = 2;
  } else if (this->time_remaining < ((time_limit * 5) / 100)){
    output = 1;
  } else {
	  output = 3;
  }
  //cerr << "Current Depth : " << output << endl;
  std::cerr << "depth " << output << '\n';
  return output;
}

double minimax::minval(state* new_state, vector<Player> players, double alpha, double beta, int depth, bool first, int moves) {
  //cerr << "Reached Evaluation Function" << endl;
  pair<double, double> result = new_state->evaluation_function1(players, moves);
  //cerr << "Evaluation function computed" << endl;
  if (result.first || depth == 0) {
    return result.second;
  }
  double best_child = std::numeric_limits<double>::infinity();
  if (first) {
    vector<string> all_moves = new_state->generate_all_moves(player, true, players);
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state current_state = (*new_state);
      current_state.execute_move(player, *iter, &current_players);
      double child = maxval(&current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      beta = min(beta, child);
      if (alpha >= beta) {
        return child;
      }
      best_child = min(best_child, child);
    }

  } else {
    vector<string> all_moves = new_state->generate_all_moves(1 - player, false, players);
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state current_state = (*new_state);
      current_state.execute_move(1 - player, *iter, &current_players);
      double child = maxval(&current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      beta = min(beta, child);
      if (alpha >= beta) {
        return child;
      }
      best_child = min(best_child, child);
    }
  }
  return best_child;
}

//execute optimal move
double minimax::maxval(state* new_state, vector<Player> players, double alpha, double beta, int depth, bool first, int moves) {
  //cerr << "Reached Evaluation Function" << endl;
  pair<double, double> result = new_state->evaluation_function1(players, moves);
  //cerr << "Evaluation function computed" << endl;
  if (result.first || depth == 0) {
    return result.second;
  }
  double best_child = -1 * std::numeric_limits<double>::infinity();
  string best_move;
  if (first) {
    vector<string> all_moves = new_state->generate_all_moves(1 - player, true, players);

    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state current_state = (*new_state);
      current_state.execute_move(1 - player, *iter, &current_players);
      double child;
      if (player == 0) {
        child = minval(&current_state, current_players, alpha, beta, depth - 1, true, moves + 1);
      } else {
        child = minval(&current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      }
      alpha = max(alpha, child);
      if (alpha >= beta) {
        if (depth == depth_initial) {
          this->execute_optimal_move(*iter, first);
        }
        return child;
      }
      if (best_child < child) {
        best_child = child;
        best_move = *iter;
      }
    }
  } else {
    vector<string> all_moves = new_state->generate_all_moves(player, false, players);
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state current_state = (*new_state);
      current_state.execute_move(player, *iter, &current_players);
      double child = minval(&current_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      alpha = max(alpha, child);
      if (alpha >= beta) {
        if (depth == depth_initial) {
          this->execute_optimal_move(*iter, first);
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
    this->execute_optimal_move(best_move, first);
  }
  return best_child;
}

void minimax::execute_optimal_move(string move, bool first) {
  if (first) {
    game_state.execute_move(1 - player, move, &players);
  } else {
    game_state.execute_move(player, move, &players);
  }
  move = move + '\n';
  cout << move;
  //std::cerr << "my move : " << move << " move length : " << move.length() << '\n';
  gettimeofday(&finish, NULL);
  time_remaining = time_remaining - ((finish.tv_sec - start.tv_sec) + ((finish.tv_usec - start.tv_usec) / 1000000));
}
