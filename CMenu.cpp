#include "CMenu.h"
#include "app_exception.h"
#include <limits>
#include <iostream>

namespace GNA {

    CMenu::CMenu(string title, CMenuItem *items, size_t count)
        : count(count), title(title), items(items) {}

    void CMenu::print() {
        for (size_t i{}; i < count; ++i) {
            std::cout << i + 1 << ". ";
            items[i].print();
            std::cout << std::endl;
        }
        std::cout << "0. exit" << std::endl;
    }

    int CMenu::runCommand() {
        print();
        std::cout << "\n   Select >> ";
        if (!(std::cin >> select)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw AppException(ExceptionType::Input,
                               "Нужно ввести номер пункта меню.");
        }
        if (select == 0) {
            return 0;
        }
        if (select < 0 || static_cast<size_t>(select) > count) {
            throw AppException(ExceptionType::Range,
                               "Пункта меню с таким номером нет.");
        }
        return items[select-1].run();
    }
}
