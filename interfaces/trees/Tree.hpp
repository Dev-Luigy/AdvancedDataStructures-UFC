#ifndef TREE_HPP
#define TREE_HPP
#include "../core/DataStructure.hpp"
#include "../core/SortedNavigable.hpp"

template <typename T>
class Tree : public DataStructure<T>, public SortedNavigable<T> {
public:
  virtual int height() = 0;
  virtual int size() = 0;
  virtual void BFS() = 0;
  virtual ~Tree() {};
};

#endif
