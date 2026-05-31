#include "app_exception.h"

AppException::AppException(ExceptionType type, const std::string &message)
    : type(type), message(message) {
  full_message = "[" + get_type_name() + "] " + message;
}

ExceptionType AppException::get_type() const { return type; }

const std::string &AppException::get_message() const { return message; }

std::string AppException::get_type_name() const {
  switch (type) {
  case ExceptionType::Input:
    return "Ошибка ввода";
  case ExceptionType::Range:
    return "Ошибка диапазона";
  case ExceptionType::File:
    return "Ошибка файла";
  case ExceptionType::Logic:
    return "Логическая ошибка";
  }

  return "Ошибка";
}

const char *AppException::what() const noexcept { return full_message.c_str(); }
