
#include "app_exception.h"
#include "worker.h"
#include <iostream>
#include <string>

using namespace std;

worker::worker(string fName, string lName, int a, string log, string pass,
               string pos)
    : user(fName, lName, a, log, pass), grade(pos) {
  if (grade.empty()) {
    throw AppException(ExceptionType::Input,
                       "Должность не должна быть пустой.");
  }
}

worker::~worker() {}

string worker::get_grade() const { return grade; }

void worker::set_grade(string pos) {
  if (pos.empty()) {
    throw AppException(ExceptionType::Input,
                       "Должность не должна быть пустой.");
  }
  grade = pos;
}

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
