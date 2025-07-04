#ifndef OPENHASHMAP_HPP
#define OPENHASHMAP_HPP

#include "../../PerformanceTracker.hpp"
#include "../../interfaces/core/DataStructure.hpp"
#include "../../interfaces/core/KeyExtractor.hpp"
#include "../../interfaces/core/Node.hpp"

#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

// reference: https://en.cppreference.com/w/cpp/types/decay.html

template <
    typename T,
    // we need to use decay_t because of vector<list>, then we get List type.
    typename Hash = std::hash<
        std::decay_t<decltype(KeyExtractor<T>::getKey(std::declval<T>()))>>>
class OpenHashMap : public DataStructure<T> {
  using KeyType = decltype(KeyExtractor<T>::getKey(std::declval<T>()));

public:
  OpenHashMap(size_t tableSize = 19, float loadFactor = 0.75)
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
      if (m_table[i].has_value())
        std::cout << "[" << i
                  << "]: " << KeyExtractor<T>::getKey(m_table[i].value())
                  << "\n";
      else
        std::cout << "[" << i << "]: \n";
    }
  }

  void clear() override { _clear(); }

  Node<T> *getNode(const T &value) override {
    int idx = _find_slot(KeyExtractor<T>::getKey(value));
    if (idx == -1)
      return nullptr;
    return new Node<T>(m_table[idx].value());
  }

private:
  std::vector<std::optional<T>> m_table;
  size_t m_table_size;
  size_t m_number_of_elements;
  float m_max_load_factor;
  Hash m_hash;

  size_t hash_key(const KeyType &key) const {
    return m_hash(key) % m_table_size;
  }

  int _find_slot(const KeyType &key) const {
    size_t index = hash_key(key);
    size_t start = index;

    do {
      PERF_TRACKER.incrementComparisons();
      PERF_TRACKER.incrementNodesVisited();
      PERF_TRACKER.incrementSearchDepth();
      if (m_table[index].has_value() &&
          KeyExtractor<T>::getKey(m_table[index].value()) == key) {
        return static_cast<int>(index);
      }
      index = (index + 1) % m_table_size;
    } while (m_table[index].has_value() && index != start);

    return -1;
  }

  void rehash(size_t new_size) {
    new_size = get_next_prime(new_size);
    std::vector<std::optional<T>> old_table = m_table;
    m_table.clear();
    m_table.resize(new_size);
    m_table_size = new_size;
    m_number_of_elements = 0;

    for (const auto &entry : old_table) {
      if (entry.has_value()) {
        _insert(entry.value());
      }
    }
  }

  void _insert(const T &value) {
    if (load_factor() >= m_max_load_factor) {
      rehash(2 * m_table_size);
    }

    KeyType key = KeyExtractor<T>::getKey(value);
    size_t index = hash_key(key);
    size_t start = index;

    do {
      PERF_TRACKER.incrementComparisons();
      PERF_TRACKER.incrementNodesVisited();
      PERF_TRACKER.incrementSearchDepth();
      if (!m_table[index].has_value()) {
        m_table[index] = value;
        ++m_number_of_elements;
        return;
      } else if (KeyExtractor<T>::getKey(m_table[index].value()) == key) {
        return;
      }
      index = (index + 1) % m_table_size;
    } while (index != start);

    throw std::overflow_error("Hash table overflow");
  }

  bool _contains(const KeyType &key) const { return _find_slot(key) != -1; }

  void _remove(const KeyType &key) {
    size_t index = hash_key(key);
    size_t start = index;

    do {
      PERF_TRACKER.incrementComparisons();
      PERF_TRACKER.incrementNodesVisited();
      PERF_TRACKER.incrementSearchDepth();
      if (m_table[index].has_value() &&
          KeyExtractor<T>::getKey(m_table[index].value()) == key) {
        m_table[index].reset();
        --m_number_of_elements;
        rehash(m_table_size);
        return;
      }
      index = (index + 1) % m_table_size;
    } while (m_table[index].has_value() && index != start);
  }

  void _clear() {
    m_table.clear();
    m_table.resize(m_table_size);
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
      bool prime = true;
      for (size_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
          prime = false;
          break;
        }
      }
      if (prime)
        return x;
      x += 2;
    }
  }
};

#endif // OPENHASHMAP_HPP
