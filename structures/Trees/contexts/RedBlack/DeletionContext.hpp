#ifndef DELETION_CONTEXT_HPP
#define DELETION_CONTEXT_HPP

#include "../../../../interfaces/core/Node.hpp"
#include "../../../../interfaces/trees/rotatable/FixupContext.hpp"
#include "RotationContext.hpp"
#include <stdexcept>

enum class DeletionCase {
  CLEAR,
  NOFIXUP,
  ROOT_CASE,

  // Node is Red and leaf - can remove without fixup
  CASE1A,

  // Node is Red with children - replace and remove child
  CASE1B,

  // Node is Black leaf - creates double black problem
  CASE2,

  // Sibling is Red - recolor and rotate to get black sibling
  CASE2A,

  // Sibling is Black with at least one Red child
  // Different subcases based on sibling and nephew positions
  CASE3A, // Sibling is left, left nephew is Red (LL case)
  CASE3B, // Sibling is right, right nephew is Red (RR case)
  CASE3C, // Sibling is left, right nephew is Red (LR case)
  CASE3D, // Sibling is right, left nephew is Red (RL case)

  // Sibling is Black with both children Black
  CASE4,
};

template <typename T>
struct DeletionContext : public FixupContext<DeletionCase, T> {
  Node<T> *node{nullptr};
  Node<T> *childrens[2]{nullptr, nullptr};
  Node<T> *parent{nullptr};
  Node<T> *sibling{nullptr};
  Node<T> *nephew[2]{nullptr, nullptr};
  Node<T> *successor{nullptr};
  Node<T> *m_root{nullptr};
  bool successorHandled{false};

  DeletionContext(Node<T> *n, Node<T> *successor, Node<T> *root)
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

  bool hasLeftNephew() const { return hasSibling() && nephew[LEFT]; }

  bool hasRightNephew() const { return hasSibling() && nephew[RIGHT]; }

  bool isLeftNephewRed() const {
    return hasLeftNephew() && nephew[LEFT]->color == RED;
  }

  bool isRightNephewRed() const {
    return hasRightNephew() && nephew[RIGHT]->color == RED;
  }

  bool isSiblingRed() const { return hasSibling() && sibling->color == RED; }

  bool isSiblingBlack() const {
    return !hasSibling() || sibling->color == BLACK;
  }

  bool isRed() const { return node && node->color == RED; }

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

  DeletionCase getCase() const override {
    if (!node) {
      return DeletionCase::CLEAR;
    }

    if (node == m_root) {
      return DeletionCase::ROOT_CASE;
    }

    // First handle successor replacement case
    if (successor && successor != node && !successorHandled) {
      return DeletionCase::NOFIXUP; // Will handle successor replacement
    }

    // Case 1A: Red leaf node - simple deletion
    if (isRed() && isLeaf()) {
      return DeletionCase::CASE1A;
    }

    // Case 1B: Red node with single child
    if (isRed() && !isLeaf() && !(childrens[LEFT] && childrens[RIGHT])) {
      return DeletionCase::CASE1B;
    }

    // For black nodes, we need to check for double black scenarios
    if (isBlack()) {
      // If it's a leaf or has only one child
      if (isLeaf() || !(childrens[LEFT] && childrens[RIGHT])) {
        if (!hasSibling()) {
          return DeletionCase::CASE2;
        }

        // Case 2A: Red sibling - convert to black sibling case
        if (isSiblingRed()) {
          return DeletionCase::CASE2A;
        }

        // Case 3: Black sibling with red children
        if (isSiblingBlack()) {
          bool leftNephewRed = isLeftNephewRed();
          bool rightNephewRed = isRightNephewRed();

          if (leftNephewRed || rightNephewRed) {
            if (isSiblingLeftChild()) {
              return leftNephewRed ? DeletionCase::CASE3A
                                   : DeletionCase::CASE3C;
            } else {
              return rightNephewRed ? DeletionCase::CASE3B
                                    : DeletionCase::CASE3D;
            }
          }

          // Case 4: Black sibling with black children
          return DeletionCase::CASE4;
        }
      }
    }

    return DeletionCase::NOFIXUP;
  }

  Node<T> *useCaseAction() override {
    // If we have a successor and haven't handled it yet, replace node's value
    // with successor
    if (successor && successor != node && !successorHandled) {
      node->key = successor->key;
      successorHandled = true;
      // Now we need to delete the successor
      node = successor;
      updateRelatives();
      return successor;
    }
    return node;
  }

  Node<T> *fixupAction(
      std::function<Node<T> *(Node<T> *)> rotateLeft = nullptr,
      std::function<Node<T> *(Node<T> *)> rotateRight = nullptr) override {

    Node<T> *current = node;
    bool fixupComplete = false;

    // Handle successor replacement first
    if (successor && successor != node && !successorHandled) {
      Node<T> *nodeToDelete = useCaseAction();
      current = nodeToDelete;
      updateRelatives();
    }

    while (current && current != m_root && !fixupComplete) {
      node = current;
      updateRelatives();

      switch (getCase()) {
      case DeletionCase::CLEAR:
        fixupComplete = true;
        break;

      case DeletionCase::ROOT_CASE:
        if (isLeaf()) {
          m_root = nullptr;
        } else if (!childrens[RIGHT]) {
          m_root = childrens[LEFT];
          m_root->parent = nullptr;
          m_root->color = BLACK;
        } else {
          node->key = successor->key;
          delete successor;
        }
        break;

      case DeletionCase::NOFIXUP:
      case DeletionCase::CASE1A:
        fixupComplete = true;
        break;

      case DeletionCase::CASE1B: {
        // Red node with one child - child must be black
        // Simply replace the node with its child
        fixupComplete = true;
        break;
      }

      case DeletionCase::CASE2A: {
        // Red sibling - recolor and rotate to get black sibling
        if (sibling) {
          sibling->color = BLACK;
        }
        if (parent) {
          parent->color = RED;
        }
        if (isSiblingRightChild()) {
          rotateLeft(parent);
        } else if (rotateRight) {
          rotateRight(parent);
        }
        updateRelatives();
        continue;
      }

      case DeletionCase::CASE3A: {
        // LL case: Left sibling, left nephew red
        if (hasLeftNephew()) {
          nephew[LEFT]->color = BLACK;
        }
        if (sibling && parent) {
          sibling->color = parent->color;
          parent->color = BLACK;
        }
        if (rotateRight) {
          rotateRight(parent);
        }
        fixupComplete = true;
        break;
      }

      case DeletionCase::CASE3B: {
        // RR case: Right sibling, right nephew red
        if (hasRightNephew()) {
          nephew[RIGHT]->color = BLACK;
        }
        if (sibling && parent) {
          sibling->color = parent->color;
          parent->color = BLACK;
        }
        if (rotateLeft) {
          rotateLeft(parent);
        }
        fixupComplete = true;
        break;
      }

      case DeletionCase::CASE3C: {
        // LR case: Left sibling, right nephew red
        if (hasRightNephew()) {
          nephew[RIGHT]->color = BLACK;
        }
        if (sibling) {
          sibling->color = RED;
        }
        if (rotateLeft) {
          rotateLeft(sibling);
        }
        updateRelatives();
        if (sibling && parent) {
          sibling->color = parent->color;
          parent->color = BLACK;
        }
        if (rotateRight) {
          rotateRight(parent);
        }
        fixupComplete = true;
        break;
      }

      case DeletionCase::CASE3D: {
        // RL case: Right sibling, left nephew red
        if (hasLeftNephew()) {
          nephew[LEFT]->color = BLACK;
        }
        if (sibling) {
          sibling->color = RED;
        }
        if (rotateRight) {
          rotateRight(sibling);
        }
        updateRelatives();
        if (sibling && parent) {
          sibling->color = parent->color;
          parent->color = BLACK;
        }
        if (rotateLeft) {
          rotateLeft(parent);
        }
        fixupComplete = true;
        break;
      }

      case DeletionCase::CASE4: {
        // Black sibling with black children - push blackness up
        if (hasSibling()) {
          sibling->color = RED;
        }
        if (parent && parent->color == RED) {
          parent->color = BLACK;
          fixupComplete = true;
        } else {
          // Parent becomes double black, continue up the tree
          current = parent;
        }
        break;
      }

      case DeletionCase::CASE2:
      default:
        // Handle remaining cases or stop
        fixupComplete = true;
        break;
      }
    }

    return m_root;
  }
};

#endif
