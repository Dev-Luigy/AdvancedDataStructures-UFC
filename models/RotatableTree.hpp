#ifndef ROTATABLETREE_HPP
#define ROTATABLETREE_HPP

#include "Node.hpp"
#include "Tree.hpp"

template <typename T> class RotatableTree : public Tree<T> {
private:
  virtual Node<T> *_rotate_left(Node<T> *node) = 0;
  virtual Node<T> *_rotate_right(Node<T> *node) = 0;
};

#endif
