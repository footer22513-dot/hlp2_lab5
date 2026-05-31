#include "app_exception.h"
#include "product.h"
#include <iostream>
#include <string>
using namespace std;

product::product(string n, string type, double p, int c)
    : name(n), type(type), price(p), count(c) {
  bool default_product = name.empty() && type.empty() && price == 0.0 && count == 0;
  if (!default_product && (name.empty() || type.empty())) {
    throw AppException(ExceptionType::Input,
                       "Название и тип товара не должны быть пустыми.");
  }
  if (price < 0) {
    throw AppException(ExceptionType::Range,
                       "Цена не может быть отрицательной.");
  }
  if (count < 0) {
    throw AppException(ExceptionType::Range,
                       "Количество товара не может быть отрицательным.");
  }
}

product::~product() {}

string product::get_name() const { return name; }
string product::get_type() const { return type; }
double product::get_price() const { return price; }
int product::get_count() const { return count; }
double product::get_total_value() const { return price * count; }

void product::set_name(string n) {
  if (n.empty()) {
    throw AppException(ExceptionType::Input,
                       "Название товара не должно быть пустым.");
  }
  name = n;
}

void product::set_type(string t) {
  if (t.empty()) {
    throw AppException(ExceptionType::Input,
                       "Тип товара не должен быть пустым.");
  }
  type = t;
}

void product::set_price(double p) {
  if (p < 0) {
    throw AppException(ExceptionType::Range,
                       "Цена не может быть отрицательной.");
  }
  price = p;
}

void product::set_count(int c) {
  if (c < 0) {
    throw AppException(ExceptionType::Range,
                       "Количество товара не может быть отрицательным.");
  }
  count = c;
}

void product::add_to_stock(int c) {
  if (c > 0) {
    count += c;
    cout << "Добавлено " << c << " шт." << endl;
  } else {
    throw AppException(ExceptionType::Range,
                       "Количество для добавления должно быть положительным.");
  }
}

void product::remove_from_stock(int c) {
  if (c > 0 && c <= count) {
    count -= c;
    cout << "Удалено " << c << " шт." << endl;
  } else {
    throw AppException(ExceptionType::Range,
                       "Недостаточно товара или неверное количество.");
  }
}

void product::show_info() const {
  cout << "=== Информация о товаре ===" << endl;
  cout << "Название: " << name << endl;
  cout << "Тип: " << type << endl;
  cout << "Цена: " << price << " руб." << endl;
  cout << "Количество: " << count << " шт." << endl;
  cout << "Общая стоимость: " << get_total_value() << " руб." << endl;
  cout << "===========================" << endl;
}

bool product::operator==(const product& other) const {
  return name == other.name &&
         type == other.type &&
         price == other.price &&
         count == other.count;
}

bool product::operator!=(const product& other) const {
  return !(*this == other);
}

bool product::operator<(const product& other) const {
  if (name != other.name) {
    return name < other.name;
  }
  return price < other.price;
}

bool product::operator>(const product& other) const {
  return other < *this;
}

bool product::operator<=(const product& other) const {
  return *this < other || *this == other;
}

bool product::operator>=(const product& other) const {
  return *this > other || *this == other;
}

ostream& operator<<(ostream& os, const product& prod) {
  os << "[ " << prod.name << " | " << prod.type 
     << " | Цена: " << prod.price << " руб. | Кол-во: " << prod.count << " шт. ]";
  return os;
}

istream& operator>>(istream& is, product& prod) {
  string name, type;
  double price;
  int count;

  cout << "Введите название товара: ";
  if (!(is >> name)) {
    throw AppException(ExceptionType::Input,
                       "Название товара обязательно для ввода.");
  }
  cout << "Введите тип товара: ";
  if (!(is >> type)) {
    throw AppException(ExceptionType::Input,
                       "Тип товара обязателен для ввода.");
  }
  cout << "Введите цену: ";
  if (!(is >> price)) {
    throw AppException(ExceptionType::Input,
                       "Цена должна быть числом.");
  }
  cout << "Введите количество: ";
  if (!(is >> count)) {
    throw AppException(ExceptionType::Input,
                       "Количество должно быть целым числом.");
  }

  prod.set_name(name);
  prod.set_type(type);
  prod.set_price(price);
  prod.set_count(count);

  return is;
}
