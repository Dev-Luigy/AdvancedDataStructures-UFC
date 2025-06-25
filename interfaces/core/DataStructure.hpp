#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include "KeyExtractor.hpp"
#include "Node.hpp"
#include <vector>
template <typename T> class DataStructure {
  using KeyType = decltype(KeyExtractor<T>::getKey(
      std::declval<T>())); // this will return type of getKey, if key is int,
                           // then keytype = int

public:
  virtual void insert(T value) = 0;
  virtual void remove(T value) = 0;
  virtual bool contains(T k) const = 0;
  virtual bool isEmpty() = 0;
  virtual void show() = 0;
  virtual void clear() = 0;
  virtual Node<T> *getNode(const T &value) = 0;
  virtual ~DataStructure() {};

  template <typename DataStructure>
  static void execute(DataStructure &structure,
                      const std::vector<std::string> &words) {
    using WordCount = std::pair<std::string, int>;

    for (const auto &word : words) {
      WordCount key{word, 0};

      if (structure.contains(key)) {
        auto *node = structure.getNode(key);
        if (node)
          node->key.second += 1;
      } else {
        structure.insert({word, 1});
      }

      structure.show();
    }
  }
};

#endif
