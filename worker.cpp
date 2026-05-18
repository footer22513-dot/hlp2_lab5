

#include "worker.h"
#include <iostream>
#include <string>

using namespace std;

worker::worker(string fName, string lName, int a, string log, string pass,
               string pos)
    : user(fName, lName, a, log, pass), grade(pos) {
  cout << "Сотрудник " << fName << " " << lName
       << " принят на должность: " << pos << endl;
}

worker::~worker() {
  cout << "Сотрудник " << Get_first_name() << " уволен" << endl;
}

string worker::get_grade() const { return grade; }

void worker::set_grade(string pos) { grade = pos; }

void worker::show_info() {
  cout << "\n===  СОТРУДНИКА ===" << endl;
  cout << "Имя: " << Get_first_name() << " " << Get_last_name() << endl;
  cout << "Возраст: " << Get_age() << endl;
  cout << "Логин: " << get_login() << endl;
  cout << "ДОЛЖНОСТЬ: " << grade << endl;
  cout << "===========================\n" << endl;
}

void worker::do_work() {
  cout << "🔨 " << Get_first_name() << " работает на позиции \"" << grade
       << "\"..." << endl;
}
