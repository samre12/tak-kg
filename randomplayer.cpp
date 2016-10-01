#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <sys/time.h>
#include <ctype.h>

#include "utility_functions.cpp"

using namespace std;

struct Player{
  int flats;
  int capstones;
};

class Game{
public:
  int n;
  int total_squares;
  int turn;
  vector<vector<pair<int, char>>> board;
  int max_flats;
  int max_capstones;
  int max_movable;
  int max_up;
  int max_down;
  char max_left;
  char max_right;
  int moves;
  vector<Player> players;
  vector<string> all_squares;

  Game() {}

  void initialize_game(int n) {
    this->n = n;
    this->total_squares = n * n;
    switch (n) {
      case 5:
        this->max_flats = 21;
        this->max_capstones = 1;
        break;

      case 6:
        this->max_flats = 30;
        this->max_capstones = 1;
        break;

      case 7:
        this->max_flats = 40;
        this->max_capstones = 1;
        break;

      default:
        break;
    }
    //initialize the board with empty boxes
    for (int i = 0; i < this->total_squares; i++) {
      vector<pair<int, char>> initial;
      board.push_back(initial);
    }

    this->max_movable = n;
    this->max_up = n;
    this->max_down = 1;
    this->max_left = 'a';
    this->max_right = (char) ('a' + n - 1);
    this->moves = 0;
    this->turn = 0;

    players.resize(2);
    players[0].flats = this->max_flats;
    players[0].capstones = this->max_capstones;

    players[1].flats = this->max_flats;
    players[1].capstones = this->max_capstones;

    for (int i = 0; i < this->total_squares; i++) {
      all_squares.push_back(this->square_to_string(i));
      //cerr << this->square_to_num(all_squares.back()) << endl;
    }
  }

  int square_to_num(string square_string){
    //return -1 if the string is invalid
    if (square_string.size() != 2) {
      return -1;
    } else {
      int row, col;
      if (isalpha(square_string[0]) != 0) {
        if ((col = return_digit(square_string[1])) != -1) {
          row = (int) (square_string[0] - 96);
          if (row > this->n || col > this->n) {
            return -1;
          } else {
            return this->n * (col - 1) + (row - 1);
          }
        } else {
          return -1;
        }
      } else {
        return -1;
      }
    }
  }

  string square_to_string(int square) {
    //convert square to string
    if (square > this->total_squares || square < 0) {
      return "";
    } else {
      string row = "";
      row.push_back((char)((square % this->n) + 97));
      string column = to_string((square / this->n) + 1);
      return row + column;
    }
  }

  vector<std::vector<int>> partition(int n) {
    vector<std::vector<int>> output;
    vector<int> base_case;
    base_case.push_back(n);
    output.push_back(base_case);
    for (int i = 1; i < n; i++) {
      vector<vector<int>> small_partition = this->partition(n - i);
      auto iter = small_partition.begin();
      for (auto iter = small_partition.begin(); iter != small_partition.end(); iter++) {
        vector<int> temp;
        temp.push_back(i);
        temp.insert(temp.end(), (*iter).begin(), (*iter).end());
        output.push_back(temp);
      }
    }
    return output;
  }

  bool check_valid(int square, char direction, vector<int> partition) {
    //for checking valid movement, stack square can moved in the direction
    int change;
    int next_square;
    if (direction == '+') {
      change = this->n;
    } else if (direction == '-') {
      change = -1 * this->n;
    } else if (direction == '>') {
      change = 1;
    } else if (direction == '<') {
      change = -1;
    }
    for (int i = 0; i < partition.size(); i++) {
      next_square = square + change * (i + 1);
      int length = this->board[next_square].size();
      if (length > 0 && this->board[next_square].back().second == 'C')
				return false;
			if (length > 0 && this->board[next_square].back().second == 'S' && (i != (partition.size() - 1)))
				return false;
			if (i == (partition.size() - 1) && length > 0 && this->board[next_square].back().second == 'S' && partition[i] > 1)
				return false;
			if (i == (partition.size() - 1) && length > 0 && this->board[next_square].back().second == 'S' && this->board[square].back().second != 'C')
				return false;
    }
    return true;
  }

  vector<string> generate_stack_moves(int square) {
    //generate stack moves from square, assumes active player is topmost color

    vector<string> all_moves;
    int r = square % this->n;
    int c = square / this->n;
    int size = this->board[square].size();
    char dirs[4] = {'+', '-', '<', '>'};
    int up = this->n - 1 - c;
    int down = c;
    int right = this->n - 1 - r;
    int left = r;
    int rem_squares[4] = {up, down, left, right};
    for (int i = 0; i < min(size, this->n); i++) {
      auto part_list = this->partition(i + 1);
      for (int di = 0; di < 4; di++) {
        for (auto iter = part_list.begin(); iter != part_list.end(); iter++) {
          if ((*iter).size() <= rem_squares[di]) {
            if (this->check_valid(square, dirs[di], (*iter))) {
              string move = "";
              int sum = 0;
              for (auto parts = (*iter).begin(); parts != (*iter).end(); parts++) {
                move = move + to_string(*parts);
                sum += (*parts);
              }
              move = to_string(sum) + this->all_squares[square] + dirs[di] + move;
              all_moves.push_back(move);
            }
          }
        }
      }
    }
    return all_moves;
  }

  vector<string> generate_all_moves(int player) {
    //generate all possible moves for player, returns a list of move strings

		vector<string> all_moves;
    for (int i = 0; i < this->total_squares; i++) {
      if (this->board[i].size() == 0) {
        if (this->players[player].flats > 0) {
          string move = "F" + this->all_squares[i];
          all_moves.push_back(move);
        }
        //cannot move standing stone of other player
        if (this->moves != player && this->players[player].flats > 0) {
          string move = "S" + this->all_squares[i];
          all_moves.push_back(move);
        }
        //cannot move capstone of other player
        if (this->moves != player && this->players[player].capstones > 0) {
          string move = "C" + this->all_squares[i];
          all_moves.push_back(move);
        }
      }
    }

    for (int i = 0; i < this->total_squares; i++) {
      if (this->board[i].size() > 0 && this->board[i].back().first == player && this->moves != player) {
        auto stack_moves = this->generate_stack_moves(i);
        all_moves.insert(all_moves.end(), stack_moves.begin(), stack_moves.end());
      }
    }
    return all_moves;
  }

  void execute_move(string move_string) {
    //execute move on the board
    int current_piece;
    if (this->turn == 0)
			this->moves += 1;
		if (this->moves != 1)
			current_piece = this->turn;
		else
			current_piece = 1 - this->turn;

    cerr << isalpha(move_string[0]) << " " << return_digit(move_string[0]) << endl;
    if (isalpha(move_string[0]) != 0) {
			int square = this->square_to_num(move_string.substr(1, move_string.size() - 1));
			if (move_string[0] == 'F' || move_string[0] == 'S') {
				this->board[square].push_back(make_pair(current_piece, move_string[0]));
        print_vector(this->board[square]);
				this->players[current_piece].flats -= 1;
      }
			else if (move_string[0] == 'C') {
				this->board[square].push_back(make_pair(current_piece, move_string[0]));
        print_vector(this->board[square]);
				this->players[current_piece].capstones -= 1;
      }
    }
		else if (return_digit(move_string[0]) != -1) {
			int count = (int)(move_string[0] - '0');
      cerr << "count is : " << count << endl;
			int square = this->square_to_num(move_string.substr(1, 2));
			char direction = move_string[3];
      int change;
			if (direction == '+')
				change = this->n;
			else if (direction == '-')
				change = -1 * this->n;
			else if (direction == '>')
				change = 1;
			else if (direction == '<')
				change = -1;

      int prev_square = square;
      for (int i = 4; i < move_string.size(); i++) {
        int next_count = (int)(move_string[i] - '0');
        cerr << "next count is : " << next_count << endl;
        int next_square = prev_square + change;
        if (this->board[next_square].size() > 0 && this->board[next_square].back().second == 'S') {
          this->board[next_square][this->board[next_square].size() - 1] = make_pair(this->board[next_square].back().first, 'F');
        }
        if (next_count == count) {
          auto begin = this->board[square].begin() + (this->board[square].size() - count);
          auto end = this->board[square].end();
          this->board[next_square].insert(this->board[next_square].end(), begin, end);
          print_vector(this->board[next_square]);
        } else {
          auto begin = this->board[square].begin() + (this->board[square].size() - count);
          auto end = this->board[square].begin() + (this->board[square].size() - count + next_count);
          this->board[next_square].insert(this->board[next_square].end(), begin, end);
          print_vector(this->board[next_square]);
        }
        prev_square = next_square;
        count -= next_count;
      }

			count = (int)(move_string[0] - '0');
      auto begin = this->board[square].begin() + (this->board[square].size() - count);
      auto end = this->board[square].end();
			this->board[square].erase(begin, end);
      print_vector(this->board[square]);
    }
		this->turn = 1 - this->turn;
  }

};

class RandomPlayer{
public:
  int player;
  int n;
  int time_left;
  Game game;

  RandomPlayer() {
    int player, n, time_left;
    cin >> player >> n >> time_left;
    //cerr << player << n << time_left << endl;

    this->player = player - 1;
    this->n = n;
    this->time_left = time_left;
    game.initialize_game(this->n);
    this->play();
  }

  void play() {
    string move;
    if (this->player == 1) {
      std::getline(cin, move);
      std::cerr << "Oponent Move : " << move << std::endl;
      cerr << "Executing Move" << endl;
			this->game.execute_move(move);
    }
		while(true) {
			auto all_moves = this->game.generate_all_moves(this->player);
      cerr << "Possible Moves : ";
      auto iter = all_moves.begin();
      while (iter != all_moves.end()) {
        cerr << *iter << " ";
        iter++;
      }
      cerr << endl;
      move = all_moves[rand() % all_moves.size()];
      cerr << "Executing Move" << endl;
			this->game.execute_move(move);
			move = move + '\n';
			cerr << "Chosen move : " << move;
			cout << move;
      string oponent_move;
      cin >> oponent_move;
      std::cerr << "Oponent Move : " << oponent_move << std::endl;
      cerr << "Executing Move" << endl;
			this->game.execute_move(oponent_move);
    }
  }
};


int main() {
  srand(time(NULL));
  RandomPlayer randomPlayer;
  return 0;
}
