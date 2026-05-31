

#pragma once
#include <string>
#include <iostream>

using namespace std;

class product {
protected:
  string name;
  string type;
  double price;
  int count;

public:
  product(string name = "", string type = "", double price = 0.0, int count = 0);
  ~product();
  string get_name() const;
  string get_type() const;
  double get_price() const;
  int get_count() const;
  double get_total_value() const;

  void set_name(string name);
  void set_type(string type);
  void set_price(double price);
  void set_count(int count);

  void add_to_stock(int count);
  void remove_from_stock(int count);
  void show_info() const;

  bool operator==(const product& other) const;
  bool operator!=(const product& other) const;
  bool operator<(const product& other) const;
  bool operator>(const product& other) const;
  bool operator<=(const product& other) const;
  bool operator>=(const product& other) const;

  friend ostream& operator<<(ostream& os, const product& prod);
  friend istream& operator>>(istream& is, product& prod);
};
