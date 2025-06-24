#ifndef BHEAP_HPP
#define BHEAP_HPP
#include "../../interfaces/core/DataStructure.hpp"

// TODO: wee need to implement this DataStructure.
template <typename T> class BinaryHeap : public DataStructure<T> {

public:
  BinaryHeap() : _elements() {}
  ~BinaryHeap() {};

  void insert(T value) override { _elements.insert(value); };
  void remove(T value) override { _elements.remove(value); };
  T minimum() override { return _elements.minimum(); };
  T maximum() override { return _elements.maximum(); };
  T successor(T value) override { return _elements.successor(value); };
  T predecessor(T value) override { return _elements.predecessor(value); };
  bool contains(T k) const override { return _elements.contains(k); };
  bool isEmpty() override { return _elements.isEmpty(); };
  void show() override { _elements.show(); };
  void clear() override { _elements.clear(); };

private:
  T _elements;
};

#endif
