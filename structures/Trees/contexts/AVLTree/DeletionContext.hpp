#ifndef AVL_DELETION_CONTEXT_HPP
#define AVL_DELETION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/enum/RotationDirection.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include "../../../../PerformanceTracker.hpp"
#include <iostream>
#include <stdexcept>

enum class AVLDeletionCase {
  // Root is DB, remove DB.
  CASEROOT,

  // Node is red, node is leaf.
  CASE1A,

  // Node is Red,
  // Successor is Red.
  // only replace values and delete successor node.
  CASE1BR,

  // Node is Red,
  // Successor is Black.
  // Replace values, Successor becomes DB.
  CASE1BB,

  // Node is Red,
  // Successor doesn't exist
  // left children are black,
  // just replace node with left children.
  CASE1C,

  // Node is left
  // DB's sibling is red,
  // swap colors of parent and sibling.
  // rotate parent in left direction.
  CASE2L,

  // Node is right
  // DB's sibling is red,
  // swap colors of parent and sibling.
  // rotate parent in right direction.
  CASE2R,

  // DB's sibling is black, and both nephew are black.
  // we have a all black case.
  // Remove DB, make sibling red, add black to parent,
  CASE3,

  // Node is left,
  // DB's sibling is black,
  // left nephew are red,
  // swap nephew and sibling colors,
  // rotate sibling right.
  // apply CASE5LR.
  CASE4LL,

  // Node is right,
  // DB's sibling is black,
  // right nephew are red,
  // swap nephew and sibling colors,
  // rotate sibling left.
  // apply CASE5RL.
  CASE4RR,

  // Node is left,
  // DB's sibling is black,
  // right nephew are red,
  // swap colors of parent and sibling,
  // rotate parent to left,
  // change color of right nephew to black,
  // remove DB.
  CASE5LR,

  // Node is right,
  // DB's sibling is black,
  // left nephew are red,
  // swap colors of parent and sibling,
  // rotate parent to right,
  // change color of left nephew to black,
  // remove DB.
  CASE5RL,

};

template <typename T>
struct AVLDeletionContext : public FixupContext<AVLDeletionCase, T> {
  Node<T> *node{nullptr};
  Node<T> *childrens[2]{nullptr, nullptr};
  Node<T> *parent{nullptr};
  Node<T> *sibling{nullptr};
  Node<T> *nephew[2]{nullptr, nullptr};
  Node<T> *successor{nullptr};
  Node<T> *m_root{nullptr};
  bool DB{false};

  AVLDeletionContext(Node<T> *n, Node<T> *successor, Node<T> *root)
      : node(n), successor(successor), m_root(root) {
    if (!node) {
      throw std::invalid_argument("Cannot create context from nullptr node");
    }
    updateRelatives();
  }

  void updateRelatives() {
    parent = node ? node->parent : nullptr;

    if (parent) {
      sibling = (parent->left == node) ? parent->right : parent->left;
    } else {
      sibling = nullptr;
    }

    childrens[LEFT] = node ? node->left : nullptr;
    childrens[RIGHT] = node ? node->right : nullptr;

    if (sibling) {
      nephew[LEFT] = sibling->left;
      nephew[RIGHT] = sibling->right;
    } else {
      nephew[LEFT] = nullptr;
      nephew[RIGHT] = nullptr;
    }
  }

  bool isLeaf() const { return node && !node->left && !node->right; }

  bool isNodeLeftChild() const { return parent && node == parent->left; }

  bool hasSibling() const { return sibling != nullptr; }

  bool isSiblingRightChild() const { return isNodeLeftChild(); }

  bool isSiblingLeftChild() const { return !isNodeLeftChild() && parent; }

  bool isLeftNephewRed() const {
    return nephew[LEFT] && nephew[LEFT]->color == RED;
  }

  bool isLeftNephewBlack() const {
    return nephew[LEFT] && nephew[LEFT]->color == BLACK;
  }

  bool isRightNephewRed() const {
    return nephew[RIGHT] && nephew[RIGHT]->color == RED;
  }

  bool isRightNephewBlack() const {
    return nephew[RIGHT] && nephew[RIGHT]->color == BLACK;
  }

  bool isSiblingRed() const { return hasSibling() && sibling->color == RED; }

  bool isSiblingBlack() const {
    return !hasSibling() || sibling->color == BLACK;
  }

  bool isRed() const { return node && node->color == RED; }
  bool successorIsRed() const { return successor && successor->color == RED; };

  bool isBlack() const { return !node || node->color == BLACK; }

  bool hasRedChild() const {
    return (childrens[LEFT] && childrens[LEFT]->color == RED) ||
           (childrens[RIGHT] && childrens[RIGHT]->color == RED);
  }

  bool hasRelativeInDirection(Node<T> *targetNode, Direction dir) const {
    if (targetNode == this->node) {
      return childrens[dir] != nullptr;
    } else if (targetNode == sibling) {
      return nephew[dir] != nullptr;
    }
    return false;
  }

  AVLDeletionCase getCase() const override {
    if (node == m_root)
      return AVLDeletionCase::CASEROOT;

    if (isRed()) {
      if (isLeaf())
        return AVLDeletionCase::CASE1A;
      if (successor) {
        if (successorIsRed())
          return AVLDeletionCase::CASE1BR;
        else {
          return AVLDeletionCase::CASE1BB;
        }
      } else {
        return AVLDeletionCase::CASE1C;
      }
    } else if (isBlack()) {
      if (isSiblingBlack()) {
        if (isLeftNephewBlack() && isRightNephewBlack())
          return AVLDeletionCase::CASE3;

        if (isNodeLeftChild())
          return isLeftNephewRed() ? AVLDeletionCase::CASE4LL
                                   : AVLDeletionCase::CASE5LR;
        else
          return isRightNephewRed() ? AVLDeletionCase::CASE4RR
                                    : AVLDeletionCase::CASE5RL;
      } else {
        return isNodeLeftChild() ? AVLDeletionCase::CASE2L
                                 : AVLDeletionCase::CASE2R;
      }
    }

    std::cout << "here" << std::endl;
    return AVLDeletionCase::CASE1A;
  }

  Node<T> *useCaseAction() override {
    if (!node)
      return m_root;

    if (getCase() == AVLDeletionCase::CASE1A) {
      if (parent) {
        if (parent->left == node)
          parent->left = nullptr;
        else
          parent->right = nullptr;
      } else {
        m_root = nullptr;
      }
      delete node;
      return m_root;
    }

    return m_root;
  }

  Node<T> *fixupAction(
      std::function<Node<T> *(Node<T> *)> rotateLeft = nullptr,
      std::function<Node<T> *(Node<T> *)> rotateRight = nullptr) override {
    AVLDeletionCase case_type = getCase();
    
    // Track deletion fixups
    if (case_type != AVLDeletionCase::CASEROOT && case_type != AVLDeletionCase::CASE1A) {
      PERF_TRACKER.incrementDeletionFixups();
    }
    
    switch (case_type) {
    case AVLDeletionCase::CASEROOT:
      DB = false;
      break;

    case AVLDeletionCase::CASE1A:
      delete node;
      node = parent;
      return m_root;

    case AVLDeletionCase::CASE1BR:
      node->key = successor->key;
      delete successor;
      break;

    case AVLDeletionCase::CASE1BB:
      node->key = successor->key;

      break;
    }
    return m_root;
  }
};

#endif
