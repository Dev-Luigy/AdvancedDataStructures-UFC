#ifndef RED_BLACK_ROTATION_CONTEXT_HPP
#define RED_BLACK_ROTATION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include <stdexcept>

enum Direction { LEFT, RIGHT };

template <typename T> struct RotationContext {
  Direction dir;
  Node<T> *node{nullptr};
  Node<T> *children{nullptr};
  Node<T> *parent{nullptr};
  Node<T> *childrenOrphan{nullptr};
  Node<T> *m_root{nullptr};

  RotationContext(Node<T> *n, Node<T> *m_root, Direction dir)
      : dir(dir), node(n), children(dir == LEFT ? n->right : n->left),
        parent(n->parent),
        childrenOrphan(dir == LEFT ? children->left : children->right),
        m_root(m_root) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }
  }

  void rotate() {
    if (dir == LEFT) {
      node->right = childrenOrphan;
      children->left = node;

      node->parent = children;
      children->parent = parent;

    } else {
      node->left = childrenOrphan;
      children->right = node;

      node->parent = children;
      children->parent = parent;
    }

    if (node == m_root)
      m_root = children;
    else { // Have to adjust parent;
      if (node == parent->left) {
        parent->left = children;
      } else {
        parent->right = children;
      }
    }

    std::swap(children->color, node->color);
  }
};

#endif
