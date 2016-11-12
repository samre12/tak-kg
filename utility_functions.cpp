#ifndef utility_functions_c
#define utility_functions_c

#include "utility_functions.h"

int return_digit(char input) {
  if (input >= '0' && input <= '9')
    return (int) (input - '0');
  else
    return -1;
}

void print_vector(vector<pair<int, char>> vec) {
  auto iter = vec.begin();
  cerr << "vector is : ";
  while (iter != vec.end()) {
    cerr << (*iter).first << (*iter).second << " ";
    iter++;
  }
  cerr << endl;
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
        if (row > n || col > n) {
          return -1;
        } else {
          return n * (col - 1) + (row - 1);
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
  if (square > total_squares || square < 0) {
    return "";
  } else {
    string row = "";
    row.push_back((char)((square % n) + 97));
    string column = to_string((square / n) + 1);
    return row + column;
  }
}

#endif
