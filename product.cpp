#include "product.h"
#include <iostream>
#include <string>
using namespace std;

product::product(string n, string type, double p, int c)
    : name(n), type(type), price(p), count(c) {
  if (n != "") {
    cout << "Объект с именем " << n << " создан" << endl;
  }
}

product::~product() { 
  if (name != "") {
    cout << "Объект " << name << " уничтожен" << endl;
  }
}

// геттеры
string product::get_name() const { return name; }
string product::get_type() const { return type; }
double product::get_price() const { return price; }
int product::get_count() const { return count; }
double product::get_total_value() const { return price * count; }

// сеттеры

void product::set_name(string n) { name = n; }
void product::set_type(string t) { type = t; }

void product::set_price(double p) {
  if (p < 0) {
    cout << "Цена не может быть отрицательной" << endl;
    return;
  }
  price = p;
}

void product::set_count(int c) { count = c; }

void product::add_to_stock(int c) {
  if (c > 0) {
    count += c;
    cout << "Добавлено " << c << " шт." << endl;
  } else {
    cout << "Нельзя добавить отрицательное кол-во!" << endl;
  }
}

void product::remove_from_stock(int c) {
  if (c > 0 && c <= count) {
    count -= c;
    cout << "Удалено " << c << " шт." << endl;
  } else {
    cout << "Недостаточно товара или неверное число!" << endl;
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

// ===== ПЕРЕГРУЗКИ ОПЕРАТОРОВ СРАВНЕНИЯ =====
bool product::operator==(const product& other) const {
  return name == other.name &&
         type == other.type &&
         price == other.price &&
         count == other.count;
}

bool product::operator!=(const product& other) const {
  return !(*this == other);
}

// Сравнение по имени, затем по цене
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

// ===== ДРУЖЕСТВЕННЫЕ ФУНКЦИИ ВВОДА/ВЫВОДА =====
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
  is >> name;
  cout << "Введите тип товара: ";
  is >> type;
  cout << "Введите цену: ";
  is >> price;
  cout << "Введите количество: ";
  is >> count;

  prod.set_name(name);
  prod.set_type(type);
  prod.set_price(price);
  prod.set_count(count);

  return is;
}
