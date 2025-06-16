#ifndef DELETION_CONTEXT_HPP
#define DELETION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include <stdexcept>

enum class DeletionCase {
  ROOT, // Now our m_root was null.

  //
  CASE1,

  // Node is the right child of a left parent, uncle is BLACK.
  // Requires left rotation on parent, followed by right rotation on
  // grandparent.
  CASE2A,

  // Node is the left child of a right parent, uncle is BLACK.
  // Requires right rotation on parent, followed by left rotation on
  // grandparent.
  CASE2B,

  // Node is the left child of a left parent, uncle is BLACK.
  // Requires right rotation on grandparent.
  CASE3A,

  // Node is the right child of a right parent, uncle is BLACK.
  // Requires left rotation on grandparent.
  CASE3B
};

template <typename T>
struct DeletionContext : public FixupContext<DeletionCase> {
  Node<T> *node;
  Node<T> *parent;
  Node<T> *grandparent;
  Node<T> *brother;

  DeletionContext(Node<T> *n)
      : node(n), parent(n ? n->parent : nullptr),
        grandparent(parent ? parent->parent : nullptr), brother(nullptr) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }

    if (parent) {
      brother = (parent->left == node) ? parent->right : parent->left;
    }
  }

  bool isNodeLeftChild() const { return parent && node->key < parent->key; }

  bool isParentLeftChild() const {
    return parent && grandparent && parent->key < grandparent->key;
  }

  // If the node has no parent or grandparent, it is the root
  // or a child directly under the root (in a tree of height 1).
  // In both cases, no fix-up is required.
  // For all other cases, grandparent, parent, and uncle are guaranteed to
  // exist.
  DeletionCase getDeletionCase() const {
    if (!parent || !grandparent)
      return DeletionCase::ROOT;

    if (brother && brother->color == RED)
      return DeletionCase::CASE1;

    if (isParentLeftChild()) {
      return isNodeLeftChild() ? DeletionCase::CASE3A : DeletionCase::CASE2A;
    } else {
      return isNodeLeftChild() ? DeletionCase::CASE2B : DeletionCase::CASE3B;
    }
  }
};

#endif
