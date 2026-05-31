#pragma once

#include <exception>
#include <string>

enum class ExceptionType {
  Input,
  Range,
  File,
  Logic
};

class AppException : public std::exception {
private:
  ExceptionType type;
  std::string message;
  std::string full_message;

public:
  AppException(ExceptionType type, const std::string &message);

  ExceptionType get_type() const;
  const std::string &get_message() const;
  std::string get_type_name() const;
  const char *what() const noexcept override;
};
