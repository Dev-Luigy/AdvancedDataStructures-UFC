#ifndef ROTATABLETREE_HPP
#define ROTATABLETREE_HPP

#include "../../core/Node.hpp"
#include "../../enum/RotationDirection.hpp"
#include "../Tree.hpp"

template <typename T, template <typename> class RotationCtx>
class RotatableTree : public Tree<T> {
public:
  ~RotatableTree() = default;

protected:
  Node<T> *_rotate_left(Node<T> *node) {
    return RotationCtx<T>::rotate(node, this->getRootRef(), Direction::LEFT);
  }

  Node<T> *_rotate_right(Node<T> *node) {
    return RotationCtx<T>::rotate(node, this->getRootRef(), Direction::RIGHT);
  }
};

;

#endif
