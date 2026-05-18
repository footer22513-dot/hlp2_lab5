#include "CMenu.h"
#include <iostream>

namespace GNA {

    CMenu::CMenu(string title, CMenuItem *items, size_t count) : title(title), items(items), count(count) {}

    void CMenu::print() {
        for (size_t i{}; i < count; ++i) {
            std::cout << i + 1 << ". ";
            items[i].print();
            std::cout << std::endl;
        }
        std::cout << "0. exit" << std::endl;
    } // Распечатка меню, вызываеться объект items и метод внутри .print()

    int CMenu::runCommand() {
        print();
        std::cout << "\n   Select >> ";
        std::cin >> select;
        if (select == 0) {
            return 0;
        }
        return items[select-1].run();
    } // вызов комманды из объекта меню, к нему также идет метод .run() 
}
