#include "func.h"
#include "product.h"
#include "supplier.h"
#include "user.h"
#include "worker.h"
#include "my_vec.h"
#include <iostream>
using namespace std;

namespace GNA {

static my_vec<user *> users; // один вектор для всех — worker и supplier


int menu_show_all_workers() {
  cout << "\n=== СПИСОК СОТРУДНИКОВ ===" << endl;
  bool found = false;
  for (auto *u : users) {
    if (worker *w = dynamic_cast<worker *>(u)) {
      w->show_info();
      found = true;
    }
  }
  if (!found)
    cout << "(список пуст)" << endl;
  return 1;
}

int menu_show_all_suppliers() {
  cout << "\n=== СПИСОК ПОСТАВЩИКОВ ===" << endl;
  bool found = false;
  for (auto *u : users) {
    if (supplier *s = dynamic_cast<supplier *>(u)) {
      s->show_info();
      found = true;
    }
  }
  if (!found)
    cout << "(список пуст)" << endl;
  return 1;
}

int menu_add_worker() {
  cout << "\n=== ДОБАВИТЬ СОТРУДНИКА ===" << endl;
  string fname, lname, login, pass, grade;
  int age;
  cout << "Имя: ";
  cin >> fname;
  cout << "Фамилия: ";
  cin >> lname;
  cout << "Возраст: ";
  cin >> age;
  cout << "Логин: ";
  cin >> login;
  cout << "Пароль: ";
  cin >> pass;
  cout << "Должность: ";
  cin >> grade;
  users.push_back(new worker(fname, lname, age, login, pass, grade));
  cout << "[OK] Сотрудник добавлен." << endl;
  return 1;
}

int menu_add_supplier() {
    // existing code...
  cout << "\n=== ДОБАВИТЬ ПОСТАВЩИКА ===" << endl;
  string fname, lname, login, pass, ptype;
  int age;
  cout << "Имя: ";
  cin >> fname;
  cout << "Фамилия: ";
  cin >> lname;
  cout << "Возраст: ";
  cin >> age;
  cout << "Логин: ";
  cin >> login;
  cout << "Пароль: ";
  cin >> pass;
  cout << "Тип продукции: ";
  cin >> ptype;
  users.push_back(new supplier(fname, lname, age, login, pass, ptype));
  cout << "[OK] Поставщик добавлен." << endl;
  return 1;
}

int menu_show_product() {
  cout << "\n=== ДЕМО ТОВАРА ===" << endl;
  product p("Мышь", "Периферия", 1500.0, 25);
  p.show_info();
  p.add_to_stock(10);
  p.remove_from_stock(5);
  p.show_info();
  return 1;
}

int menu_polymorphism_demo() {
  cout << "\n=== ДЕМОНСТРАЦИЯ ПОЛИМОРФИЗМА ===" << endl;
  cout << "\n[Все пользователи в векторе user* — вызываем виртуальные методы]"
       << endl;
  if (users.empty()) {
    cout << "(вектор пуст, добавьте сотрудников или поставщиков)" << endl;
    return 1;
  }
  for (user *u : users) {
    u->show_info();
    u->do_work();
  }
  return 1;
}

int menu_add_product_to_supplier() {
    using namespace std;
    cout << "\n=== ДОБАВИТЬ ТОВАР ДЛЯ ПОСТАВЩИКА ===" << endl;
    string sup_login;
    cout << "Логин поставщика: ";
    cin >> sup_login;
    // найти поставщика в векторе users
    supplier *target = nullptr;
    for (auto *u : users) {
        if (supplier *s = dynamic_cast<supplier *>(u)) {
            if (s->get_login() == sup_login) { // get_login from base user
                target = s;
                break;
            }
        }
    }
    if (!target) {
        cout << "[!] Поставщик с таким логином не найден." << endl;
        return 1;
    }
    // ввод данных продукта
    string pname, ptype;
    double price;
    int count;
    cout << "Название продукта: ";
    cin >> pname;
    cout << "Тип продукта: ";
    cin >> ptype;
    cout << "Цена: ";
    cin >> price;
    cout << "Количество: ";
    cin >> count;
    product p(pname, ptype, price, count);
    target->add_product(p);
    cout << "[OK] Товар добавлен поставщику." << endl;
    return 1;
}

} // namespace GNA
