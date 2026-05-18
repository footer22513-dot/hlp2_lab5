#pragma once

#include <string>

using namespace std;

namespace GNA {
class CMenuItem {
public:
  typedef int (*Func)(); // создание типа который вызывает функцию без
                         // параметров, возвращает int
  CMenuItem(std::string, Func); // констуктор
                                //
                                // attribute

  Func func{};
  string item_name{};
  string getName();
  void print();
  int run();
};
} // namespace GNA
