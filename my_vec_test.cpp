// Simple tests for my_vec<T>
#include "my_vec.h"
#include <cassert>

int main() {
  // Test with int
  my_vec<int> v;
  assert(v.size() == 0);
  assert(v.empty());

  v.push_back(10);
  v.push_back(20);
  v.push_back(30);
  assert(v.size() == 3);
  assert(v[0] == 10);
  assert(v.at(1) == 20);
  assert(v.back() == 30);
  assert(v.front() == 10);

  // Test erase
  v.erase(1); // remove element 20
  assert(v.size() == 2);
  assert(v[0] == 10);
  assert(v[1] == 30);

  // Test pop_back
  v.pop_back();
  assert(v.size() == 1);
  assert(v.back() == 10);

  // Test clear
  v.clear();
  assert(v.empty());
  assert(v.size() == 0);

  // Test iterator
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it) {
    sum += *it;
  }
  assert(sum == 6);

  // Test out_of_range exception
  bool caught = false;
  try {
    v.at(5);
  } catch (const std::out_of_range &) {
    caught = true;
  }
  assert(caught);

  return 0;
}
