#pragma once
#include "../user/user.h"
#include <string>
#include <iostream>
using namespace std;

class employee : public user {
private:
  string role;

public:
  employee(string Fname = "", string Lname = "", int age = 0, string log = "",
           string pass = "", string role = "worker");
  ~employee();

  string get_role() const;
  void set_role(string role);

  void show_info() override;
  void do_work() override;

  // Операторы сравнения (по фамилии)
  bool operator<(const employee& other) const { return Get_last_name() < other.Get_last_name(); }
  bool operator>(const employee& other) const { return Get_last_name() > other.Get_last_name(); }
  bool operator==(const employee& other) const { return Get_last_name() == other.Get_last_name(); }

  friend ostream& operator<<(ostream& os, const employee& emp);
  friend istream& operator>>(istream& is, employee& emp);
};