#ifndef AVL_ROTATION_CONTEXT_HPP
#define AVL_ROTATION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/enum/RotationDirection.hpp"
#include "../../utils/treeUtils.cpp"
#include <stdexcept>

template <typename T> struct RotationContext {
  static Node<T> *rotate(Node<T> *node, Node<T> *&m_root, Direction dir) {
    if (!node)
      throw std::invalid_argument("Cannot rotate null node");

    Node<T> *parent = node->parent;
    Node<T> *children = (dir == LEFT) ? node->right : node->left;

    if (!children)
      throw std::logic_error("Cannot perform rotation: child is null");

    Node<T> *childrenOrphan = (dir == LEFT) ? children->left : children->right;

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

    node->height = 1 + greater_children_height(node);
    children->height = 1 + greater_children_height(children);

    return children;
  }
};

#endif
