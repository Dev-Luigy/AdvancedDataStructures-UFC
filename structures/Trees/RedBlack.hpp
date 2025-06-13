#ifndef REDBLACK_HPP
#define REDBLACK_HPP
#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/trees/rotatable/RotatableTree.hpp"
#include "contexts/RedBlack/InsertionContext.hpp"
#include <iostream>
#include <queue>

template <typename T> class RedBlack : public RotatableTree<T> {

public:
  RedBlack() {};
  RedBlack(T value) { m_root = new Node<T>(value); };
  RedBlack(Node<T> *root) : m_root(root) {};
  ~RedBlack() { clear(); };

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
  bool contains(T k) const override { return _contains(m_root, k) != nullptr; };
  bool isEmpty() override { return !_size(m_root); };
  void show() override { show(m_root, ""); };
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

    return 1 + _greater_children_height(node);
  }

  int _greater_children_height(Node<T> *node) {
    return std::max(_height(node->left), _height(node->right));
  }

  // RotatableTree methods:
  Node<T> *_rotate_left(Node<T> *node) override {
    Node<T> *u = node->right;
    node->right = u->left;
    u->left = node;

    return u;
  }

  Node<T> *_rotate_right(Node<T> *node) override {
    Node<T> *u = node->left;
    node->left = u->right;
    u->right = node;

    return u;
  }

  void show(Node<T> *node, std::string heranca) {
    if (node != nullptr && (node->left != nullptr || node->right != nullptr))
      show(node->right, heranca + "r");
    for (int i = 0; i < (int)heranca.size() - 1; i++)
      std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
    if (heranca != "")
      std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
    if (node == nullptr) {
      std::cout << "#" << std::endl;
      return;
    }
    std::cout << node->key << std::endl;
    if (node != nullptr && (node->left != nullptr || node->right != nullptr))
      show(node->left, heranca + "l");
  }

  void _adjust_node_family(Node<T> *new_node, Node<T> *old_node) {
    if (!new_node || !old_node)
      return;

    new_node->parent = old_node->parent;
    new_node->left = old_node->left;
    new_node->right = old_node->right;
  }

  void _adjust_node_parent(Node<T> *new_node, Node<T> *old_node) {
    if (!new_node || !old_node)
      return;

    new_node->parent = old_node;
  }

  Node<T> *_insert(Node<T> *node, T value) {
    if (!node)
      return new Node(value, RED);

    if (node->key == value) {
      return node;
    }

    if (value < node->key) {
      node->left = _insert(node->left, value);

      _adjust_node_parent(node->left, node);

    } else {
      node->right = _insert(node->right, value);

      _adjust_node_parent(node->right, node);
    }

    return _fixup_node(node);
  }

  Node<T> *_fixup_node(Node<T> *node) {
    InsertionContext<T> ctx(node);
    switch (ctx.getInsertionCase()) {
    case InsertionCase::ROOT:
      if (node == m_root)
        node->color = BLACK;
      // else node are children of root and are Red, no fixup needeed
      break;

    case InsertionCase::CASE1:

      break;

    case InsertionCase::CASE2A:
    case InsertionCase::CASE2B:
      break;

    case InsertionCase::CASE3A:
    case InsertionCase::CASE3B:
      break;
    }
  }

  Node<T> *_fixup_deletion(Node<T> *node) {}

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

    Node<T> *current = root;
    Node<T> *successor = nullptr;

    while (current && current->key != value) {
      if (value < current->key) {
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

    Node<T> *current = root;
    Node<T> *predecessor = nullptr;

    while (current && current->key != value) {
      if (value < current->key) {
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

  Node<T> *_remove(Node<T> *node, T value) {}

  Node<T> *_remove_node(Node<T> *node) {}

  Node<T> *_contains(Node<T> *node, int k) const {
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
