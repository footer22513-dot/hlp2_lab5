#!/bin/bash

# g++ -std=c++17 main.cpp auth.cpp func_module/func.cpp menu/CMenu.cpp menu/CMenuItem.cpp models/user/user.cpp models/product/product.cpp models/supplier/supplier.cpp models/worker/worker.cpp models/employee/employee.cpp my_vec/vec.cpp my_vec/account_vec.cpp -o app

# Компилируем проект
g++ -std=c++17 -I include \
  src/main.cpp \
  src/auth.cpp \
  src/func.cpp \
  src/CMenu.cpp \
  src/CMenuItem.cpp \
  src/worker.cpp \
  src/supplier.cpp \
  src/product.cpp \
  src/user.cpp \
  -o app

echo "Скомпилировано: app"
echo "Today is $(date). Запускаю app..."
./app
