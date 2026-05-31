#include "app_exception.h"
#include "user.h"
#include <iostream>
using namespace std;

user::user(string fName, string lName, int a, string log, string pass)
    : first_name(fName), last_name(lName), age(a), login(log), password(pass) {
  if (first_name.empty() || last_name.empty()) {
    throw AppException(ExceptionType::Input,
                       "Имя и фамилия не должны быть пустыми.");
  }
  if (age <= 0) {
    throw AppException(ExceptionType::Range,
                       "Возраст должен быть положительным числом.");
  }
  if (login.length() < 5) {
    throw AppException(ExceptionType::Input,
                       "Логин должен быть не менее 5 символов.");
  }
  if (password.length() < 8) {
    throw AppException(ExceptionType::Input,
                       "Пароль должен быть не менее 8 символов.");
  }
};

user::~user() {};

string user::Get_first_name() const { return first_name; };

string user::Get_last_name() const { return last_name; };

int user::Get_age() const { return age; };

string user::get_login() const { return login; };

string user::get_password() const { return password; };

void user::set_first_name(string fName) {
  if (fName.empty()) {
    throw AppException(ExceptionType::Input,
                       "Имя не должно быть пустым.");
  }
  first_name = fName;
};

void user::set_last_name(string Lname) {
  if (Lname.empty()) {
    throw AppException(ExceptionType::Input,
                       "Фамилия не должна быть пустой.");
  }
  last_name = Lname;
};

void user::set_Age(int a) {
  if (a <= 0) {
    throw AppException(ExceptionType::Range,
                       "Возраст должен быть положительным числом.");
  }
  age = a;
}

void user::set_pass(string pass) {
  if (pass.length() < 8) {
    throw AppException(ExceptionType::Input,
                       "Пароль должен быть не менее 8 символов.");
  }
  password = pass;
}

void user::set_login(string log) {
  if (log.length() < 5) {
    throw AppException(ExceptionType::Input,
                       "Логин должен быть не менее 5 символов.");
  }
  login = log;
}
