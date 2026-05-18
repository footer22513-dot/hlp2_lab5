#pragma once
#include "user.h"
#include "product.h"
#include "my_vec.h"
#include <string>
#include <iostream>
using namespace std;

class supplier : public user {
private:
  string product_type;
    my_vec<product> product_list;

public:
  supplier(string fName = "", string lName = "", int a = 0, string log = "",
           string pass = "", string pType = "");
  ~supplier();

  string get_product_type() const;
  void set_product_type(string pType);

  void show_info() override;
  void do_work() override;
    my_vec<product>& get_products();
    void add_product(const product& p);

  // Операторы сравнения (по фамилии)
  bool operator<(const supplier& other) const { return Get_last_name() < other.Get_last_name(); }
  bool operator>(const supplier& other) const { return Get_last_name() > other.Get_last_name(); }
  bool operator==(const supplier& other) const { return Get_last_name() == other.Get_last_name(); }

  friend ostream& operator<<(ostream& os, const supplier& sup);
  friend istream& operator>>(istream& is, supplier& sup);
};