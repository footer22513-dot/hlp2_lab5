#!/usr/bin/env bash
set -e

g++ -std=c++17 -Wall -Wextra -I. \
  main.cpp \
  auth.cpp \
  func.cpp \
  CMenu.cpp \
  CMenuItem.cpp \
  worker.cpp \
  supplier.cpp \
  product.cpp \
  user.cpp \
  app_exception.cpp \
  -o app

echo "Compilation succeeded: ./app"
