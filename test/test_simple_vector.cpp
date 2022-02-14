#include "simple_vector.hpp"

int main(int argc, char**argv) {
  SimpleVector<int> sv;
  sv.push_back(100);
  sv.push_back(1000);
  sv.push_back(1000);
  sv.push_back(10000);

  for (auto it = sv.begin(); it != sv.end(); it++) {
    cout << "total size: " << sv.size() << ", cur value: " << *it << endl;
  }

  return 0;
}
