#ifndef REDBLACK_HPP
#define REDBLACK_HPP
#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/trees/rotatable/RotatableTree.hpp"
#include "contexts/RedBlack/DeletionContext.hpp"
#include "contexts/RedBlack/InsertionContext.hpp"
#include "contexts/RedBlack/RotationContext.hpp"
#include <iostream>
#include <queue>

template <typename T, typename InsertionCtx = InsertionContext<T>,
          typename DeletionCtx = DeletionContext<T>>
class RedBlack : public RotatableTree<T> {

public:
  RedBlack() {};
  RedBlack(T value) { m_root = new Node<T>(value); };
  RedBlack(Node<T> *root) : m_root(root) {};
  ~RedBlack() { clear(); };

  // Data Structure: methods
  void insert(T value) override {
    m_root = _insert(m_root, value);
    if (m_root)
      while (m_root->parent) { // because of rotations
        m_root = m_root->parent;
      }
  }
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

    Node<T> *node = _contains(m_root, value);
    Node<T> *succ = _successor(node);

    return succ ? succ->key : value;
  };
  T predecessor(T value) override {
    if (isEmpty()) {
      throw std::runtime_error("Tree is empty");
    }

    Node<T> *node = _contains(m_root, value);
    Node<T> *pred = _predecessor(node);

    return pred ? pred->key : value;
  };

  // - others datastructures methods.
  bool contains(T k) const override { return _contains(m_root, k) != nullptr; };
  bool isEmpty() override { return !_size(m_root); };
  void clear() override { m_root = _clear(m_root); };

  // Tree methods
  int size() override { return _size(m_root); };
  int height() override { return _tree_height(m_root); };
  void BFS() override { _BFS(m_root); };
  Node<T> *getRoot() const override { return m_root; }

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

  // RotatableTree methods:
  //
  Node<T> *_rotate_left(Node<T> *node) override {
    RotationContext<T> ctx(node, m_root, LEFT);
    ctx.rotate();

    return node;
  }

  Node<T> *_rotate_right(Node<T> *node) override {
    RotationContext<T> ctx(node, m_root, RIGHT);
    ctx.rotate();

    return node;
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
    ctx.fixupAction([this](Node<T> *n) { return this->_rotate_left(n); },
                    [this](Node<T> *n) { return this->_rotate_right(n); });

    return m_root;
  }

  Node<T> *_remove(Node<T> *node, T value) {
    if (!node)
      return nullptr;

    Node<T> *target = _contains(node, value);
    if (!target)
      return node;

    Node<T> *successor = _successor(target);

    DeletionCtx ctx(target, (successor != target ? successor : nullptr),
                    m_root);
    m_root = ctx.useCaseAction();

    while (ctx.DB && ctx.parent) {
      m_root = ctx.fixupAction(
          [this](Node<T> *n) { return this->_rotate_left(n); },
          [this](Node<T> *n) { return this->_rotate_right(n); });
      ctx.updateRelatives();
    }

    while (m_root && m_root->parent)
      m_root = m_root->parent;

    return m_root;
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

  Node<T> *_contains(Node<T> *node, const T &k) const {
    if (node == nullptr || node->key == k)
      return node;

    if (k < node->key)
      return _contains(node->left, k);
    else
      return _contains(node->right, k);
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
