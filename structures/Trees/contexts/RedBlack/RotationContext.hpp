#ifndef RED_BLACK_ROTATION_CONTEXT_HPP
#define RED_BLACK_ROTATION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/enum/RotationDirection.hpp"
#include <stdexcept>
#include <utility>

template <typename T> struct RotationContext {
  static Node<T> *rotate(Node<T> *node, Node<T> *&m_root, Direction dir) {
    if (!node)
      throw std::invalid_argument("Cannot rotate null node");

    Node<T> *parent = node->parent;
    Node<T> *children = (dir == LEFT) ? node->right : node->left;

    if (!children)
      throw std::logic_error("Cannot rotate without child");

    Node<T> *childrenOrphan = (dir == LEFT) ? children->left : children->right;

    // Rotação
    if (dir == LEFT) {
      node->right = childrenOrphan;
      if (childrenOrphan)
        childrenOrphan->parent = node;

      children->left = node;
    } else {
      node->left = childrenOrphan;
      if (childrenOrphan)
        childrenOrphan->parent = node;

      children->right = node;
    }

    node->parent = children;
    children->parent = parent;

    if (node == m_root) {
      m_root = children;
    } else if (parent) {
      if (parent->left == node)
        parent->left = children;
      else
        parent->right = children;
    }

    std::swap(children->color, node->color);

    return children;
  }
};

#endif
