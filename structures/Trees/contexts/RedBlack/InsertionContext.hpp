#ifndef INSERTION_CONTEXT_HPP
#define INSERTION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include <stdexcept>

enum class InsertionCase {
  ROOT, // No fix-up required (node is root or no grandparent)

  // Uncle are RED.
  // Recolor parent and uncle to BLACK, grandparent to RED.
  // This maintains black height but may introduce red-red violation higher
  // up.
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
struct InsertionContext : public FixupContext<InsertionCase> {
  Node<T> *node{nullptr};
  Node<T> *parent{nullptr};
  Node<T> *grandparent{nullptr};
  Node<T> *uncle{nullptr};

  InsertionContext(Node<T> *n)
      : node(n), parent(n ? n->parent : nullptr),
        grandparent(parent ? parent->parent : nullptr), uncle(nullptr) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }

    if (grandparent) {
      uncle = (grandparent->left == parent) ? grandparent->right
                                            : grandparent->left;
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
  InsertionCase getCase() const override {
    if (!parent || !grandparent)
      return InsertionCase::ROOT;

    if (uncle && uncle->color == RED)
      return InsertionCase::CASE1;

    if (isParentLeftChild()) {
      return isNodeLeftChild() ? InsertionCase::CASE3A : InsertionCase::CASE2A;
    } else {
      return isNodeLeftChild() ? InsertionCase::CASE2B : InsertionCase::CASE3B;
    }
  }
};

#endif
