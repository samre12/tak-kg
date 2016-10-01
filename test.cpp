#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<std::vector<int>> partition(int n) {
  vector<std::vector<int>> output;
  vector<int> base_case;
  base_case.push_back(n);
  output.push_back(base_case);
  for (int i = 1; i < n; i++) {
    vector<vector<int>> small_partition = partition(n - i);
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

int main() {
    string move;
    std::cout.flush();
    std::getline(cin, move);
    cout << move << endl;

    return 0;
}
