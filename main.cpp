#include <iostream>
#include <vector>

#include "variables.hpp"
#include "state.hpp"
#include "minimax.hpp"
#include "utility_functions.h"

int n;
int player;
int total_squares;
int max_movable;
int max_up;
int max_down;
char max_left;
char max_right;
vector<string> all_squares;
vector<vector<vector<int>>> partitions;
vector<int> x;
vector<int> y;
vector<bool> has_left;
vector<bool> has_right;
vector<bool> has_down;
vector<bool> has_up;

void partition(int n) {
  vector<std::vector<int>> output;
  vector<int> base_case;
  base_case.push_back(n);
  output.push_back(base_case);
  for (int i = 1; i < n; i++) {
    if (partitions[n - i].size() == 0) {
      partition(n - i);
    }
    for (auto iter = partitions[n - i].begin(); iter != partitions[n - i].end(); iter++) {
      vector<int> temp;
      temp.push_back(i);
      temp.insert(temp.end(), (*iter).begin(), (*iter).end());
      output.push_back(temp);
    }
  }
  partitions[n] = output;
}

void initialize_system() {
  total_squares = n * n;
  max_movable = n;
  max_up = n;
  max_down = 1;
  max_left = 'a';
  max_right = (char) ('a' + n - 1);

  for (int i = 0; i < total_squares; i++) {
    all_squares.push_back(square_to_string(i));
  }

  partitions.resize(n + 1);
  partition(n);
  //initialize the components of flood_fill
  x.resize(n * n);
  y.resize(n * n);
  has_left.resize(n * n);
  has_right.resize(n * n);
  has_up.resize(n * n);
  has_down.resize(n * n);

  for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			x[i + n*j] = i;
			y[i + n*j] = j;

			if (i > 0){
				has_left[i+n*j] = true;
			}
			if( i == 0){
				has_left[i+n*j] = false;
			}
			if (i < (n - 1)){
				has_right[i+n*j] = true;
			}
			if( i == n - 1){
				has_left[i+n*j] = true;
			}
      if (j > 0){
				has_down[i+n*j] = true;
			}
			if( j == 0){
				has_down[i+n*j] = false;
			}
			if (j < (n - 1)){
				has_up[i+n*j] = true;
			}
			if( j == (n - 1)){
				has_up[i+n*j] = false;
			}
		}
	}
}

int main() {
  srand(time(NULL));
  static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 required");

  int time_input;
  cin >> player >> n >> time_input;
  player = player - 1;

  initialize_system();

  // for (int i = 0; i < partitions.length)
  std::cerr << "System is initialized!" << '\n';
  minimax my_player(time_input);
  return 0;
}
