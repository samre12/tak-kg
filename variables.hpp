#ifndef variables_h
#define variables_h

#include <vector>

using namespace std;

extern int n;
extern int player;
extern int total_squares;
extern int max_movable;
extern int max_up;
extern int max_down;
extern char max_left;
extern char max_right;
extern vector<string> all_squares;
extern vector<vector<vector<int>>> partitions;
extern vector<int> x;
extern vector<int> y;
extern vector<bool> has_left;
extern vector<bool> has_right;
extern vector<bool> has_down;
extern vector<bool> has_up;
extern int states_explored;

#endif
