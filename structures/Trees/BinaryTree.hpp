#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP
#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/trees/Tree.hpp"
#include <iostream>
#include <queue>

template <typename T> class BinaryTree : public Tree<T> {

public:
  BinaryTree() {};
  BinaryTree(T value) { m_root = new Node<T>(value); };
  BinaryTree(Node<T> *root) : m_root(root) {};
  ~BinaryTree() { clear(); };

  // Data Structure: methods
  void insert(T value) override { m_root = _insert(m_root, value); };
  void remove(T value) override { m_root = _remove(m_root, value); };
  T minimum() override { return _minimum(m_root)->key; };
  T maximum() override { return _maximum(m_root)->key; };
  T successor(T value) override { return _successor(m_root, value)->key; };
  T predecessor(T value) override { return _predecessor(m_root, value)->key; };
  bool contains(T k) override { return _contains(m_root, k) != nullptr; };
  bool isEmpty() override { return !size(); };
  void clear() override { m_root = _clear(m_root); };

  // Tree methods
  int size() override { return _size(m_root); };
  int height() override { return _height(m_root); };
  void BFS() override { _BFS(m_root); };
  Node<T> *getRoot() const override { return m_root; }

private:
  Node<T> *m_root{nullptr};

  Node<T> *_insert(Node<T> *node, T value) {
    if (node == nullptr)
      return new Node<T>(value);

    if (node->key > value)
      node->left = _insert(node->left, value);
    else if (node->key < value)
      node->right = _insert(node->right, value);

    return node;
  }

  Node<T> *_minimum(Node<T> *node) {
    if (!node) {
      std::cout << "Não existem valores na arvore." << std::endl;
      return nullptr;
    }

    if (!node->left)
      return node;
    else
      return _minimum(node->left);
  };

  Node<T> *_maximum(Node<T> *node) {
    if (!node) {
      std::cout << "Não existem valores na arvore." << std::endl;
      return nullptr;
    }

    if (!node->right)
      return node;
    else
      return _maximum(node->right);
  }

  Node<T> *_successor(Node<T> *root, T value) {
    if (!root) {
      throw std::runtime_error("Value not found in tree");
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
      throw std::runtime_error("Value not found in tree");
    }

    if (current->right) {
      Node<T> *temp = current->right;
      while (temp->left) {
        temp = temp->left;
      }
      return temp;
    }

    return predecessor;
  }

  Node<T> *_predecessor(Node<T> *root, T value) {
    if (!root) {
      throw std::runtime_error("Value not found in tree");
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
      throw std::runtime_error("Value not found in tree");
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

    if (node->key == value)
      return _remove_node(node);

    if (node->key < value) {
      node->left = _remove(node->left, value);
    } else if (node->key > value)
      node->right = _remove(node->right, value);

    return node;
  }

  Node<T> *_remove_node(Node<T> *node) {
    if (node->right == nullptr)
      return node->left;

    Node<T> *parent, *q;
    parent = node;
    q = parent->right;

    while (q->left != nullptr) {
      parent = q;
      q = q->left;
    }

    if (parent != node) {
      parent->left = q->left;
      q->right = node->right;
    }
    q->left = node->left;

    delete node;
    return q;
  }

  Node<T> *_contains(Node<T> *node, int k) {
    if (node == nullptr || node->key == k)
      return node;

    if (k < node->key)
      return _contains(node->left, k);
    else
      return _contains(node->right, k);
  }

  Node<T> *_find_parent(Node<T> *node, int k) {
    if (!node)
      return nullptr;

    Node<T> *parent = nullptr;
    Node<T> *current = node;

    while (current && current->key != k) {
      parent = current;
      if (k < current->key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    return (current != nullptr) ? parent : nullptr;
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

  int _height(Node<T> *node) {
    if (node == nullptr)
      return 0;

    return 1 + std::max(_height(node->left), _height(node->right));
  }
};

#endif
