#include "app_exception.h"
#include "auth.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace GNA {

static void validate_account_fields(const string &log, const string &pass,
                                    const string &role) {
  if (log.length() < 5) {
    throw AppException(ExceptionType::Input,
                       "Логин должен быть не менее 5 символов.");
  }
  if (pass.length() < 8) {
    throw AppException(ExceptionType::Input,
                       "Пароль должен быть не менее 8 символов.");
  }
  if (role.empty()) {
    throw AppException(ExceptionType::Input,
                       "Роль не должна быть пустой.");
  }
  if (role != "admin" && role != "worker" && role != "supplier") {
    throw AppException(ExceptionType::Input,
                       "Неизвестная роль аккаунта.");
  }
}

AuthSystem::AuthSystem(const string &filename) { load_from_file(filename); }

void AuthSystem::load_from_file(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    throw AppException(ExceptionType::File,
                       "Файл аккаунтов \"" + filename + "\" не найден.");
  }

  string login, password, role;
  while (file >> login >> password >> role) {
    try {
      validate_account_fields(login, password, role);
      accounts.push_back({login, password, role});
    } catch (const AppException &) {
      cout << "[!] Некорректный аккаунт пропущен: " << login << endl;
    }
  }

  cout << "[OK] Загружено аккаунтов: " << accounts.size() << endl;
}

bool AuthSystem::login(const string &log, const string &pass,
                       string &role_out) {
  for (auto &acc : accounts) {
    if (acc.login == log && acc.password == pass) {
      role_out = acc.role;
      return true;
    }
  }
  return false;
}

void AuthSystem::add_account(const string &log, const string &pass,
                             const string &role) {
  validate_account_fields(log, pass, role);
  accounts.push_back({log, pass, role});
}

}
