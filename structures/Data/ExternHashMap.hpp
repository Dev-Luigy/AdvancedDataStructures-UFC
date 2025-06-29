#ifndef EXTERN_HASHMAP_HPP
#define EXTERN_HASHMAP_HPP

#include "../../interfaces/core/DataStructure.hpp"
#include "../../interfaces/core/KeyExtractor.hpp"
#include "../../interfaces/core/Node.hpp"

#include <functional>
#include <iostream>
#include <queue>
#include <vector>

// reference: https://en.cppreference.com/w/cpp/types/decay.html

template <
    typename T,
    // we need to use decay_t because of vector<list>, then we get List type.
    typename Hash = std::hash<
        std::decay_t<decltype(KeyExtractor<T>::getKey(std::declval<T>()))>>>

class ExternHashMap : public DataStructure<T> {
  using KeyType = decltype(KeyExtractor<T>::getKey(std::declval<T>()));

public:
  ExternHashMap(size_t tableSize = 19, float loadFactor = 1.0)
      : m_table_size(get_next_prime(tableSize)), m_number_of_elements(0),
        m_max_load_factor(loadFactor) {
    m_table.resize(m_table_size);
  }

  void insert(T value) override { _insert(value); }

  void remove(T value) override { _remove(KeyExtractor<T>::getKey(value)); }

  bool contains(T k) const override {
    return _contains(KeyExtractor<T>::getKey(k));
  }

  bool isEmpty() override { return m_number_of_elements == 0; }

  void show() override {
    for (size_t i = 0; i < m_table_size; i++) {
      std::queue<T> temp = m_table[i];
      std::cout << "[" << i << "]: ";
      while (!temp.empty()) {
        std::cout << "(" << KeyExtractor<T>::getKey(temp.front()) << ") ";
        temp.pop();
      }
      std::cout << "\n";
    }
  }

  void clear() override { _clear(); }

  Node<T> *getNode(const T &value) override {
    size_t index = hash_key(KeyExtractor<T>::getKey(value));
    std::queue<T> &bucket = m_table[index];
    T *result = nullptr;
    std::queue<T> temp;

    while (!bucket.empty()) {
      T &element = bucket.front();
      if (KeyExtractor<T>::getKey(element) == KeyExtractor<T>::getKey(value))
        result = &element;
      temp.push(std::move(element));
      bucket.pop();
    }

    bucket = std::move(temp);

    if (!result)
      return nullptr;

    return new Node<T>(*result);
  }

private:
  std::vector<std::queue<T>> m_table;
  size_t m_table_size;
  size_t m_number_of_elements;
  float m_max_load_factor;
  Hash m_hash;

  size_t hash_key(const KeyType &key) const {
    return m_hash(key) % m_table_size;
  }

  void rehash(size_t new_size) {
    new_size = get_next_prime(new_size);
    std::vector<std::queue<T>> old_table = m_table;

    m_table.clear();
    m_table.resize(new_size);
    m_table_size = new_size;
    m_number_of_elements = 0;

    for (auto &bucket : old_table) {
      while (!bucket.empty()) {
        _insert(bucket.front());
        bucket.pop();
      }
    }
  }

  void _insert(const T &value) {
    if (load_factor() >= m_max_load_factor)
      rehash(2 * m_table_size);

    KeyType key = KeyExtractor<T>::getKey(value);
    size_t index = hash_key(key);

    std::queue<T> &bucket = m_table[index];
    bool found = false;
    std::queue<T> temp;
    while (!bucket.empty()) {
      T front = bucket.front();
      bucket.pop();
      if (KeyExtractor<T>::getKey(front) == key)
        found = true;
      temp.push(front);
    }
    bucket = temp;

    if (!found) {
      bucket.push(value);
      ++m_number_of_elements;
    }
  }

  bool _contains(const KeyType &key) const {
    size_t index = hash_key(key);
    std::queue<T> temp = m_table[index];
    while (!temp.empty()) {
      T element = temp.front();
      temp.pop();
      if (KeyExtractor<T>::getKey(element) == key)
        return true;
    }
    return false;
  }

  void _remove(const KeyType &key) {
    size_t index = hash_key(key);
    std::queue<T> &bucket = m_table[index];
    std::queue<T> temp;
    bool found = false;

    while (!bucket.empty()) {
      T element = bucket.front();
      bucket.pop();
      if (KeyExtractor<T>::getKey(element) == key) {
        found = true;
        continue;
      }
      temp.push(element);
    }

    bucket = temp;

    if (found)
      --m_number_of_elements;
  }

  void _clear() {
    for (auto &q : m_table)
      std::queue<T>().swap(q);
    m_number_of_elements = 0;
  }

  float load_factor() const {
    return static_cast<float>(m_number_of_elements) / m_table_size;
  }

  size_t get_next_prime(size_t x) const {
    if (x <= 2)
      return 3;
    if (x % 2 == 0)
      ++x;

    while (true) {
      bool is_prime = true;
      for (size_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
          is_prime = false;
          break;
        }
      }
      if (is_prime)
        return x;
      x += 2;
    }
  }
};

#endif // EXTERN_HASHMAP_HPP
