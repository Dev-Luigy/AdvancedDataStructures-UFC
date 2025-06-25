#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include "KeyExtractor.hpp"
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
  virtual ~DataStructure() {};
};

#endif
