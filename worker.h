#pragma once
#include "user.h"
#include <string>
#include <fstream>

using namespace std;

class worker : public user {
private:
    string grade;

public:
    worker(string Fname, string Lname, int age, string log, string pass,
           string grade);
    ~worker();

    string get_grade() const;
    void set_grade(string grade);

    void show_info() override;
    void do_work() override;
};
