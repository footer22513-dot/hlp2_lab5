#pragma once
#include <string>
using namespace std;

class user {
protected:
  string first_name;
  string last_name;
  int age;
  string login;
  string password;

public:
  user(string Fname, string Lname, int age, string log, string pass);
  virtual ~user();

  // геттеры
  string Get_first_name() const;
  string Get_last_name() const;
  int Get_age() const;
  string get_login() const;
  string get_password() const;

  // сеттеры
  void set_first_name(string Fname);
  void set_last_name(string Lname);
  void set_Age(int age);
  void set_pass(string pass);
  void set_login(string log);

  virtual void show_info() = 0;
  virtual void do_work() = 0;
};