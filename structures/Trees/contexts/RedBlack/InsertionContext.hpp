#ifndef RED_BLACK_INSERTION_CONTEXT_HPP
#define RED_BLACK_INSERTION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include "RotationContext.hpp"
#include <stdexcept>

enum class RBInsertionCase {
  ROOT, // No fix-up required (node is root or no grandparent)

  // In this case, we are returning from recursion and we don't need fixups;
  // if parent color is different of node color.
  NOFIXUP,

  // Uncle are RED.
  // Recolor parent and uncle to BLACK, grandparent to RED.
  // This maintains black height but may introduce red-red violation higher
  // up.
  REDUNCLE,

  // Node is the right child of a left parent, uncle is BLACK.
  // Requires left rotation on parent, followed by right rotation on
  // grandparent.
  ZIGLEFTRIGHT,

  // Node is the left child of a right parent, uncle is BLACK.
  // Requires right rotation on parent, followed by left rotation on
  // grandparent.
  ZIGRIGHTLEFT,

  // Node is the left child of a left parent, uncle is BLACK.
  // Requires right rotation on grandparent.
  LINEARLEFT,

  // Node is the right child of a right parent, uncle is BLACK.
  // Requires left rotation on grandparent.
  LINEARRIGHT
};

template <typename T, typename RotationCtx = RBRotationContext<T>>
struct RBInsertionContext : public FixupContext<RBInsertionCase, T> {
  Node<T> *node{nullptr};
  Node<T> *parent{nullptr};
  Node<T> *grandparent{nullptr};
  Node<T> *uncle{nullptr};
  Node<T> *&m_root;

  RBInsertionContext(Node<T> *n, Node<T> *&m_root)
      : node(n), parent(n ? n->parent : nullptr),
        grandparent(parent ? parent->parent : nullptr), uncle(nullptr),
        m_root(m_root) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }

    if (grandparent) {
      uncle =
          grandparent->left == parent ? grandparent->right : grandparent->left;
    }
  }

  void updateRelatives() {
    parent = node ? node->parent : nullptr;
    grandparent = parent ? parent->parent : nullptr;
    uncle = (grandparent && parent)
                ? (grandparent->left == parent ? grandparent->right
                                               : grandparent->left)
                : nullptr;
  }

  bool isNodeLeftChildren() const { return parent && node->key < parent->key; }
  bool isParentLeftChildren() const {
    return parent && grandparent && parent->key < grandparent->key;
  }
  bool isParentBlack() const { return parent->color == BLACK; }
  bool isNodeColorDifFromParent() const { return node->color != parent->color; }
  bool hasParent() const { return !!parent; }
  bool hasGrandParent() const { return !!grandparent; }
  bool hasUncle() const { return !!uncle; }

  RBInsertionCase getCase() const override {
    if (!hasParent() || !hasGrandParent())
      return RBInsertionCase::ROOT;

    if (isParentBlack() || isNodeColorDifFromParent())
      return RBInsertionCase::NOFIXUP;

    if (uncle && uncle->color == RED)
      return RBInsertionCase::REDUNCLE;

    if (isParentLeftChildren()) {
      return isNodeLeftChildren() ? RBInsertionCase::LINEARLEFT
                                  : RBInsertionCase::ZIGLEFTRIGHT;
    } else {
      return isNodeLeftChildren() ? RBInsertionCase::ZIGRIGHTLEFT
                                  : RBInsertionCase::LINEARRIGHT;
    }
  }

  Node<T> *useCaseAction() override {
    Node<T> *current = m_root;

    while (current) {
      if (node->key == current->key) {
        return node;
      }

      Node<T> **children =
          (node->key < current->key) ? &current->left : &current->right;

      if (*children) {
        current = *children;
      } else {
        *children = node;
        node->parent = current;
        break;
      }
    }

    return node;
  }

  Node<T> *fixupAction() override {
    Node<T> *inserted_node = node;

    while (node) {
      updateRelatives();

      switch (getCase()) {
      case RBInsertionCase::ROOT:
        if (node == m_root)
          node->color = BLACK;
        break;

      case RBInsertionCase::NOFIXUP:
        break;

      case RBInsertionCase::REDUNCLE:
        parent->color = BLACK;
        uncle->color = BLACK;
        if (grandparent != m_root)
          grandparent->color = RED;
        break;

      case RBInsertionCase::ZIGLEFTRIGHT:
        RotationCtx::rotate(parent, m_root, Direction::LEFT);
        RotationCtx::rotate(grandparent, m_root, Direction::RIGHT);
        break;

      case RBInsertionCase::ZIGRIGHTLEFT:
        RotationCtx::rotate(parent, m_root, Direction::RIGHT);
        RotationCtx::rotate(grandparent, m_root, Direction::LEFT);
        break;

      case RBInsertionCase::LINEARLEFT:
        RotationCtx::rotate(grandparent, m_root, Direction::RIGHT);
        break;

      case RBInsertionCase::LINEARRIGHT:
        RotationCtx::rotate(grandparent, m_root, Direction::LEFT);
        break;
      }

      node = node->parent;
    }

    return inserted_node;
  }
};

#endif
