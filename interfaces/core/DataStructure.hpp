
#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include "KeyExtractor.hpp"
#include "Node.hpp"
#include <string>
#include <vector>

template <typename T> class DataStructure {
public:
  virtual void insert(T value) = 0;
  virtual void remove(T value) = 0;
  virtual bool contains(T k) const = 0;
  virtual bool isEmpty() = 0;
  virtual void show() = 0;
  virtual void clear() = 0;
  virtual Node<T> *getNode(const T &value) = 0;
  virtual ~DataStructure() {}

  template <typename Structure>
  static void execute(Structure &structure,
                      const std::vector<std::string> &words) {
    using WordCount = std::pair<std::string, int>;

    for (const auto &word : words) {
      WordCount probe{word, 0};

      if (structure.contains(probe)) {
        auto *node = structure.getNode(probe);
        if (node)
          node->key.second += 1;
      } else {
        structure.insert({word, 1});
      }

      structure.show();
    }
  }

  template <typename Structure>
  static void execute(Structure &structure, const std::vector<int> &numbers) {
    for (const auto &number : numbers) {
      if (structure.contains(number)) {
        auto *node = structure.getNode(number);
        if (node)
          node->key++;
      } else {
        structure.insert(number);
      }

      structure.show();
    }
  }
};

#endif
