#include "auth.h"
#include <fstream>
#include <iostream>
using namespace std;

namespace GNA {

AuthSystem::AuthSystem(const string &filename) { load_from_file(filename); }

void AuthSystem::load_from_file(const string &filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "[!] Файл аккаунтов \"" << filename << "\" не найден." << endl;
    return;
  }

  string login, password, role;
  while (file >> login >> password >> role) {
    accounts.push_back({login, password, role});
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
  accounts.push_back({log, pass, role});
}

} // namespace GNA
