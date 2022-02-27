#include "simple_vector.hpp"
#include "std_inc.hpp"

void test_iterator() {
  SimpleVector<int> v;
  v.push_back(100);
  v.push_back(1000);
  v.push_back(1000);
  v.push_back(10000);

  for (auto it = v.begin(); it != v.end(); it++) {
    std::ostringstream oss;
    oss << "capacity: " << v.capacity() << ", size: " << v.size() << ", ";
    oss << "origin value: " << *it;
    *it = *it + 1;
    oss << ", modified value: " << *it << "\n";
    cout << oss.str();
  }
}

void test_const_iterator(const SimpleVector<int>& v) {
  for (auto it = v.begin(); it != v.end(); it++) {
    std::ostringstream oss;
    oss << "capacity: " << v.capacity() << ", size: " << v.size() << ", ";
    oss << "origin value: " << *it;
    oss << ", modified value: " << *it << "\n";
    cout << oss.str();
  }
}

int main(int argc, char** argv) {
  cout << "test iterator:" << endl;
  cout << "------------------------------" << endl;
  test_iterator();
  cout << endl;

  cout << "const test iterator:" << endl;
  cout << "------------------------------" << endl;
  SimpleVector<int> v;
  v.push_back(100);
  v.push_back(1000);
  v.push_back(1000);
  v.push_back(10000);
  test_const_iterator(v);
  return 0;
}
