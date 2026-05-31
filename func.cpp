#include "func.h"
#include "product.h"
#include "supplier.h"
#include "user.h"
#include "worker.h"
#include "app_exception.h"
#include "my_vec.h"
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
using namespace std;

namespace GNA {

static my_vec<user *> users;
static const string WORKERS_FILE = "workers.txt";
static const string SUPPLIERS_FILE = "suppliers.txt";
static const string PRODUCTION_FILE = "production.txt";
static const string SALES_FILE = "sales.txt";
static const string REPORT_FILE = "report.txt";

struct ProductionRecord {
  string product_name;
  int count;
  double cost;
  string date;
};

struct SaleRecord {
  string product_name;
  int count;
  double price;
  string date;
};

static my_vec<ProductionRecord> production_records;
static my_vec<SaleRecord> sale_records;

static void clear_database() {
  for (auto *u : users) {
    delete u;
  }
  users.clear();
  production_records.clear();
  sale_records.clear();
}

static worker *find_worker_by_login(const string &login) {
  for (auto *u : users) {
    worker *w = dynamic_cast<worker *>(u);
    if (w && w->get_login() == login) {
      return w;
    }
  }
  return nullptr;
}

static supplier *find_supplier_by_login(const string &login) {
  for (auto *u : users) {
    supplier *s = dynamic_cast<supplier *>(u);
    if (s && s->get_login() == login) {
      return s;
    }
  }
  return nullptr;
}

static bool login_exists_except(const string &login, const string &ignored_login) {
  for (auto *u : users) {
    if (u->get_login() == login && u->get_login() != ignored_login) {
      return true;
    }
  }
  return false;
}

static string read_token_or_throw(const string &field_name) {
  string value;
  if (!(cin >> value)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Поле \"" + field_name + "\" обязательно для ввода.");
  }
  return value;
}

static int read_int_or_throw(const string &field_name) {
  int value;
  if (!(cin >> value)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Поле \"" + field_name + "\" должно быть целым числом.");
  }
  return value;
}

static double read_double_or_throw(const string &field_name) {
  double value;
  if (!(cin >> value)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Поле \"" + field_name + "\" должно быть числом.");
  }
  return value;
}

static void validate_common_user_fields(const string &fname,
                                        const string &lname, int age,
                                        const string &login,
                                        const string &pass,
                                        const string &ignored_login = "") {
  if (fname.empty() || lname.empty()) {
    throw AppException(ExceptionType::Input,
                       "Имя и фамилия не должны быть пустыми.");
  }
  if (age <= 0) {
    throw AppException(ExceptionType::Range,
                       "Возраст должен быть положительным числом.");
  }
  if (login.length() < 5) {
    throw AppException(ExceptionType::Input,
                       "Логин должен быть не менее 5 символов.");
  }
  if (pass.length() < 8) {
    throw AppException(ExceptionType::Input,
                       "Пароль должен быть не менее 8 символов.");
  }
  if (login_exists_except(login, ignored_login)) {
    throw AppException(ExceptionType::Logic,
                       "Пользователь с таким логином уже существует.");
  }
}

static void validate_date(const string &date) {
  bool valid = date.size() == 10 && date[4] == '-' && date[7] == '-';
  for (size_t i = 0; valid && i < date.size(); ++i) {
    if (i != 4 && i != 7 &&
        !isdigit(static_cast<unsigned char>(date[i]))) {
      valid = false;
    }
  }
  if (!valid) {
    throw AppException(ExceptionType::Input,
                       "Дата должна быть в формате ГГГГ-ММ-ДД.");
  }
  int month = stoi(date.substr(5, 2));
  int day = stoi(date.substr(8, 2));
  if (month < 1 || month > 12 || day < 1 || day > 31) {
    throw AppException(ExceptionType::Range,
                       "Дата содержит неверный месяц или день.");
  }
}

static void edit_common_user_fields(user *target) {
  string fname, lname, login, pass;
  int age;

  cout << "Новое имя: ";
  fname = read_token_or_throw("Новое имя");
  cout << "Новая фамилия: ";
  lname = read_token_or_throw("Новая фамилия");
  cout << "Новый возраст: ";
  age = read_int_or_throw("Новый возраст");
  cout << "Новый логин: ";
  login = read_token_or_throw("Новый логин");
  cout << "Новый пароль: ";
  pass = read_token_or_throw("Новый пароль");

  validate_common_user_fields(fname, lname, age, login, pass,
                              target->get_login());
  target->set_first_name(fname);
  target->set_last_name(lname);
  target->set_Age(age);
  target->set_login(login);
  target->set_pass(pass);
}

void save_users_to_files() {
  ofstream workers_file(WORKERS_FILE);
  ofstream suppliers_file(SUPPLIERS_FILE);

  if (!workers_file.is_open() || !suppliers_file.is_open()) {
    throw AppException(ExceptionType::File,
                       "Не удалось открыть файлы пользователей для сохранения.");
  }

  for (auto *u : users) {
    if (worker *w = dynamic_cast<worker *>(u)) {
      workers_file << w->Get_first_name() << ' ' << w->Get_last_name() << ' '
                   << w->Get_age() << ' ' << w->get_login() << ' '
                   << w->get_password() << ' ' << w->get_grade() << '\n';
    } else if (supplier *s = dynamic_cast<supplier *>(u)) {
      suppliers_file << s->Get_first_name() << ' ' << s->Get_last_name() << ' '
                     << s->Get_age() << ' ' << s->get_login() << ' '
                     << s->get_password() << ' ' << s->get_product_type()
                     << '\n';
    }
  }
}

static void save_production_to_file() {
  ofstream file(PRODUCTION_FILE);
  if (!file.is_open()) {
    throw AppException(ExceptionType::File,
                       "Не удалось открыть файл производства для сохранения.");
  }

  for (const auto &record : production_records) {
    file << record.product_name << ' ' << record.count << ' ' << record.cost
         << ' ' << record.date << '\n';
  }
}

static void save_sales_to_file() {
  ofstream file(SALES_FILE);
  if (!file.is_open()) {
    throw AppException(ExceptionType::File,
                       "Не удалось открыть файл продаж для сохранения.");
  }

  for (const auto &record : sale_records) {
    file << record.product_name << ' ' << record.count << ' ' << record.price
         << ' ' << record.date << '\n';
  }
}

static void save_supplier_products_to_files() {
  for (auto *u : users) {
    supplier *s = dynamic_cast<supplier *>(u);
    if (!s) {
      continue;
    }

    ofstream file(s->get_login() + "_products.txt");
    if (!file.is_open()) {
      throw AppException(ExceptionType::File,
                         "Не удалось сохранить товары поставщика " +
                             s->get_login() + ".");
    }

    my_vec<product> &products = s->get_products();
    for (const auto &p : products) {
      file << p.get_name() << ' ' << p.get_type() << ' ' << p.get_price()
           << ' ' << p.get_count() << '\n';
    }
  }
}

static void save_database_to_files() {
  save_users_to_files();
  save_production_to_file();
  save_sales_to_file();
  save_supplier_products_to_files();
}

static int load_production_from_file() {
  ifstream file(PRODUCTION_FILE);
  ProductionRecord record;
  int count = 0;

  while (file >> record.product_name >> record.count >> record.cost >>
         record.date) {
    production_records.push_back(record);
    ++count;
  }

  return count;
}

static int load_sales_from_file() {
  ifstream file(SALES_FILE);
  SaleRecord record;
  int count = 0;

  while (file >> record.product_name >> record.count >> record.price >>
         record.date) {
    sale_records.push_back(record);
    ++count;
  }

  return count;
}

static void load_database_from_files(bool show_message) {
  clear_database();

  ifstream workers_file(WORKERS_FILE);
  ifstream suppliers_file(SUPPLIERS_FILE);
  string fname, lname, login, pass, extra;
  int age;
  int workers_count = 0;
  int suppliers_count = 0;
  int skipped_count = 0;

  while (workers_file >> fname >> lname >> age >> login >> pass >> extra) {
    try {
      users.push_back(new worker(fname, lname, age, login, pass, extra));
      ++workers_count;
    } catch (const AppException &) {
      ++skipped_count;
    }
  }

  while (suppliers_file >> fname >> lname >> age >> login >> pass >> extra) {
    try {
      users.push_back(new supplier(fname, lname, age, login, pass, extra));
      ++suppliers_count;
    } catch (const AppException &) {
      ++skipped_count;
    }
  }

  int production_count = load_production_from_file();
  int sales_count = load_sales_from_file();

  if (show_message) {
    cout << "[OK] Загружено сотрудников: " << workers_count << endl;
    cout << "[OK] Загружено поставщиков: " << suppliers_count << endl;
    cout << "[OK] Загружено записей производства: " << production_count << endl;
    cout << "[OK] Загружено записей продаж: " << sales_count << endl;
    if (skipped_count > 0) {
      cout << "[!] Пропущено некорректных записей: " << skipped_count << endl;
    }
  }
}

void load_users_from_files() {
  load_database_from_files(false);
}

int menu_load_database() {
  cout << "\n=== ЗАГРУЗКА БАЗЫ ===" << endl;
  load_database_from_files(true);
  cout << "[OK] База загружена из файлов." << endl;
  return 1;
}

int menu_unload_database() {
  cout << "\n=== ВЫГРУЗКА БАЗЫ ===" << endl;
  save_database_to_files();
  cout << "[OK] База выгружена в файлы." << endl;
  return 1;
}


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
  fname = read_token_or_throw("Имя");
  cout << "Фамилия: ";
  lname = read_token_or_throw("Фамилия");
  cout << "Возраст: ";
  age = read_int_or_throw("Возраст");
  cout << "Логин: ";
  login = read_token_or_throw("Логин");
  cout << "Пароль: ";
  pass = read_token_or_throw("Пароль");
  cout << "Должность: ";
  grade = read_token_or_throw("Должность");
  validate_common_user_fields(fname, lname, age, login, pass);
  if (grade.empty()) {
    throw AppException(ExceptionType::Input,
                       "Должность не должна быть пустой.");
  }
  users.push_back(new worker(fname, lname, age, login, pass, grade));
  save_users_to_files();
  cout << "[OK] Сотрудник добавлен." << endl;
  return 1;
}

int menu_add_supplier() {
  cout << "\n=== ДОБАВИТЬ ПОСТАВЩИКА ===" << endl;
  string fname, lname, login, pass, ptype;
  int age;
  cout << "Имя: ";
  fname = read_token_or_throw("Имя");
  cout << "Фамилия: ";
  lname = read_token_or_throw("Фамилия");
  cout << "Возраст: ";
  age = read_int_or_throw("Возраст");
  cout << "Логин: ";
  login = read_token_or_throw("Логин");
  cout << "Пароль: ";
  pass = read_token_or_throw("Пароль");
  cout << "Тип продукции: ";
  ptype = read_token_or_throw("Тип продукции");
  validate_common_user_fields(fname, lname, age, login, pass);
  if (ptype.empty()) {
    throw AppException(ExceptionType::Input,
                       "Тип продукции не должен быть пустым.");
  }
  users.push_back(new supplier(fname, lname, age, login, pass, ptype));
  save_users_to_files();
  cout << "[OK] Поставщик добавлен." << endl;
  return 1;
}

int menu_delete_worker() {
  cout << "\n=== УДАЛИТЬ СОТРУДНИКА ===" << endl;
  string login;
  cout << "Логин сотрудника: ";
  login = read_token_or_throw("Логин сотрудника");

  for (size_t i = 0; i < users.size(); ++i) {
    worker *w = dynamic_cast<worker *>(users[i]);
    if (w && w->get_login() == login) {
      delete users[i];
      users.erase(i);
      save_users_to_files();
      cout << "[OK] Сотрудник удален." << endl;
      return 1;
    }
  }

  throw AppException(ExceptionType::Logic,
                     "Сотрудник с таким логином не найден.");
}

int menu_delete_supplier() {
  cout << "\n=== УДАЛИТЬ ПОСТАВЩИКА ===" << endl;
  string login;
  cout << "Логин поставщика: ";
  login = read_token_or_throw("Логин поставщика");

  for (size_t i = 0; i < users.size(); ++i) {
    supplier *s = dynamic_cast<supplier *>(users[i]);
    if (s && s->get_login() == login) {
      delete users[i];
      users.erase(i);
      save_users_to_files();
      cout << "[OK] Поставщик удален." << endl;
      return 1;
    }
  }

  throw AppException(ExceptionType::Logic,
                     "Поставщик с таким логином не найден.");
}

int menu_edit_worker() {
  cout << "\n=== РЕДАКТИРОВАТЬ СОТРУДНИКА ===" << endl;
  string old_login;
  cout << "Логин сотрудника: ";
  old_login = read_token_or_throw("Логин сотрудника");

  worker *target = find_worker_by_login(old_login);
  if (!target) {
    throw AppException(ExceptionType::Logic,
                       "Сотрудник с таким логином не найден.");
  }

  string grade;
  edit_common_user_fields(target);
  cout << "Новая должность: ";
  grade = read_token_or_throw("Новая должность");
  if (grade.empty()) {
    throw AppException(ExceptionType::Input,
                       "Должность не должна быть пустой.");
  }
  target->set_grade(grade);

  save_users_to_files();
  cout << "[OK] Сотрудник отредактирован." << endl;
  return 1;
}

int menu_edit_supplier() {
  cout << "\n=== РЕДАКТИРОВАТЬ ПОСТАВЩИКА ===" << endl;
  string old_login;
  cout << "Логин поставщика: ";
  old_login = read_token_or_throw("Логин поставщика");

  supplier *target = find_supplier_by_login(old_login);
  if (!target) {
    throw AppException(ExceptionType::Logic,
                       "Поставщик с таким логином не найден.");
  }

  string product_type;
  edit_common_user_fields(target);
  cout << "Новый тип продукции: ";
  product_type = read_token_or_throw("Новый тип продукции");
  if (product_type.empty()) {
    throw AppException(ExceptionType::Input,
                       "Тип продукции не должен быть пустым.");
  }
  target->set_product_type(product_type);

  save_users_to_files();
  cout << "[OK] Поставщик отредактирован." << endl;
  return 1;
}

int menu_search_workers() {
  cout << "\n=== ПОИСК СОТРУДНИКОВ ===" << endl;
  string query;
  cout << "Введите логин или фамилию: ";
  query = read_token_or_throw("Запрос поиска");

  bool found = false;
  for (auto *u : users) {
    worker *w = dynamic_cast<worker *>(u);
    if (w && (w->get_login() == query || w->Get_last_name() == query)) {
      w->show_info();
      found = true;
    }
  }

  if (!found) {
    throw AppException(ExceptionType::Logic, "Сотрудники не найдены.");
  }
  return 1;
}

int menu_search_suppliers() {
  cout << "\n=== ПОИСК ПОСТАВЩИКОВ ===" << endl;
  string query;
  cout << "Введите логин или фамилию: ";
  query = read_token_or_throw("Запрос поиска");

  bool found = false;
  for (auto *u : users) {
    supplier *s = dynamic_cast<supplier *>(u);
    if (s && (s->get_login() == query || s->Get_last_name() == query)) {
      s->show_info();
      found = true;
    }
  }

  if (!found) {
    throw AppException(ExceptionType::Logic, "Поставщики не найдены.");
  }
  return 1;
}

static bool worker_field_less(const worker *left, const worker *right,
                              int field) {
  switch (field) {
  case 1:
    return left->Get_first_name() < right->Get_first_name();
  case 2:
    return left->Get_last_name() < right->Get_last_name();
  case 3:
    return left->Get_age() < right->Get_age();
  case 4:
    return left->get_login() < right->get_login();
  case 5:
    return left->get_grade() < right->get_grade();
  default:
    return false;
  }
}

static bool supplier_field_less(const supplier *left, const supplier *right,
                                int field) {
  switch (field) {
  case 1:
    return left->Get_first_name() < right->Get_first_name();
  case 2:
    return left->Get_last_name() < right->Get_last_name();
  case 3:
    return left->Get_age() < right->Get_age();
  case 4:
    return left->get_login() < right->get_login();
  case 5:
    return left->get_product_type() < right->get_product_type();
  default:
    return false;
  }
}

static string worker_sort_field_name(int field) {
  switch (field) {
  case 1:
    return "имени";
  case 2:
    return "фамилии";
  case 3:
    return "возрасту";
  case 4:
    return "логину";
  case 5:
    return "должности";
  default:
    return "";
  }
}

static string supplier_sort_field_name(int field) {
  switch (field) {
  case 1:
    return "имени";
  case 2:
    return "фамилии";
  case 3:
    return "возрасту";
  case 4:
    return "логину";
  case 5:
    return "типу продукции";
  default:
    return "";
  }
}

static void sort_workers(int field, bool ascending) {
  for (size_t i = 0; i < users.size(); ++i) {
    worker *left = dynamic_cast<worker *>(users[i]);
    if (!left) {
      continue;
    }

    for (size_t j = i + 1; j < users.size(); ++j) {
      worker *right = dynamic_cast<worker *>(users[j]);
      bool need_swap = right && (ascending
                                     ? worker_field_less(right, left, field)
                                     : worker_field_less(left, right, field));
      if (need_swap) {
        user *tmp = users[i];
        users[i] = users[j];
        users[j] = tmp;
        left = right;
      }
    }
  }
}

static void sort_suppliers(int field, bool ascending) {
  for (size_t i = 0; i < users.size(); ++i) {
    supplier *left = dynamic_cast<supplier *>(users[i]);
    if (!left) {
      continue;
    }

    for (size_t j = i + 1; j < users.size(); ++j) {
      supplier *right = dynamic_cast<supplier *>(users[j]);
      bool need_swap = right && (ascending
                                     ? supplier_field_less(right, left, field)
                                     : supplier_field_less(left, right, field));
      if (need_swap) {
        user *tmp = users[i];
        users[i] = users[j];
        users[j] = tmp;
        left = right;
      }
    }
  }
}

int menu_sort() {
  cout << "\n=== СОРТИРОВКА ===" << endl;
  cout << "1. Сотрудники" << endl;
  cout << "2. Поставщики" << endl;
  cout << "0. Назад" << endl;
  cout << "\n   Select >> ";

  int database_select;
  if (!(cin >> database_select)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Нужно ввести номер пункта сортировки.");
  }
  if (database_select == 0) {
    return 1;
  }
  if (database_select != 1 && database_select != 2) {
    throw AppException(ExceptionType::Range,
                       "Пункта сортировки с таким номером нет.");
  }

  cout << "\nВыберите поле:" << endl;
  cout << "1. Имя" << endl;
  cout << "2. Фамилия" << endl;
  cout << "3. Возраст" << endl;
  cout << "4. Логин" << endl;
  if (database_select == 1) {
    cout << "5. Должность" << endl;
  } else {
    cout << "5. Тип продукции" << endl;
  }
  cout << "\n   Select >> ";

  int field_select;
  if (!(cin >> field_select)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Нужно ввести номер поля сортировки.");
  }
  if (field_select < 1 || field_select > 5) {
    throw AppException(ExceptionType::Range,
                       "Поля сортировки с таким номером нет.");
  }

  cout << "\nВыберите направление:" << endl;
  cout << "1. По возрастанию" << endl;
  cout << "2. По убыванию" << endl;
  cout << "\n   Select >> ";

  int direction_select;
  if (!(cin >> direction_select)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    throw AppException(ExceptionType::Input,
                       "Нужно ввести номер направления сортировки.");
  }
  if (direction_select != 1 && direction_select != 2) {
    throw AppException(ExceptionType::Range,
                       "Направления сортировки с таким номером нет.");
  }

  bool ascending = direction_select == 1;
  if (database_select == 1) {
    sort_workers(field_select, ascending);
    cout << "[OK] Сотрудники отсортированы по "
         << worker_sort_field_name(field_select) << " "
         << (ascending ? "по возрастанию." : "по убыванию.") << endl;
  } else {
    sort_suppliers(field_select, ascending);
    cout << "[OK] Поставщики отсортированы по "
         << supplier_sort_field_name(field_select) << " "
         << (ascending ? "по возрастанию." : "по убыванию.") << endl;
  }

  save_users_to_files();
  return 1;
}

int menu_show_production() {
  cout << "\n=== БАЗА ПРОИЗВОДСТВА ===" << endl;

  if (production_records.empty()) {
    cout << "(список пуст)" << endl;
    return 1;
  }

  for (size_t i = 0; i < production_records.size(); ++i) {
    const ProductionRecord &record = production_records[i];
    cout << i + 1 << ". Товар: " << record.product_name
         << ", количество: " << record.count
         << ", себестоимость: " << record.cost
         << ", дата: " << record.date << endl;
  }

  return 1;
}

int menu_show_sales() {
  cout << "\n=== БАЗА ПРОДАЖ ===" << endl;

  if (sale_records.empty()) {
    cout << "(список пуст)" << endl;
    return 1;
  }

  for (size_t i = 0; i < sale_records.size(); ++i) {
    const SaleRecord &record = sale_records[i];
    cout << i + 1 << ". Товар: " << record.product_name
         << ", количество: " << record.count << ", цена продажи: "
         << record.price << ", сумма: " << record.price * record.count
         << ", дата: " << record.date << endl;
  }

  return 1;
}

int menu_add_production() {
  cout << "\n=== ДОБАВИТЬ ЗАПИСЬ ПРОИЗВОДСТВА ===" << endl;

  ProductionRecord record;
  cout << "Название товара: ";
  record.product_name = read_token_or_throw("Название товара");
  cout << "Количество: ";
  record.count = read_int_or_throw("Количество");
  cout << "Себестоимость за единицу: ";
  record.cost = read_double_or_throw("Себестоимость за единицу");
  cout << "Дата (например 2026-05-31): ";
  record.date = read_token_or_throw("Дата");

  if (record.product_name.empty()) {
    throw AppException(ExceptionType::Input,
                       "Название товара не должно быть пустым.");
  }
  if (record.count <= 0) {
    throw AppException(ExceptionType::Range,
                       "Количество должно быть положительным.");
  }
  if (record.cost < 0) {
    throw AppException(ExceptionType::Range,
                       "Себестоимость не может быть отрицательной.");
  }
  validate_date(record.date);

  production_records.push_back(record);
  save_production_to_file();
  cout << "[OK] Запись производства добавлена." << endl;
  return 1;
}

int menu_add_sale() {
  cout << "\n=== ДОБАВИТЬ ЗАПИСЬ ПРОДАЖИ ===" << endl;

  SaleRecord record;
  cout << "Название товара: ";
  record.product_name = read_token_or_throw("Название товара");
  cout << "Количество: ";
  record.count = read_int_or_throw("Количество");
  cout << "Цена продажи за единицу: ";
  record.price = read_double_or_throw("Цена продажи за единицу");
  cout << "Дата (например 2026-05-31): ";
  record.date = read_token_or_throw("Дата");

  if (record.product_name.empty()) {
    throw AppException(ExceptionType::Input,
                       "Название товара не должно быть пустым.");
  }
  if (record.count <= 0) {
    throw AppException(ExceptionType::Range,
                       "Количество должно быть положительным.");
  }
  if (record.price < 0) {
    throw AppException(ExceptionType::Range,
                       "Цена не может быть отрицательной.");
  }
  validate_date(record.date);

  sale_records.push_back(record);
  save_sales_to_file();
  cout << "[OK] Запись продажи добавлена." << endl;
  return 1;
}

int menu_create_report() {
  ofstream report(REPORT_FILE);
  if (!report.is_open()) {
    throw AppException(ExceptionType::File,
                       "Не удалось создать файл отчета.");
  }

  int workers_count = 0;
  int suppliers_count = 0;
  int total_produced = 0;
  int total_sold = 0;
  double total_production_cost = 0.0;
  double total_sales_sum = 0.0;

  report << "ОТЧЕТ АВТОМАТИЗИРОВАННОЙ СИСТЕМЫ ЗАВОДА\n";
  report << "========================================\n\n";

  report << "СОТРУДНИКИ\n";
  report << "----------\n";
  for (auto *u : users) {
    if (worker *w = dynamic_cast<worker *>(u)) {
      ++workers_count;
      report << workers_count << ". " << w->Get_first_name() << ' '
             << w->Get_last_name() << ", возраст: " << w->Get_age()
             << ", логин: " << w->get_login()
             << ", должность: " << w->get_grade() << '\n';
    }
  }
  if (workers_count == 0) {
    report << "(нет сотрудников)\n";
  }

  report << "\nПОСТАВЩИКИ\n";
  report << "----------\n";
  for (auto *u : users) {
    if (supplier *s = dynamic_cast<supplier *>(u)) {
      ++suppliers_count;
      report << suppliers_count << ". " << s->Get_first_name() << ' '
             << s->Get_last_name() << ", возраст: " << s->Get_age()
             << ", логин: " << s->get_login()
             << ", тип продукции: " << s->get_product_type() << '\n';
    }
  }
  if (suppliers_count == 0) {
    report << "(нет поставщиков)\n";
  }

  report << "\nПРОИЗВОДСТВО\n";
  report << "------------\n";
  if (production_records.empty()) {
    report << "(нет записей производства)\n";
  } else {
    for (size_t i = 0; i < production_records.size(); ++i) {
      const ProductionRecord &record = production_records[i];
      double sum = record.count * record.cost;
      total_produced += record.count;
      total_production_cost += sum;
      report << i + 1 << ". " << record.product_name
             << ", количество: " << record.count
             << ", себестоимость за единицу: " << record.cost
             << ", сумма: " << sum << ", дата: " << record.date << '\n';
    }
  }

  report << "\nПРОДАЖИ\n";
  report << "-------\n";
  if (sale_records.empty()) {
    report << "(нет записей продаж)\n";
  } else {
    for (size_t i = 0; i < sale_records.size(); ++i) {
      const SaleRecord &record = sale_records[i];
      double sum = record.count * record.price;
      total_sold += record.count;
      total_sales_sum += sum;
      report << i + 1 << ". " << record.product_name
             << ", количество: " << record.count
             << ", цена за единицу: " << record.price << ", сумма: " << sum
             << ", дата: " << record.date << '\n';
    }
  }

  report << "\nИТОГИ\n";
  report << "-----\n";
  report << "Количество сотрудников: " << workers_count << '\n';
  report << "Количество поставщиков: " << suppliers_count << '\n';
  report << "Всего произведено единиц: " << total_produced << '\n';
  report << "Общая себестоимость производства: " << total_production_cost
         << '\n';
  report << "Всего продано единиц: " << total_sold << '\n';
  report << "Общая сумма продаж: " << total_sales_sum << '\n';
  report << "Прибыль: " << total_sales_sum - total_production_cost << '\n';

  cout << "[OK] Отчет создан: " << REPORT_FILE << endl;
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
    sup_login = read_token_or_throw("Логин поставщика");
    supplier *target = nullptr;
    for (auto *u : users) {
        if (supplier *s = dynamic_cast<supplier *>(u)) {
            if (s->get_login() == sup_login) {
                target = s;
                break;
            }
        }
    }
    if (!target) {
        throw AppException(ExceptionType::Logic,
                           "Поставщик с таким логином не найден.");
    }
    string pname, ptype;
    double price;
    int count;
    cout << "Название продукта: ";
    pname = read_token_or_throw("Название продукта");
    cout << "Тип продукта: ";
    ptype = read_token_or_throw("Тип продукта");
    cout << "Цена: ";
    price = read_double_or_throw("Цена");
    cout << "Количество: ";
    count = read_int_or_throw("Количество");
    if (pname.empty() || ptype.empty()) {
        throw AppException(ExceptionType::Input,
                           "Название и тип продукта не должны быть пустыми.");
    }
    if (price < 0) {
        throw AppException(ExceptionType::Range,
                           "Цена не может быть отрицательной.");
    }
    if (count <= 0) {
        throw AppException(ExceptionType::Range,
                           "Количество должно быть положительным.");
    }
    product p(pname, ptype, price, count);
    target->add_product(p);
    cout << "[OK] Товар добавлен поставщику." << endl;
    return 1;
}

}
