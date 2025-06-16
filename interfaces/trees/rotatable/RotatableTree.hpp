#ifndef ROTATABLETREE_HPP
#define ROTATABLETREE_HPP

#include "../../core/Node.hpp"
#include "../Tree.hpp"

template <typename T> class RotatableTree : public Tree<T> {
public:
  ~RotatableTree() = default;

private:
  virtual Node<T> *_rotate_left(Node<T> *node) = 0;
  virtual Node<T> *_rotate_right(Node<T> *node) = 0;
  virtual Node<T> *_fixup_node(Node<T> *node) = 0;
  virtual Node<T> *_fixup_deletion(Node<T> *node) = 0;
};

#endif
