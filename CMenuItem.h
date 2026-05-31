#pragma once

#include <string>

using namespace std;

namespace GNA {
class CMenuItem {
public:
  typedef int (*Func)();
  CMenuItem(std::string, Func);

  Func func{};
  string item_name{};
  string getName();
  void print();
  int run();
};
}
