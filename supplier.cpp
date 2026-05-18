

#include "supplier.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

supplier::supplier(string fName, string lName, int a, string log, string pass,
                   string p_type)
    : user(fName, lName, a, log, pass), product_type(p_type) {
    // Load existing products for this supplier from a file if it exists
    std::ifstream in(get_login() + "_products.txt");
    if (in) {
        product prod;
        while (in >> prod) {
            product_list.push_back(prod);
        }
    }
}
supplier::~supplier() {};

// Геттер и сеттер
string supplier::get_product_type() const { return product_type; }

void supplier::set_product_type(string pType) { product_type = pType; }

void supplier::show_info() {
  cout << "\n=== КАРТОЧКА ПОСТАВЩИКА ===" << endl;
  cout << "Имя: " << Get_first_name() << " " << Get_last_name() << endl;
  cout << "Возраст: " << Get_age() << endl;
  cout << "Логин: " << get_login() << endl;
  cout << "ТИП ПРОДУКЦИИ: " << product_type << endl;
  // List products supplied
  if (product_list.empty()) {
    cout << "Товары: (нет)" << endl;
  } else {
    cout << "Товары:" << endl;
    for (const auto& p : product_list) {
      p.show_info();
    }
  }
  cout << "===========================\n" << endl;
}

void supplier::do_work() { cout << "Supplier is working..." << endl; }

my_vec<product>& supplier::get_products() {
    return product_list;
}

void supplier::add_product(const product& p) {
    product_list.push_back(p);
    // Append the new product to the supplier's product file for persistence
    std::ofstream out(get_login() + "_products.txt", std::ios::app);
    if (out) {
        out << p << "\n";
    }
}


ostream& operator<<(ostream& os, const supplier& sup) {
    os << sup.Get_first_name() << "\n"
       << sup.Get_last_name() << "\n"
       << sup.Get_age() << "\n"
       << sup.get_login() << "\n"
       << sup.get_password() << "\n"
       << sup.product_type << "\n";
    return os;
}



istream& operator>>(istream& is, supplier& sup) {
    string fName, lName, log, pass, pType;
    int age;

    cout << "Имя: ";           is >> fName;
    cout << "Фамилия: ";       is >> lName;
    cout << "Возраст: ";       is >> age;
    cout << "Логин: ";         is >> log;
    cout << "Пароль: ";        is >> pass;
    cout << "Тип продукции: "; is >> pType;

    sup.set_first_name(fName);  // было Set_first_name
    sup.set_last_name(lName);   // было Set_last_name
    sup.set_Age(age);           // было Set_age
    sup.set_login(log);
    sup.set_pass(pass);         // было set_password
    sup.product_type = pType;

    return is;
}