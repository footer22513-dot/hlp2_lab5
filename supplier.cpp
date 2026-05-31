
#include "app_exception.h"
#include "supplier.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

supplier::supplier(string fName, string lName, int a, string log, string pass,
                   string p_type)
    : user(fName, lName, a, log, pass), product_type(p_type) {
    if (product_type.empty()) {
        throw AppException(ExceptionType::Input,
                           "Тип продукции не должен быть пустым.");
    }
    std::ifstream in(get_login() + "_products.txt");
    if (in) {
        string name, type;
        double price;
        int count;
        while (in >> name >> type >> price >> count) {
            product_list.push_back(product(name, type, price, count));
        }
    }
}
supplier::~supplier() {};

string supplier::get_product_type() const { return product_type; }

void supplier::set_product_type(string pType) {
    if (pType.empty()) {
        throw AppException(ExceptionType::Input,
                           "Тип продукции не должен быть пустым.");
    }
    product_type = pType;
}

void supplier::show_info() {
  cout << "\n=== КАРТОЧКА ПОСТАВЩИКА ===" << endl;
  cout << "Имя: " << Get_first_name() << " " << Get_last_name() << endl;
  cout << "Возраст: " << Get_age() << endl;
  cout << "Логин: " << get_login() << endl;
  cout << "ТИП ПРОДУКЦИИ: " << product_type << endl;
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
    std::ofstream out(get_login() + "_products.txt", std::ios::app);
    if (!out) {
        throw AppException(ExceptionType::File,
                           "Не удалось открыть файл товаров поставщика.");
    }
    product_list.push_back(p);
    out << p.get_name() << " " << p.get_type() << " " << p.get_price()
        << " " << p.get_count() << "\n";
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

    cout << "Имя: ";
    if (!(is >> fName)) {
        throw AppException(ExceptionType::Input, "Имя обязательно для ввода.");
    }
    cout << "Фамилия: ";
    if (!(is >> lName)) {
        throw AppException(ExceptionType::Input,
                           "Фамилия обязательна для ввода.");
    }
    cout << "Возраст: ";
    if (!(is >> age)) {
        throw AppException(ExceptionType::Input,
                           "Возраст должен быть целым числом.");
    }
    cout << "Логин: ";
    if (!(is >> log)) {
        throw AppException(ExceptionType::Input,
                           "Логин обязателен для ввода.");
    }
    cout << "Пароль: ";
    if (!(is >> pass)) {
        throw AppException(ExceptionType::Input,
                           "Пароль обязателен для ввода.");
    }
    cout << "Тип продукции: ";
    if (!(is >> pType)) {
        throw AppException(ExceptionType::Input,
                           "Тип продукции обязателен для ввода.");
    }

    sup.set_first_name(fName);
    sup.set_last_name(lName);
    sup.set_Age(age);
    sup.set_login(log);
    sup.set_pass(pass);
    sup.set_product_type(pType);

    return is;
}
