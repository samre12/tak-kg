#ifndef state_c
#define state_c

#include "state.hpp"

stack::stack(int res, int cap, char top_piece, int cont) {
  reserves = res;
  captives = cap;
  top = top_piece;
  controller = cont;
}

void state::initialize_state() {
  for (int i = 0; i < total_squares; i++) {
    vector<pair<int, char>> initial;
    board.push_back(initial);
  }
  min_x = 0;
  min_y = 0;
  max_x = 0;
  max_y = 0;

  stacks.resize(n * n);
}

bool state::check_valid(int square, char direction, vector<int> partition) {
  //for checking valid movement, stack square can moved in the direction
  int change;
  int next_square;
  if (direction == '+') {
    change = n;
  } else if (direction == '-') {
    change = -1 * n;
  } else if (direction == '>') {
    change = 1;
  } else if (direction == '<') {
    change = -1;
  }
  for (int i = 0; i < partition.size(); i++) {
    next_square = square + change * (i + 1);
    int length = board[next_square].size();
    if (length > 0 && board[next_square].back().second == 'C')
			return false;
		if (length > 0 && board[next_square].back().second == 'S' && (i != (partition.size() - 1)))
			return false;
		if (i == (partition.size() - 1) && length > 0 && board[next_square].back().second == 'S' && partition[i] > 1)
			return false;
		if (i == (partition.size() - 1) && length > 0 && board[next_square].back().second == 'S' && board[square].back().second != 'C')
			return false;
  }
  return true;
}

vector<string> state::generate_stack_moves(int square) {
  //generate stack moves from square, assumes active player is topmost color

  vector<string> all_moves;
  int r = square % n;
  int c = square / n;
  int size = board[square].size();
  char dirs[4] = {'+', '-', '<', '>'};
  int up = n - 1 - c;
  int down = c;
  int right = n - 1 - r;
  int left = r;
  int rem_squares[4] = {up, down, left, right};
  for (int i = 0; i < min(size, n); i++) {
    vector<vector<int>> part_list = partitions[i + 1];
    //std::cerr << "length of partitions for i : " << (i + 1) << " is : " << part_list.size() << '\n';
    for (int di = 0; di < 4; di++) {
      for (vector<vector<int> >::iterator iter = part_list.begin(); iter != part_list.end(); iter++) {
        if ((*iter).size() <= rem_squares[di]) {
          if (this->check_valid(square, dirs[di], (*iter))) {
            string move = "";
            int sum = 0;
            for (vector<int>::iterator parts = (*iter).begin(); parts != (*iter).end(); parts++) {
              move = move + to_string(*parts);
              sum += (*parts);
            }
            move = to_string(sum) + all_squares[square] + dirs[di] + move;
            all_moves.push_back(move);
          }
        }
      }
    }
  }
  return all_moves;
}

vector<string> state::generate_all_moves(int player, bool first, vector<Player> players) {
  //generate all possible moves for player, returns a list of move strings

	vector<string> all_moves;
  for (int i = 0; i < total_squares; i++) {
    if (board[i].size() == 0) {
      if (players[player].flats > 0) {
        string move = "F" + all_squares[i];
        all_moves.push_back(move);
      }
      //cannot move standing stone of other player
      if (!first && players[player].flats > 0) {
        string move = "S" + all_squares[i];
        all_moves.push_back(move);
      }
      //cannot move capstone of other player
      if (!first && players[player].capstones > 0) {
        string move = "C" + all_squares[i];
        all_moves.push_back(move);
      }
    }
  }

  for (int i = 0; i < total_squares; i++) {
    if (board[i].size() > 0 && board[i].back().first == player && !first) {
      vector<string> stack_moves = this->generate_stack_moves(i);
      all_moves.insert(all_moves.end(), stack_moves.begin(), stack_moves.end());
    }
  }
  return all_moves;
}

void state::execute_move(int current_piece, string move_string, vector<Player> *players) {
  //execute move on the board
  //cerr << isalpha(move_string[0]) << " " << return_digit(move_string[0]) << endl;
  // if (move_string.length() == 0){
  //   std::cerr << "move of length 0 found" << '\n';
  // } else {
  //   std::cerr << "Exceute move called for " << move_string << '\n';
  // }
  if (isalpha(move_string[0]) != 0) {
		int square = square_to_num(move_string.substr(1, move_string.size() - 1));
		if (move_string[0] == 'F' || move_string[0] == 'S') {
			board[square].push_back(make_pair(current_piece, move_string[0]));
      //print_vector(board[square]);
			(*players)[current_piece].flats -= 1;
    }
		else if (move_string[0] == 'C') {
			board[square].push_back(make_pair(current_piece, move_string[0]));
      //print_vector(board[square]);
			(*players)[current_piece].capstones -= 1;
    }
    stack new_stack(1, 0, board[square].back().second, current_piece);
    stacks[square] = new_stack;
  }
	else if (return_digit(move_string[0]) != -1) {
		int count = (int)(move_string[0] - '0');
    //cerr << "count is : " << count << endl;
		int square = square_to_num(move_string.substr(1, 2));
    int square_player = board[square].back().first;
		char direction = move_string[3];
    int change;
		if (direction == '+')
			change = n;
		else if (direction == '-')
			change = -1 * n;
		else if (direction == '>')
			change = 1;
		else if (direction == '<')
			change = -1;

    int prev_square = square;

    int total_reserves = 0;
    int total_captives = 0;
    for (int i = 4; i < move_string.size(); i++) {
      int next_count = (int)(move_string[i] - '0');
      //cerr << "next count is : " << next_count << endl;
      int next_square = prev_square + change;

      int reserves = 0;
      int captives = 0;
      int stack_reserves = stacks[next_square].reserves;
      int stack_captives = stacks[next_square].captives;

      vector<pair<int, char> >::iterator begin;
      vector<pair<int, char> >::iterator end;

      if (board[next_square].size() > 0 && board[next_square].back().second == 'S') {
        board[next_square][board[next_square].size() - 1] = make_pair(board[next_square].back().first, 'F');
      }
      if (next_count == count) {
        begin = board[square].begin() + (board[square].size() - count);
        end = board[square].end();
      } else {
        begin = board[square].begin() + (board[square].size() - count);
        end = board[square].begin() + (board[square].size() - count + next_count);
      }

      board[next_square].insert(board[next_square].end(), begin, end);

      for (vector<pair<int, char> >::iterator iter = begin; iter != end; iter++) {
        if (iter->first == square_player) {
          reserves++;
        } else {
          captives++;
        }
      }

      stack new_stack;
      new_stack.top = board[next_square].back().second;
      new_stack.controller = board[next_square].back().first;
      if (stacks[next_square].controller == board[next_square].back().first) {
        if (board[next_square].back().first == square_player) {
          new_stack.captives = stack_captives + captives;
          new_stack.reserves = stack_reserves + reserves;
        } else {
          new_stack.captives = stack_captives + reserves;
          new_stack.reserves = stack_reserves + captives;
        }
      } else {
        if (board[next_square].back().first == square_player) {
          new_stack.reserves = stack_captives + reserves;
          new_stack.captives = stack_reserves + captives;
        } else {
          new_stack.reserves = stack_captives + captives;
          new_stack.captives = stack_reserves + reserves;
        }
      }
      stacks[next_square] = new_stack;

      prev_square = next_square;
      count -= next_count;
      total_reserves += reserves;
      total_captives += captives;
    }

		count = (int)(move_string[0] - '0');
    vector<pair<int, char> >::iterator begin = board[square].begin() + (board[square].size() - count);
    vector<pair<int, char> >::iterator end = board[square].end();
		board[square].erase(begin, end);

    stacks[square].reserves -= total_reserves;
    stacks[square].captives -= total_captives;
    if (square_player != board[square].back().first) {
      int temp = stacks[square].reserves;
      stacks[square].reserves = stacks[square].captives;
      stacks[square].captives = temp;
    }
    stacks[square].controller = board[square].back().first;
    stacks[square].top = board[square].back().second;
    //print_vector(board[square]);
  }
}

double state::position_strength(int i) {
  double x_strength = abs(((n + 1) / 2) - x[i]);
  double y_strength = abs(((n + 1) / 2) - y[i]);
  return x_strength + y_strength;
}

bool state::are_remaining_pieces(vector<Player> players) {
  bool output = false;
  for (int i = 0; i < players.size(); i++) {
    if (players[i].flats > 0 || players[i].capstones > 0) {
      output = true;
    }
  }
  return output;
}


int state::flood_fill(int j, int player, vector<bool>* explored)
{
	int sum = 0;
  if ((board[j].size() > 0)) {
  	if (!(*explored)[j] && (board[j].back().first == player) && (board[j].back().second == 'F' || board[j].back().second == 'C'))
  	{
  		(*explored)[j] = true;
  		sum = sum + 1;

  		min_x = std::min(x[j],min_x);
  		max_x = std::max(x[j],max_x);
  		min_y = std::min(y[j],min_y);
  		max_y = std::max(y[j],max_y);

  		if (has_left[j]) {
        if (!(*explored)[j - 1]){
  			  sum += flood_fill(j - 1, player, explored);
        }
  		}

  		if (has_right[j]){
        if (!(*explored)[j + 1]) {
  			  sum += flood_fill(j + 1, player, explored);
        }
  		}

  		if (has_down[j]){
        if (!(*explored)[j - n]) {
  			  sum += flood_fill(j - n, player, explored);
        }
      }

  		if (has_up[j]){
        if (!(*explored)[j + n]) {
          sum += flood_fill(j + n, player, explored);
        }
      }
  	}
  }
  return sum;
}

vector<int> state::surrounding_influencer(int s, int i, int player) {
	vector<int> output(5);
  if (board[i - s].size() > 0) {
    if(board[i-s].back().first == (1- player) && (board[i-s].back().second == 'F' ) ){
  		output[0] = 1;
  	} else if(board[i-s].back().first == (1- player) && (board[i-s].back().second == 'S' || board[i-s].back().second == 'C')) {
  		output[1] = 1;
  	} else if(board[i-s].back().first == (1- player) && board[i-s].back().second == 'C'){
  		output[2] = 1;
  	} else if(board[i-s].back().first == player && board[i-s].back().second == 'F') {
  		output[3] = 1;
  	}else if(board[i-s].back().first == player && (board[i-s].back().second == 'S' || board[i-s].back().second == 'C')) {
  		output[4] = 1;
  	}
  }
	return output;
}

vector<int> state::rough_influence_measure(int i, int player) {
	int enemy_flats = 0;
	int enemy_blocks = 0;
	int enemy_caps = 0;
	int self_flats = 0;
	int self_blocks = 0;
	if (has_left[i]) {
		vector<int> inf = surrounding_influencer(1, i, player);
		enemy_flats += inf[0];
		enemy_blocks += inf[1];
		enemy_caps += inf[2];
		self_flats += inf[3];
		self_blocks += inf[4];
	}
	if (has_down[i]) {
		vector<int> inf = surrounding_influencer(n, i, player);
		enemy_flats += inf[0];
		enemy_blocks += inf[1];
		enemy_caps += inf[2];
		self_flats += inf[3];
		self_blocks += inf[4];
	}
	if (has_right[i]) {
		vector<int> inf = surrounding_influencer(-1, i, player);
		enemy_flats += inf[0];
		enemy_blocks += inf[1];
		enemy_caps += inf[2];
		self_flats += inf[3];
		self_blocks += inf[4];
	}
	if (has_up[i]) {
		vector<int> inf = surrounding_influencer(-1*n, i, player);
		enemy_flats += inf[0];
		enemy_blocks += inf[1];
		enemy_caps += inf[2];
		self_flats += inf[3];
		self_blocks += inf[4];
	}
	return {enemy_flats, enemy_blocks, enemy_caps, self_flats, self_blocks};
}

pair<double, double> state::evaluate_stack_strength() {
  double strength_player1 = 0;
  double strength_player2 = 0;

  for (int i = 0; i < stacks.size(); i++) {
    if (board[i].size() > 1) {
      int reserves = stacks[i].reserves;
      int captives = stacks[i].captives;
      int size = reserves + captives;
      if (stacks[i].controller == player) {
        if (stacks[i].top == 'C')
          strength_player1 += pow(reserves, 1.6) + captives;
        else if (stacks[i].top == 'S')
          strength_player1 += pow(reserves, 1.4) + captives;
        else
          strength_player1 += pow(reserves, 1.2) + captives;
      } else {
        if (stacks[i].top == 'C')
          strength_player2 += pow(reserves, 1.8) + pow(captives, 1.2);
        else if (stacks[i].top == 'S')
          strength_player2 += pow(reserves, 1.6) + pow(captives, 1.1);
        else
          strength_player2 += pow(reserves, 1.4) + captives;
      }
    }
  }
  //cerr << "strength evaluated" << endl;
  return make_pair(strength_player1, strength_player2);
}

void state::evaluation_function1(vector<Player> players, int moves) {
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
  bool p1_rw = false;
  bool p2_rw = false;
  bool complete = true;
  int counter1 = 0;
  int counter2 = 0;
	for (int i = 0; i < size; i++)
	{
    pair<double, double> stack_evaluation = evaluate_stack_strength();
    strength_player1 += stack_evaluation.first;
    strength_player2 += stack_evaluation.second;
    if ((board[i].size() > 0)) {
      //cerr << "board Top is " << board[i].back().first << " ";

      /********  Evaluate Stack Strength  and Position Strength  *********/
      if (!explored[i]) {
  			if ((board[i].back().first == player) && (board[i].back().second == 'F' || board[i].back().second == 'C'))
  			{
  				min_x = x[i];
  				max_x = x[i];
  				min_y = y[i];
  				max_y = y[i];
          int sum = flood_fill(i, player, &explored);
          counter1 += sum;
          //cerr << board[i].back().first << ", " << i << ", " << sum << " " << endl;
  				p1_isles.push_back(sum);
          int current_dim = 0;
          current_dim = std::max(max_y - min_y, max_x - min_x);
  				dim1 = std::max(current_dim, dim1);
  				dimsum1 = dimsum1 + dim1;
          strength_player1 += pow(sum, 1.2) + 2.5 * pow(current_dim, 2);
  			}
  			if ((board[i].back().first == (1 - player)) && (board[i].back().second == 'F' || board[i].back().second == 'C'))
  			{
  				min_x = x[i];
  				max_x = x[i];
  				min_y = y[i];
  				max_y = y[i];
  				int sum = flood_fill(i, 1 - player, &explored);
          counter2 += sum;
          //cerr << 1 - player << ", " << i << ", " << sum << " " << endl;
  				p2_isles.push_back(sum);
          int current_dim = 0;
          current_dim = std::max(max_y - min_y, max_x - min_x);
  				dim2 = std::max(current_dim, dim2);
  				dimsum2 = dimsum2 + dim2;
          strength_player2 += pow(sum, 2) + 3.5 * pow(current_dim, 2);
  			}
      }
    } else {
      complete = false;
    }
	}
  //cerr << "Completed Exploration" << endl;
	double eval1 = strength_player1 +  0.8 * players[player].flats + 2 * players[player].capstones;
	double eval2 = strength_player2 +  0.8 * players[1 - player].flats + 2 * players[1 - player].capstones;

  //need to check flat wins and draw positions
  pair<bool, double> output;

  if (complete || !are_remaining_pieces(players)) {
    if (counter1 > counter2) {
      std::cerr << "flatwin for player : " << player << '\n';
      eval1 = 10000;
      eval2 = 0;
      output.first = true;
      output.second = eval1 - eval2;
    } else if (counter1 < counter2) {
      std::cerr << "flatwin for player : " << 1 - player << '\n';
      eval2 = 10000 ;
      eval1 = 0;
      output.first = true;
      output.second = eval1 - eval2;
    } else {
      if (players[player].flats > players[1 - player].flats) {
        eval1 = 10000;
        eval2 = 0;
        output.first = true;
        output.second = eval1 - eval2;
      } else if (players[player].flats < players[1 - player].flats) {
        eval2 = 10000;
        eval1 = 0;
        output.first = true;
        output.second = eval1 - eval2;
      } else {
        output.first = true;
        output.second = 6000;
      }
    }
  }

  //if (dim1 >= 3)
  //  cerr << "Max Length Path : " << dim1 << endl;
  if(dim1 == n - 1)
	{
		p1_rw = true;
    std::cerr << "Road for possbile for player : " << player << '\n';
		eval1 = 20000;
		eval2 = 0;
	}
	if(dim2 == n - 1)
	{
		p2_rw = true;
    std::cerr << "Road for possbile for player : " << 1 - player << '\n';
		eval2 = 20000;
		eval1 = 0;
	}

  if (p1_rw || p2_rw) {
    //if (p1_rw)
    //  cerr << "Road Win possible!" << endl;
    output.first = true;
    output.second = eval1 - eval2;
  } else {
    output.first = false;
    output.second = eval1 - eval2;
  }
  this->terminal = output.first;
  this->evaluation = output.second;
}

// pair<bool, double> state::evaluation_function2(vector<Player> players, int moves) {
//   return make_pair(false, diff_flats(player));
// }

#endif
