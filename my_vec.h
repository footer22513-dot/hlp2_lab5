#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>

// Шаблонный итератор
template <typename T> class vec_iterator {
private:
  T *m_ptr;

public:
  vec_iterator(T *ptr) : m_ptr(ptr) {}

  T &operator*() const { return *m_ptr; }
  T *operator->() const { return m_ptr; }

  vec_iterator &operator++() {
    ++m_ptr;
    return *this;
  }

  vec_iterator operator++(int) {
    vec_iterator tmp = *this;
    ++m_ptr;
    return tmp;
  }

  bool operator==(const vec_iterator &other) const {
    return m_ptr == other.m_ptr;
  }

  bool operator!=(const vec_iterator &other) const {
    return m_ptr != other.m_ptr;
  }
};

// Шаблонный вектор
template <typename T> class my_vec {
private:
  T *m_data;
  size_t m_size;
  size_t m_capacity;

  void resize() {
    m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
    T *newData = new T[m_capacity];
    for (size_t i = 0; i < m_size; i++) {
      newData[i] = m_data[i];
    }
    delete[] m_data;
    m_data = newData;
  }

public:
  my_vec() : m_data(nullptr), m_size(0), m_capacity(0) {}

  ~my_vec() { delete[] m_data; }

  my_vec(const my_vec &) = delete;
  my_vec &operator=(const my_vec &) = delete;

  size_t size() const { return m_size; }
  bool empty() const { return m_size == 0; }
  size_t capacity() const { return m_capacity; }

  void push_back(const T &value) {
    if (m_size >= m_capacity) {
      resize();
    }
    m_data[m_size++] = value;
  }

  void pop_back() {
    if (m_size > 0) {
      --m_size;
    }
  }

  void erase(size_t index) {
    if (index >= m_size) {
      throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i + 1 < m_size; ++i) {
      m_data[i] = (m_data[i + 1]);
    }
    --m_size;
  }

  void clear() { m_size = 0; }

  T &operator[](size_t i) { return m_data[i]; }

  const T &operator[](size_t i) const { return m_data[i]; }

  T &at(size_t i) {
    if (i >= m_size) {
      throw std::out_of_range("Index out of range");
    }
    return m_data[i];
  }

  const T &at(size_t i) const {
    if (i >= m_size) {
      throw std::out_of_range("Index out of range");
    }
    return m_data[i];
  }

  T &back() { return m_data[m_size - 1]; }

  const T &back() const { return m_data[m_size - 1]; }

  T &front() { return m_data[0]; }

  const T &front() const { return m_data[0]; }

  T *data() { return m_data; }

  const T *data() const { return m_data; }

  vec_iterator<T> begin() { return vec_iterator<T>(m_data); }

  vec_iterator<T> end() { return vec_iterator<T>(m_data + m_size); }
};
