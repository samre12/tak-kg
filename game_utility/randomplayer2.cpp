#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <sys/time.h>

#include "Game.h"
#include "Game.cpp"

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
    int moves = 0;
    string move;
    std::vector<vector<pair<int, char>>> board;
    std::vector<Player> players;

    for (int i = 0; i < this->game.total_squares; i++) {
      vector<pair<int, char>> initial;
      board.push_back(initial);
    }
    players.resize(2);
    players[0].flats = 21;
    players[0].capstones = 1;

    players[1].flats = 21;
    players[1].capstones = 1;

    while (true) {
      if (moves == 0) {
        //first move
        auto all_moves = game.generate_all_moves(1, true, board, players);
        cerr << "Possible Moves : ";
        auto iter = all_moves.begin();
        while (iter != all_moves.end()) {
          cerr << *iter << " ";
          iter++;
        }
        cerr << endl;
        move = all_moves[rand() % all_moves.size()];
        cerr << "Executing Move" << endl;
  			board = this->game.execute_move(1, move, board, &players);
  			move = move + '\n';
  			cerr << "Chosen move : " << move;
  			cout << move;
      } else {
        auto all_moves = game.generate_all_moves(0, false, board, players);
        cerr << "Possible Moves : ";
        auto iter = all_moves.begin();
        while (iter != all_moves.end()) {
          cerr << *iter << " ";
          iter++;
        }
        cerr << endl;
        move = all_moves[rand() % all_moves.size()];
        cerr << "Executing Move" << endl;
  			board = this->game.execute_move(0, move, board, &players);
  			move = move + '\n';
  			cerr << "Chosen move : " << move;
  			cout << move;
      }
      string oponent_move;
      cin >> oponent_move;
      std::cerr << "Oponent Move : " << oponent_move << std::endl;
      cerr << "Executing Move" << endl;
      if (moves == 0) {
        board = this->game.execute_move(0, oponent_move, board, &players);
      } else {
        board = this->game.execute_move(1, oponent_move, board, &players);
      }
      moves++;
    }
  }
};


int main() {
  srand(time(NULL));
  RandomPlayer randomPlayer;
  return 0;
}
