#ifndef AVL_INSERTION_CONTEXT_HPP
#define AVL_INSERTION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include "../../utils/treeUtils.cpp"
#include <functional>
#include <stdexcept>

enum class InsertionCase {
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

template <typename T>
struct InsertionContext : public FixupContext<InsertionCase, T> {
  int balance{0};
  T key{0};
  Node<T> *node{nullptr};
  Node<T> *&m_root{nullptr};

  InsertionContext(Node<T> *node, Node<T> *&m_root, T key, int balance)
      : balance(balance), key(key), node(node), m_root(m_root) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }
  }

  bool isRootChildren() const {
    return node == m_root->left || node == m_root->right;
  }

  bool isLeaf(Node<T> *node) const { return !node->left && !node->right; }

  bool wasInsertedAtLeft() const { return node->left && key < node->left->key; }
  bool isLeftWeighted() const { return balance < -1; }

  bool wasInsertedAtRight() const {
    return node->right && key > node->right->key;
  }
  bool isRightWeighted() const { return balance > 1; }

  InsertionCase getCase() const override {
    if (!m_root ||
        (isRootChildren() &&
         ((isLeaf(m_root->left) && node == m_root->left) ||
          (isLeaf(m_root->right) && node == m_root->right))) ||
        (node == m_root && isLeaf(m_root)))
      return InsertionCase::ROOT;
    if (!isLeftWeighted() && !isRightWeighted())
      return InsertionCase::NOFIXUP;
    if (isLeftWeighted())
      return wasInsertedAtLeft() ? InsertionCase::LINEARLEFT
                                 : InsertionCase::ZIGLEFTRIGHT;
    else {
      return wasInsertedAtRight() ? InsertionCase::LINEARRIGHT
                                  : InsertionCase::ZIGRIGHTLEFT;
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
          parent->parent = grandParent;
        } else {
          parent->left = node;
          break;
        }
      } else if (key > parent->key) {
        if (parent->right) {
          grandParent = parent;
          parent = parent->right;
          parent->parent = grandParent;
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

  Node<T> *
  fixupAction(std::function<Node<T> *(Node<T> *)> rotateLeft,
              std::function<Node<T> *(Node<T> *)> rotateRight) override {

    switch (getCase()) {
    case InsertionCase::ROOT:
    case InsertionCase::NOFIXUP:
      return node;

    case InsertionCase::LINEARLEFT:
      return rotateRight(node);

    case InsertionCase::LINEARRIGHT:
      return rotateLeft(node);

    case InsertionCase::ZIGRIGHTLEFT:
      rotateRight(node->right);
      return rotateLeft(node);

    case InsertionCase::ZIGLEFTRIGHT:
      rotateLeft(node->left);
      return rotateRight(node);
    }

    return node;
  }
};

#endif
