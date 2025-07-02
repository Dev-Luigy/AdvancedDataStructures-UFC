#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/trees/rotatable/RotatableTree.hpp"
#include "../../PerformanceTracker.hpp"
#include "contexts/AVLTree/DeletionContext.hpp"
#include "contexts/AVLTree/InsertionContext.hpp"
#include "contexts/AVLTree/RotationContext.hpp"
#include "utils/treeUtils.cpp"
#include <iostream>
#include <queue>
#include <utility>

template <typename T, typename InsertionCtx = AVLInsertionContext<T>,
          typename DeletionCtx = AVLDeletionContext<T>,
          template <typename> class RotationCtx = AVLRotationContext>
class AVLTree : public RotatableTree<T, RotationCtx> {
  using Base = RotatableTree<T, RotationCtx>;
  using Base::_rotate_left;
  using Base::_rotate_right;

  using KeyType = decltype(KeyExtractor<T>::getKey(std::declval<T>()));

public:
  AVLTree() {};
  AVLTree(T value) { m_root = new Node<T>(value); };
  AVLTree(Node<T> *root) : m_root(root) {};
  ~AVLTree() { clear(); };

  // Data Structure: methods
  void insert(T value) override { m_root = _insert(m_root, value); };
  void remove(T value) override { m_root = _remove(m_root, value); };

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

  // - successor and predecessor.
  T successor(T value) override {
    if (isEmpty()) {
      throw std::runtime_error("Tree is empty");
    }
    Node<T> *succ = _successor(m_root, value);
    if (succ == nullptr) {
      throw std::runtime_error("No successor exists for this value");
    }
    return succ->key;
  };
  T predecessor(T value) override {
    if (isEmpty()) {
      throw std::runtime_error("Tree is empty");
    }
    Node<T> *pred = _predecessor(m_root, value);
    if (pred == nullptr) {
      throw std::runtime_error("No predecessor exists for this value");
    }
    return pred->key;
  };

  // - others datastructures methods.
  bool contains(T k) const override {
    KeyType key = KeyExtractor<T>::getKey(k);
    return _contains(m_root, key) != nullptr;
  };

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
  unsigned int rotations{0};

  int _height(Node<T> *node) { return node ? node->height : 0; }

  int _tree_height(Node<T> *node) {
    if (!node)
      return 0;

    return 1 + greater_children_height(node);
  }

  // AVL methods
  int _balance(Node<T> *node) {
    return _height(node->right) - _height(node->left);
  }

  Node<T> *_fixup_deletion(Node<T> *node) {
    int bal = _balance(node);

    if (bal > 1) {
      rotations++;
      if (_balance(node->right) >= 0) {
        return _rotate_left(node);
      } else {
        node->right = _rotate_right(node->right);
        return _rotate_left(node);
      }
    }

    else if (bal < -1) {
      rotations++;
      if (_balance(node->left) <= 0) {
        return _rotate_right(node);
      } else {
        node->left = _rotate_left(node->left);
        return _rotate_right(node);
      }
    }

    node->height = 1 + greater_children_height(node);

    return node;
  }

  Node<T> *_insert(Node<T> *node, T value) {
    InsertionCtx ctx(new Node<T>(value), m_root, value, 0);
    Node<T> *inserted = ctx.useCaseAction();

    while (inserted) {
      InsertionCtx ctx(inserted, m_root, value, _balance(inserted));
      ctx.fixupAction();

      inserted->height = 1 + greater_children_height(inserted);
      inserted = inserted->parent;
    }

    return inserted ? inserted : m_root;
  }

  Node<T> *_contains(Node<T> *node, const KeyType &key) const {
    if (node == nullptr)
      return nullptr;

    PERF_TRACKER.incrementNodesVisited();
    PERF_TRACKER.incrementSearchDepth();
    
    KeyType nodeKey = KeyExtractor<T>::getKey(node->key);
    PERF_TRACKER.incrementComparisons();

    if (key == nodeKey)
      return node;

    if (key < nodeKey)
      return _contains(node->left, key);
    else
      return _contains(node->right, key);
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

  Node<T> *_successor(Node<T> *root, T value) {
    if (!root) {
      return nullptr;
    }

    KeyType valueKey = KeyExtractor<T>::getKey(value);
    Node<T> *current = root;
    Node<T> *successor = nullptr;

    while (current && KeyExtractor<T>::getKey(current->key) != valueKey) {
      if (valueKey < KeyExtractor<T>::getKey(current->key)) {
        successor = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }

    if (!current) {
      return nullptr; // Value not found
    }

    if (current->right) {
      Node<T> *temp = current->right;
      while (temp->left) {
        temp = temp->left;
      }
      return temp;
    }

    return successor;
  }

  Node<T> *_predecessor(Node<T> *root, T value) {
    if (!root) {
      return nullptr;
    }

    KeyType valueKey = KeyExtractor<T>::getKey(value);
    Node<T> *current = root;
    Node<T> *predecessor = nullptr;

    while (current && KeyExtractor<T>::getKey(current->key) != valueKey) {
      if (valueKey < KeyExtractor<T>::getKey(current->key)) {
        current = current->left;
      } else {
        predecessor = current;
        current = current->right;
      }
    }

    if (!current) {
      return nullptr; // Value not found
    }

    if (current->left) {
      Node<T> *temp = current->left;
      while (temp->right) {
        temp = temp->right;
      }
      return temp;
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

  Node<T> *_remove(Node<T> *node, T value) {
    if (node == nullptr)
      return nullptr;

    KeyType valueKey = KeyExtractor<T>::getKey(value);
    KeyType nodeKey = KeyExtractor<T>::getKey(node->key);

    if (valueKey < nodeKey) {
      node->left = _remove(node->left, value);
    } else if (valueKey > nodeKey) {
      node->right = _remove(node->right, value);
    } else {
      // Node found
      if (node->left == nullptr) {
        Node<T> *temp = node->right;
        delete node;
        return temp;
      } else if (node->right == nullptr) {
        Node<T> *temp = node->left;
        delete node;
        return temp;
      } else {
        Node<T> *temp = _minimum(node->right);
        node->key = temp->key;
        node->right = _remove(node->right, temp->key);
      }
    }
    return _fixup_deletion(node);
  }

  void _BFS(Node<T> *node) {
    if (node == nullptr)
      return;

    std::queue<Node<T> *> nodeSequence;
    nodeSequence.push(node);

    bool first = true;
    while (!nodeSequence.empty()) {
      Node<T> *aux = nodeSequence.front();
      nodeSequence.pop();

      if (first) {
        std::cout << aux->key;
        first = false;
      } else {
        std::cout << " " << aux->key;
      }

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
