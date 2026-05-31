
#include "CMenu.h"
#include "CMenuItem.h"
#include "app_exception.h"
#include "auth.h"
#include "func.h"
#include "supplier.h"
#include "user.h"
#include "worker.h"
#include <exception>
#include <iostream>
#include <limits>
#include <string>

using namespace std;
using namespace GNA;

void demo_simple_polymorphism() {
  user *u =
      new worker("Anna", "Ivanova", 25, "anna_i", "securepass", "Manager");
  cout << "\n[user* u = new worker(...)]" << endl;
  u->show_info();
  u->do_work();
  delete u;
}

void demo_complex_polymorphism() {

  user *team[3];
  team[0] =
      new worker("Pavel", "Nava", 20, "navap", "pass1234", "Programmer");
  team[1] =
      new supplier("Ivan", "Ivanov", 35, "ivani", "pass1234", "Electronics");
  team[2] =
      new worker("Maria", "Sokol", 28, "maria", "pass1234", "Designer");

  for (user *item : team) {
    item->show_info();
    item->do_work();
  }

  for (user *item : team)
    delete item;
}

bool run_auth(AuthSystem &auth, string &role) {
  cout << "\n================================================" << endl;
  cout << "  АВТОРИЗАЦИЯ" << endl;
  cout << "================================================" << endl;

  string login, pass;
  int attempts = 3;

  while (attempts > 0) {
    cout << "Логин: ";
    if (!(cin >> login)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      throw AppException(ExceptionType::Input,
                         "Логин обязателен для ввода.");
    }
    cout << "Пароль: ";
    if (!(cin >> pass)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      throw AppException(ExceptionType::Input,
                         "Пароль обязателен для ввода.");
    }

    if (auth.login(login, pass, role)) {
      cout << "\n[OK] Добро пожаловать! Роль: " << role << endl;
      return true;
    }
    attempts--;
    if (attempts > 0)
      cout << "[!] Неверно. Осталось попыток: " << attempts << endl;
  }

  cout << "[X] Доступ заблокирован." << endl;
  return false;
}

int run_role_menu(const string &title, CMenuItem *items, size_t count) {
  CMenu menu(title, items, count);
  bool is_running = true;
  while (is_running) {
    try {
      is_running = menu.runCommand();
    } catch (const AppException &e) {
      cout << e.what() << endl;
    } catch (const exception &e) {
      cout << "[Стандартное исключение] " << e.what() << endl;
    }
  }
  return 0;
}

int main() {
  try {

  AuthSystem auth("accounts.txt");
  string role;
  if (!run_auth(auth, role)) {
    cout << "Выход из программы." << endl;
    return 1;
  }
  load_users_from_files();

  if (role == "admin") {
    const int ITEMS_NUMBER = 19;
    CMenuItem items[ITEMS_NUMBER]{
      CMenuItem{"Показать всех сотрудников", menu_show_all_workers},
      CMenuItem{"Показать всех поставщиков", menu_show_all_suppliers},
      CMenuItem{"Загрузить базу", menu_load_database},
      CMenuItem{"Выгрузить базу", menu_unload_database},
      CMenuItem{"Добавить сотрудника", menu_add_worker},
      CMenuItem{"Добавить поставщика", menu_add_supplier},
      CMenuItem{"Удалить сотрудника", menu_delete_worker},
      CMenuItem{"Удалить поставщика", menu_delete_supplier},
      CMenuItem{"Редактировать сотрудника", menu_edit_worker},
      CMenuItem{"Редактировать поставщика", menu_edit_supplier},
      CMenuItem{"Поиск сотрудников", menu_search_workers},
      CMenuItem{"Поиск поставщиков", menu_search_suppliers},
      CMenuItem{"Сортировка", menu_sort},
      CMenuItem{"Показать производство", menu_show_production},
      CMenuItem{"Показать продажи", menu_show_sales},
      CMenuItem{"Добавить запись производства", menu_add_production},
      CMenuItem{"Добавить запись продажи", menu_add_sale},
      CMenuItem{"Создать отчет", menu_create_report},
      CMenuItem{"Добавить товар поставщику", menu_add_product_to_supplier}};
    return run_role_menu("=== МЕНЮ АДМИНИСТРАТОРА ===", items, ITEMS_NUMBER);
  }

  if (role == "worker") {
    const int ITEMS_NUMBER = 12;
    CMenuItem items[ITEMS_NUMBER]{
      CMenuItem{"Показать всех сотрудников", menu_show_all_workers},
      CMenuItem{"Показать всех поставщиков", menu_show_all_suppliers},
      CMenuItem{"Поиск сотрудников", menu_search_workers},
      CMenuItem{"Поиск поставщиков", menu_search_suppliers},
      CMenuItem{"Сортировка", menu_sort},
      CMenuItem{"Показать производство", menu_show_production},
      CMenuItem{"Показать продажи", menu_show_sales},
      CMenuItem{"Добавить запись производства", menu_add_production},
      CMenuItem{"Добавить запись продажи", menu_add_sale},
      CMenuItem{"Создать отчет", menu_create_report},
      CMenuItem{"Загрузить базу", menu_load_database},
      CMenuItem{"Выгрузить базу", menu_unload_database}};
    return run_role_menu("=== МЕНЮ СОТРУДНИКА ===", items, ITEMS_NUMBER);
  }

  if (role == "supplier") {
    const int ITEMS_NUMBER = 6;
    CMenuItem items[ITEMS_NUMBER]{
      CMenuItem{"Показать всех поставщиков", menu_show_all_suppliers},
      CMenuItem{"Поиск поставщиков", menu_search_suppliers},
      CMenuItem{"Показать производство", menu_show_production},
      CMenuItem{"Показать продажи", menu_show_sales},
      CMenuItem{"Добавить товар поставщику", menu_add_product_to_supplier},
      CMenuItem{"Выгрузить базу", menu_unload_database}};
    return run_role_menu("=== МЕНЮ ПОСТАВЩИКА ===", items, ITEMS_NUMBER);
  }

  throw AppException(ExceptionType::Logic,
                     "Для роли \"" + role + "\" не настроено меню.");
  } catch (const AppException &e) {
    cout << e.what() << endl;
  } catch (const exception &e) {
    cout << "[Стандартное исключение] " << e.what() << endl;
  }

  return 1;
}
