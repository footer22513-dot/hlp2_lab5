#include "user.h"
#include <iostream>
using namespace std;

// констуктор + деструктор;
user::user(string fName, string lName, int a, string log, string pass)
    : first_name(fName), last_name(lName), age(a), login(log), password(pass) {

      };

user::~user() { cout << "Обьект пользователь уничтожен" << endl; };

// геттер

string user::Get_first_name() const { return first_name; };

string user::Get_last_name() const { return last_name; };

int user::Get_age() const { return age; };

string user::get_login() const { return login; };

string user::get_password() const { return password; };

// сеттер

void user::set_first_name(string fName) { first_name = fName; };

void user::set_last_name(string Lname) { last_name = Lname; };

void user::set_Age(int a) {
  if (a < 0) {
    cout << "Возраст не может быть отрицательным" << endl;
    return;
  }
  age = a;
}

void user::set_pass(string pass) {
  if (pass.length() < 8) {
    cout << "Пароль должен быть не менее 8 символов" << endl;
  }
  password = pass;
}

void user::set_login(string log) {
  if (log.length() < 5) {
    cout << "Логин должен быть не менее 5 символов" << endl;
  }
  login = log;
}
