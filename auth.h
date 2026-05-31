#pragma once
#include "my_vec.h"
#include <string>

using namespace std;

namespace GNA {

struct Account {
  string login;
  string password;
  string role;
};

class AuthSystem {
private:
  my_vec<Account> accounts;

  void load_from_file(const string &filename);

public:
  AuthSystem(const string &filename = "accounts.txt");
  bool login(const string &log, const string &pass, string &role_out);
  void add_account(const string &log, const string &pass, const string &role);
};

}
