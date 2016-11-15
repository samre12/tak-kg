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

      players[1].flats = 40;
      players[1].capstones = 1;
      break;
  }

  while (true) {
    if (moves == 0) {
      if (player == 0) {
        gettimeofday(&start, NULL);
        depth_initial = depth(moves);
        game_state.evaluation_function1(players, moves, depth_initial);
        states_explored = 0;
        double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        std::cerr << "Evaluation Function : " << game_state.evaluation << '\n';
        std::cerr <<  "Backed Up Value : " << value << '\n';
        // for (int i = n - 1; i >= 0; i--) {
        //   for (int j = 0; j < n; j++) {
        //     int k = n * i + j;
        //     std::cerr << game_state.stacks[k].captives << " " << game_state.stacks[k].reserves << " " << game_state.stacks[k].top << " " << game_state.stacks[k].controller << " " << game_state.board[k].size();
        //     std::cerr << "   ";
        //   }
        //   std::cerr << '\n';
        // }
        // std::cerr << "States Ended" << '\n';
        std::cerr << "States Explored : " << states_explored << '\n';

        // std::cerr << "Influence : " << '\n';
        // for (int i = 0; i < game_state.influence.size(); i++) {
        //   for (int j = 0; j < game_state.influence[i].size(); j++) {
        //     std::cerr << game_state.influence[i][j] << " ";
        //   }
        //   std::cerr << '\n';
        // }
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
        game_state.evaluation_function1(players, moves, depth_initial);
        states_explored = 0;
        double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, true, moves);
        std::cerr << "Evaluation Function : " << game_state.evaluation << '\n';
        std::cerr <<  "Backed Up Value : " << value << '\n';
        // for (int i = n - 1; i >= 0; i--) {
        //   for (int j = 0; j < n; j++) {
        //     int k = n * i + j;
        //     std::cerr << game_state.stacks[k].captives << " " << game_state.stacks[k].reserves << " " << game_state.stacks[k].top << " " << game_state.stacks[k].controller << " " << game_state.board[k].size();
        //     std::cerr << "   ";
        //   }
        //   std::cerr << '\n';
        // }
        // std::cerr << "States Ended" << '\n';
        std::cerr << "States Explored : " << states_explored << '\n';
        // std::cerr << "Influence : " << '\n';
        // for (int i = 0; i < game_state.influence.size(); i++) {
        //   for (int j = 0; j < game_state.influence[i].size(); j++) {
        //     std::cerr << game_state.influence[i][j] << " ";
        //   }
        //   std::cerr << '\n';
        // }
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
      game_state.evaluation_function1(players, moves, depth_initial);
      states_explored = 0;
      double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, false, moves);
      std::cerr << "Evaluation Function : " << game_state.evaluation << '\n';
      std::cerr <<  "Backed Up Value : " << value << '\n';
      // for (int i = n - 1; i >= 0; i--) {
      //   for (int j = 0; j < n; j++) {
      //     int k = n * i + j;
      //     std::cerr << game_state.stacks[k].captives << " " << game_state.stacks[k].reserves << " " << game_state.stacks[k].top << " " << game_state.stacks[k].controller << " " << game_state.board[k].size();
      //     std::cerr << "   ";
      //   }
      //   std::cerr << '\n';
      // }
      // std::cerr << "States Ended" << '\n';
      std::cerr << "States Explored : " << states_explored << '\n';
      // std::cerr << "Influence : " << '\n';
      // for (int i = 0; i < game_state.influence.size(); i++) {
      //   for (int j = 0; j < game_state.influence[i].size(); j++) {
      //     std::cerr << game_state.influence[i][j] << " ";
      //   }
      //   std::cerr << '\n';
      // }
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
      game_state.evaluation_function1(players, moves, depth_initial);
      states_explored = 0;
      double value = maxval(&game_state, players, alpha_initial, beta_initial, depth_initial, false, moves);
      std::cerr << "Evaluation Function : " << game_state.evaluation << '\n';
      std::cerr <<  "Backed Up Value : " << value << '\n';
      // for (int i = n - 1; i >= 0; i--) {
      //   for (int j = 0; j < n; j++) {
      //     int k = n * i + j;
      //     std::cerr << game_state.stacks[k].captives << " " << game_state.stacks[k].reserves << " " << game_state.stacks[k].top << " " << game_state.stacks[k].controller << " " << game_state.board[k].size();
      //     std::cerr << "   ";
      //   }
      //   std::cerr << '\n';
      // }
      // std::cerr << "States Ended" << '\n';
      std::cerr << "States Explored : " << states_explored << '\n';
      // std::cerr << "Influence : " << '\n';
      // for (int i = 0; i < game_state.influence.size(); i++) {
      //   for (int j = 0; j < game_state.influence[i].size(); j++) {
      //     std::cerr << game_state.influence[i][j] << " ";
      //   }
      //   std::cerr << '\n';
      // }
      moves++;
    }
  }
}

int minimax::depth(int moves) {
  int output;
  if (n == 5) {
    if (this->time_remaining < ((time_limit * 40) / 100) && this->time_remaining > ((time_limit * 5) / 100)) {
      output = 3;
    } else if (this->time_remaining < ((time_limit * 5) / 100)){
      output = 2;
    } else {
  	  output = 4;
    }
  } else if (n == 6) {
    if (this->time_remaining < ((time_limit * 20) / 100) && this->time_remaining > ((time_limit * 5) / 100)) {
      output = 2;
    } else if (this->time_remaining < ((time_limit * 5) / 100)){
      output = 1;
    } else if (this->time_remaining < ((time_limit * 50) / 100)){
  	  output = 3;
    } else {
      output = 4;
    }
  } else {
    if (this->time_remaining < ((time_limit * 20) / 100) && this->time_remaining > ((time_limit * 5) / 100)) {
      output = 2;
    } else if (this->time_remaining < ((time_limit * 5) / 100)){
      output = 1;
    } else if (this->time_remaining < ((time_limit * 75) / 100)){
  	  output = 3;
    } else {
      output = 4;
    }
  }
  //cerr << "Current Depth : " << output << endl;
  std::cerr << "depth " << output << '\n';
  return output;
}

double minimax::minval(state* new_state, vector<Player> players, double alpha, double beta, int depth, bool first, int moves) {
  //cerr << "Reached Evaluation Function" << endl;
  //cerr << "Evaluation function computed" << endl;
  states_explored++;
  if (new_state->terminal || depth == 0) {
    return new_state->evaluation;
  }
  double best_child = std::numeric_limits<double>::infinity();
  if (first) {
    vector<string> all_moves = new_state->generate_all_moves(player, true, players);

    vector<move_ordering> new_moves;
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child = (*new_state);
      child.execute_move(player, *iter, &current_players);
      child.evaluation_function1(current_players, moves + 1, depth - 1);
      move_ordering move((*iter), child.evaluation, child.terminal);
      new_moves.push_back(move);
    }

    std::sort(new_moves.begin(), new_moves.end(), lesser_than_key());

    for (vector<move_ordering>::iterator iter = new_moves.begin(); iter != new_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child_state = (*new_state);
      child_state.execute_move(player, (*iter).move, &current_players);
      child_state.evaluation = iter->evaluation;
      child_state.terminal = iter->terminal;
      double child = maxval(&child_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      beta = min(beta, child);
      if (alpha >= beta) {
        return child;
      }
      best_child = min(best_child, child);
    }

  } else {
    vector<string> all_moves = new_state->generate_all_moves(1 - player, false, players);

    vector<move_ordering> new_moves;
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child = (*new_state);
      child.execute_move(1 - player, *iter, &current_players);
      child.evaluation_function1(current_players, moves + 1, depth - 1);
      move_ordering move((*iter), child.evaluation, child.terminal);
      new_moves.push_back(move);
    }

    std::sort(new_moves.begin(), new_moves.end(), lesser_than_key());

    for (vector<move_ordering>::iterator iter = new_moves.begin(); iter != new_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child_state = (*new_state);
      child_state.execute_move(1 - player, (*iter).move, &current_players);
      child_state.evaluation = iter->evaluation;
      child_state.terminal = iter->terminal;
      double child = maxval(&child_state, current_players, alpha, beta, depth - 1, false, moves + 1);
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
  //cerr << "Evaluation function computed" << endl;
  states_explored++;
  if (new_state->terminal || depth == 0) {
    return new_state->evaluation;
  }
  double best_child = -1 * std::numeric_limits<double>::infinity();
  string best_move;
  if (first) {
    vector<string> all_moves = new_state->generate_all_moves(1 - player, true, players);

    vector<move_ordering> new_moves;
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child = (*new_state);
      child.execute_move(1 - player, *iter, &current_players);
      child.evaluation_function1(current_players, moves + 1, depth - 1);
      move_ordering move((*iter), child.evaluation, child.terminal);
      new_moves.push_back(move);
    }

    std::sort(new_moves.begin(), new_moves.end(), greater_than_key());

    for (vector<move_ordering>::iterator iter = new_moves.begin(); iter != new_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child_state = (*new_state);
      child_state.execute_move(1 - player, (*iter).move, &current_players);
      child_state.evaluation = iter->evaluation;
      child_state.terminal = iter->terminal;
      double child;
      if (player == 0) {
        child = minval(&child_state, current_players, alpha, beta, depth - 1, true, moves + 1);
      } else {
        child = minval(&child_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      }
      alpha = max(alpha, child);
      if (alpha >= beta) {
        if (depth == depth_initial) {
          this->execute_optimal_move((*iter).move, first);
        }
        return child;
      }
      if (best_child < child) {
        best_child = child;
        best_move = (*iter).move;
      }
    }
  } else {
    vector<string> all_moves = new_state->generate_all_moves(player, false, players);

    vector<move_ordering> new_moves;
    for (vector<string>::iterator iter = all_moves.begin(); iter != all_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child = (*new_state);
      child.execute_move(player, *iter, &current_players);
      child.evaluation_function1(current_players, moves + 1, depth - 1);
      move_ordering move((*iter), child.evaluation, child.terminal);
      new_moves.push_back(move);
    }

    std::sort(new_moves.begin(), new_moves.end(), greater_than_key());

    for (vector<move_ordering>::iterator iter = new_moves.begin(); iter != new_moves.end(); iter++) {
      vector<Player> current_players = players;
      state child_state = (*new_state);
      child_state.execute_move(player, (*iter).move, &current_players);
      child_state.evaluation = iter->evaluation;
      child_state.terminal = iter->terminal;
      double child = minval(&child_state, current_players, alpha, beta, depth - 1, false, moves + 1);
      alpha = max(alpha, child);
      if (alpha >= beta) {
        if (depth == depth_initial) {
          this->execute_optimal_move((*iter).move, first);
        }
        return child;
      }
      if (best_child < child) {
        best_child = child;
        best_move = (*iter).move;
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
  // for (int i = 0; i < game_state.stacks.size(); i++) {
  //   if (game_state.board[i].size() > 1) {
  //     cerr << "For square : " << i << " " << game_state.stacks[i].reserves << " " << game_state.stacks[i].captives << " " << game_state.stacks[i].top << " " << game_state.stacks[i].controller << endl;
  //   }
  // }
  move = move + '\n';
  cout << move;
  //std::cerr << "my move : " << move << " move length : " << move.length() << '\n';
  gettimeofday(&finish, NULL);
  time_remaining = time_remaining - ((finish.tv_sec - start.tv_sec) + ((finish.tv_usec - start.tv_usec) / 1000000));
}
