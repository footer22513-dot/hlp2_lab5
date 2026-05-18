#include "CMenuItem.h"
#include <iostream>

namespace GNA {
CMenuItem::CMenuItem(std::string item_name, Func func)
    : item_name(item_name), func(func) {}

std::string CMenuItem::getName() { return item_name; }

void CMenuItem::print() { std::cout << item_name; }

int CMenuItem::run() { return func(); }
} // namespace GNA
