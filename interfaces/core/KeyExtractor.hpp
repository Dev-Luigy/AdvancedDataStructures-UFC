#ifndef KEYEXTRACTOR_HPP
#define KEYEXTRACTOR_HPP

#include <tuple>
#include <utility>

template <typename T> struct KeyExtractor {
  static const T &getKey(const T &value) { return value; }
  static T &getKey(T &value) { return value; }
};

template <typename K, typename V> struct KeyExtractor<std::pair<K, V>> {
  static const K &getKey(const std::pair<K, V> &pair) { return pair.first; }
  static K &getKey(std::pair<K, V> &pair) { return pair.first; }
};

template <typename K, typename... Rest>
struct KeyExtractor<std::tuple<K, Rest...>> {
  static const K &getKey(const std::tuple<K, Rest...> &tpl) {
    return std::get<0>(tpl);
  }
  static K &getKey(std::tuple<K, Rest...> &tpl) { return std::get<0>(tpl); }
};

#endif
