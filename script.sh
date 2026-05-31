#!/bin/bash

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
