#ifndef ROTATABLETREE_HPP
#define ROTATABLETREE_HPP

#include "../../core/Node.hpp"
#include "../Tree.hpp"

template <typename T> class RotatableTree : public Tree<T> {
  virtual Node<T> *rotate_left(Node<T> *node) = 0;
  virtual Node<T> *rotate_right(Node<T> *node) = 0;
  virtual Node<T> *fixup_node(Node<T> *node) = 0;
  virtual Node<T> *fixup_deletion(Node<T> *node) = 0;
  ~RotatableTree() = default;
};

#endif
