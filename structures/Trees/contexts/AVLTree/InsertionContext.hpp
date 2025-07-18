#ifndef AVL_INSERTION_CONTEXT_HPP
#define AVL_INSERTION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include "RotationContext.hpp"
#include "../../../../PerformanceTracker.hpp"
#include <stdexcept>

enum class AVLInsertionCase {
  // Root or root children.
  ROOT,

  NOFIXUP,
  // Easy case, just perform left rotate on top node.
  LINEARLEFT,
  // Easy case, just perform right rotate on top node.
  LINEARRIGHT,

  // Have to perform left rotate on mid node and right at top node.
  ZIGLEFTRIGHT,
  // Have to perform right rotate on mid node and left at top node.
  ZIGRIGHTLEFT,
};

template <typename T, typename RotationCtx = AVLRotationContext<T>>
struct AVLInsertionContext : public FixupContext<AVLInsertionCase, T> {
  int balance{0};
  T key{0};
  Node<T> *node{nullptr};
  Node<T> *&m_root{nullptr};

  AVLInsertionContext(Node<T> *node, Node<T> *&m_root, T key, int balance)
      : balance(balance), key(key), node(node), m_root(m_root) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }
  }

  bool isRootChildren() const {
    return node == m_root->left || node == m_root->right;
  }

  bool isLeaf(Node<T> *node) const {
    return node && !node->left && !node->right;
  }

  bool wasInsertedAtLeft() const { return node->left && key < node->left->key; }
  bool isLeftWeighted() const { return balance < -1; }

  bool wasInsertedAtRight() const {
    return node->right && key > node->right->key;
  }
  bool isRightWeighted() const { return balance > 1; }

  AVLInsertionCase getCase() const override {
    if (!m_root ||
        (isRootChildren() &&
         ((isLeaf(m_root->left) && node == m_root->left) ||
          (isLeaf(m_root->right) && node == m_root->right))) ||
        (node == m_root && isLeaf(m_root)))
      return AVLInsertionCase::ROOT;
    if (!isLeftWeighted() && !isRightWeighted())
      return AVLInsertionCase::NOFIXUP;
    if (isLeftWeighted())
      return wasInsertedAtLeft() ? AVLInsertionCase::LINEARLEFT
                                 : AVLInsertionCase::ZIGLEFTRIGHT;
    else {
      return wasInsertedAtRight() ? AVLInsertionCase::LINEARRIGHT
                                  : AVLInsertionCase::ZIGRIGHTLEFT;
    }
  }

  Node<T> *useCaseAction() override {
    if (!m_root) {
      m_root = node;
      return node;
    }

    Node<T> *parent{m_root};
    Node<T> *grandParent{nullptr};

    while (1) {
      if (key < parent->key) {
        if (parent->left) {
          grandParent = parent;
          parent = parent->left;
        } else {
          parent->left = node;
          break;
        }
      } else if (key > parent->key) {
        if (parent->right) {
          grandParent = parent;
          parent = parent->right;
        } else {
          parent->right = node;
          break;
        }
      } else {
        return parent;
      }
    }

    node->parent = parent;

    return node;
  }

  Node<T> *fixupAction() override {
    AVLInsertionCase case_type = getCase();
    
    // Track fixup operations
    if (case_type != AVLInsertionCase::ROOT && case_type != AVLInsertionCase::NOFIXUP) {
      PERF_TRACKER.incrementInsertionFixups();
    }

    switch (case_type) {
    case AVLInsertionCase::ROOT:
    case AVLInsertionCase::NOFIXUP:
      return node;

    case AVLInsertionCase::LINEARLEFT:
      return AVLRotationContext<T>::rotate(node, m_root, Direction::RIGHT);

    case AVLInsertionCase::LINEARRIGHT:
      return AVLRotationContext<T>::rotate(node, m_root, Direction::LEFT);

    case AVLInsertionCase::ZIGRIGHTLEFT:
      AVLRotationContext<T>::rotate(node->right, m_root, Direction::RIGHT);
      return AVLRotationContext<T>::rotate(node, m_root, Direction::LEFT);

    case AVLInsertionCase::ZIGLEFTRIGHT:
      AVLRotationContext<T>::rotate(node->left, m_root, Direction::LEFT);
      return AVLRotationContext<T>::rotate(node, m_root, Direction::RIGHT);
    }

    return node;
  }
};

#endif
