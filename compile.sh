#!/usr/bin/env bash
# Compile all C++ source files in the project
set -e
# Gather all .cpp files in the current directory
SRC_FILES=$(ls *.cpp 2>/dev/null | tr '\n' ' ')
if [ -z "$SRC_FILES" ]; then
  echo "No .cpp source files found."
  exit 1
fi
# Compile with C++17 standard, include the current directory for headers
 g++ -std=c++17 $SRC_FILES -I. -o app
echo "Compilation succeeded: ./app"
# Optionally run the application
echo "Running app..."
./app