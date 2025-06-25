#ifndef REDBLACK_HPP
#define REDBLACK_HPP
#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/enum/NodeColor.hpp"
#include "../../interfaces/trees/rotatable/RotatableTree.hpp"
#include "contexts/RedBlack/DeletionContext.hpp"
#include "contexts/RedBlack/InsertionContext.hpp"
#include "contexts/RedBlack/RotationContext.hpp"
#include <iostream>
#include <queue>
#include <utility>

template <typename T, typename InsertionCtx = RBInsertionContext<T>,
          typename DeletionCtx = RBDeletionContext<T>,
          template <typename> class RotationCtx = RBRotationContext>
class RedBlack : public RotatableTree<T, RotationCtx> {
  using Base = RotatableTree<T, RotationCtx>;
  using Base::_rotate_left;
  using Base::_rotate_right;
  using KeyType = decltype(KeyExtractor<T>::getKey(std::declval<T>()));

public:
  RedBlack() {};
  RedBlack(T value) { m_root = new Node<T>(value); };
  RedBlack(Node<T> *root) : m_root(root) {};
  ~RedBlack() { clear(); };

  // Data Structure: methods
  void insert(T value) override { m_root = _insert(m_root, value); }

  void remove(T value) override {
    m_root = _remove(m_root, KeyExtractor<T>::getKey(value));
  }

  T successor(T value) override {
    Node<T> *node = _contains(m_root, KeyExtractor<T>::getKey(value));
    Node<T> *succ = _successor(node);
    return succ ? succ->key : T{};
  }

  T predecessor(T value) override {
    Node<T> *node = _contains(m_root, KeyExtractor<T>::getKey(value));
    Node<T> *pred = _predecessor(node);
    return pred ? pred->key : T{};
  }

  bool contains(T value) const override {
    return _contains(m_root, KeyExtractor<T>::getKey(value)) != nullptr;
  }

  // - maximum and minimum
  T minimum() override {
    if (isEmpty())
      throw std::runtime_error("Tree is empty");
    return _minimum(m_root)->key;
  };
  T maximum() override {
    if (isEmpty())
      throw std::runtime_error("Tree is empty");
    return _maximum(m_root)->key;
  };

  // - others datastructures methods.
  bool isEmpty() override { return !_size(m_root); };
  void clear() override { m_root = _clear(m_root); };

  // Tree methods
  int size() override { return _size(m_root); };
  int height() override { return _tree_height(m_root); };
  void BFS() override { _BFS(m_root); };
  Node<T> *getRoot() const override { return m_root; }
  Node<T> *&getRootRef() override { return m_root; }
  Node<T> *getNode(const T &value) override {
    KeyType key = KeyExtractor<T>::getKey(value);
    return _contains(m_root, key);
  }

private:
  Node<T> *m_root{nullptr};

  int _height(Node<T> *node) { return node ? node->height : 0; }

  int _tree_height(Node<T> *node) {
    if (!node)
      return 0;

    return 1 + greater_children_height(node);
  }

  int greater_children_height(Node<T> *node) {
    return std::max(_height(node->left), _height(node->right));
  }

  void _adjust_node_parent(Node<T> *new_node, Node<T> *parent) {
    new_node->parent = parent;
  }

  template <typename CaseEnum>
  void contextExecution(FixupContext<CaseEnum, T> &ctx) {}

  Node<T> *_insert(Node<T> *node, T value) {
    if (!m_root) {
      return m_root = new Node<T>(value, BLACK);
    }

    InsertionCtx ctx(new Node<T>(value), m_root);
    ctx.useCaseAction();
    ctx.fixupAction();

    return m_root;
  }

  Node<T> *_remove(Node<T> *node, const KeyType &key) {
    if (!node)
      return nullptr;

    Node<T> *nodeToDelete = _contains(m_root, key);
    if (!nodeToDelete)
      return m_root;

    Node<T> *actualNodeBeingDeleted = nodeToDelete;
    Node<T> *replacementNode = nullptr;
    NodeColor originalColor = actualNodeBeingDeleted->color;

    if (!nodeToDelete->left) {
      replacementNode = nodeToDelete->right;
      _transplant(nodeToDelete, nodeToDelete->right);
    } else if (!nodeToDelete->right) {
      replacementNode = nodeToDelete->left;
      _transplant(nodeToDelete, nodeToDelete->left);
    } else {
      actualNodeBeingDeleted = _minimum(nodeToDelete->right);
      originalColor = actualNodeBeingDeleted->color;
      replacementNode = actualNodeBeingDeleted->right;

      if (actualNodeBeingDeleted->parent == nodeToDelete) {
        if (replacementNode)
          replacementNode->parent = actualNodeBeingDeleted;
      } else {
        _transplant(actualNodeBeingDeleted, actualNodeBeingDeleted->right);
        actualNodeBeingDeleted->right = nodeToDelete->right;
        if (actualNodeBeingDeleted->right)
          actualNodeBeingDeleted->right->parent = actualNodeBeingDeleted;
      }

      _transplant(nodeToDelete, actualNodeBeingDeleted);
      actualNodeBeingDeleted->left = nodeToDelete->left;
      if (actualNodeBeingDeleted->left)
        actualNodeBeingDeleted->left->parent = actualNodeBeingDeleted;
      actualNodeBeingDeleted->color = nodeToDelete->color;
    }

    delete nodeToDelete;

    if (originalColor == BLACK && replacementNode) {
      _delete_fixup(replacementNode);
    }

    Node<T> *newRoot = m_root;
    if (newRoot) {
      while (newRoot->parent) {
        newRoot = newRoot->parent;
      }
    }

    return newRoot;
  }

  void _transplant(Node<T> *nodeToReplace, Node<T> *replacementNode) {
    if (!nodeToReplace->parent) {
      m_root = replacementNode;
    } else if (nodeToReplace == nodeToReplace->parent->left) {
      nodeToReplace->parent->left = replacementNode;
    } else {
      nodeToReplace->parent->right = replacementNode;
    }

    if (replacementNode) {
      replacementNode->parent = nodeToReplace->parent;
    }
  }

  void _delete_fixup(Node<T> *currentNode) {
    while (currentNode != m_root && currentNode &&
           currentNode->color == BLACK) {
      if (currentNode == currentNode->parent->left) {
        Node<T> *siblingNode = currentNode->parent->right;

        if (siblingNode && siblingNode->color == RED) {
          siblingNode->color = BLACK;
          currentNode->parent->color = RED;
          _rotate_left(currentNode->parent);
          siblingNode = currentNode->parent->right;
        }

        if (siblingNode &&
            (!siblingNode->left || siblingNode->left->color == BLACK) &&
            (!siblingNode->right || siblingNode->right->color == BLACK)) {
          siblingNode->color = RED;
          currentNode = currentNode->parent;
        } else if (siblingNode) {
          if (!siblingNode->right || siblingNode->right->color == BLACK) {
            if (siblingNode->left)
              siblingNode->left->color = BLACK;
            siblingNode->color = RED;
            _rotate_right(siblingNode);
            siblingNode = currentNode->parent->right;
          }

          siblingNode->color = currentNode->parent->color;
          currentNode->parent->color = BLACK;
          if (siblingNode->right)
            siblingNode->right->color = BLACK;
          _rotate_left(currentNode->parent);
          currentNode = m_root;
        } else {
          break;
        }
      } else {
        Node<T> *siblingNode = currentNode->parent->left;

        if (siblingNode && siblingNode->color == RED) {
          siblingNode->color = BLACK;
          currentNode->parent->color = RED;
          _rotate_right(currentNode->parent);
          siblingNode = currentNode->parent->left;
        }

        if (siblingNode &&
            (!siblingNode->right || siblingNode->right->color == BLACK) &&
            (!siblingNode->left || siblingNode->left->color == BLACK)) {
          siblingNode->color = RED;
          currentNode = currentNode->parent;
        } else if (siblingNode) {
          if (!siblingNode->left || siblingNode->left->color == BLACK) {
            if (siblingNode->right)
              siblingNode->right->color = BLACK;
            siblingNode->color = RED;
            _rotate_left(siblingNode);
            siblingNode = currentNode->parent->left;
          }

          siblingNode->color = currentNode->parent->color;
          currentNode->parent->color = BLACK;
          if (siblingNode->left)
            siblingNode->left->color = BLACK;
          _rotate_right(currentNode->parent);
          currentNode = m_root;
        } else {
          break;
        }
      }
    }

    if (currentNode)
      currentNode->color = BLACK;
  }

  Node<T> *_minimum(Node<T> *node) {
    if (!node->left)
      return node;
    else
      return _minimum(node->left);
  };

  Node<T> *_maximum(Node<T> *node) {
    if (!node->right)
      return node;
    else
      return _maximum(node->right);
  }

  Node<T> *_successor(Node<T> *root) {
    if (!root) {
      return nullptr;
    }

    Node<T> *successor = root;

    if (!successor->right) {
      return root;
    } else {
      successor = root->right;
      while (successor->left) {
        successor = successor->left;
      }
    }

    return successor;
  }

  Node<T> *_predecessor(Node<T> *root) {
    if (!root) {
      return nullptr;
    }

    Node<T> *predecessor = root;

    if (!predecessor->left) {
      return root;
    } else {
      predecessor = root->left;
      while (predecessor->right) {
        predecessor = predecessor->right;
      }
    }

    return predecessor;
  }

  Node<T> *_clear(Node<T> *node) {
    if (node != nullptr) {
      node->left = _clear(node->left);
      node->right = _clear(node->right);
      delete node;
      return nullptr;
    }
    return nullptr;
  }

  Node<T> *_contains(Node<T> *node, const KeyType &key) const {
    if (node == nullptr)
      return nullptr;

    KeyType nodeKey = KeyExtractor<T>::getKey(node->key);

    if (key == nodeKey)
      return node;

    if (key < nodeKey)
      return _contains(node->left, key);
    else
      return _contains(node->right, key);
  }

  void _BFS(Node<T> *node) {
    if (node == nullptr)
      return;

    std::queue<Node<T> *> nodeSequence;
    nodeSequence.push(node);

    while (!nodeSequence.empty()) {
      Node<T> *aux = nodeSequence.front();
      nodeSequence.pop();

      if (aux == m_root)
        std::cout << aux->key;
      else
        std::cout << " " << aux->key;

      if (aux->left)
        nodeSequence.push(aux->left);

      if (aux->right)
        nodeSequence.push(aux->right);
    }
    std::cout << std::endl;
  }

  int _size(Node<T> *node) {
    if (node == nullptr)
      return 0;

    return 1 + _size(node->left) + _size(node->right);
  }
};

#endif
