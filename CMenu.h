#pragma once

#include <string>
#include <cstddef>
#include "CMenuItem.h"

using namespace std;


namespace GNA {
    class CMenu {
    public:
        CMenu(string, CMenuItem *, size_t);
        int getSelect() const;
        int runCommand();

        bool isRun() const;

        string getTitle();
        size_t getCount() const;

        CMenuItem *getItems();

        void print();
        

    private:

        int select{-1};
        size_t count{};
        bool running{};
        string title{};
        CMenuItem *items{};
    };
}
