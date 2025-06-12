#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include "../../models/Node.hpp"
#include "../../models/RotatableTree.hpp"
#include "utils/treeFunctions.cpp"
#include <functional>
#include <iostream>
#include <queue>

template <typename T> class AVLTree : public RotatableTree<T> {
  using callback = std::function<void(Node<T> *)>;

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
  bool contains(T k) const override { return _contains(m_root, k) != nullptr; };
  bool isEmpty() override { return !_size(m_root); };
  void show() override { show(m_root, ""); };
  void clear() override { m_root = _clear(m_root); };

  // Tree methods
  int size() override { return _size(m_root); };
  int height() override { return _tree_height(m_root); };
  void BFS() override { _BFS(m_root); };

  void pre_order(const callback &func = &printValues) const {
    _pre_order(m_root, func);
  };

  void reverse_pre_order(const callback &func = &printValues) const {
    _reverse_pre_order(m_root, func);
  }

  void post_order(const callback &func = &printValues) const {
    _post_order(m_root, func);
  };

  void in_order(const callback &func = &printValues) const {
    _in_order(m_root, func);
  };

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
  // AVL methods
  int _balance(Node<T> *node) {
    return _height(node->right) - _height(node->left);
  }

  Node<T> *_rotate_left(Node<T> *node) {
    Node<T> *u = node->right;
    node->right = u->left;
    u->left = node;

    node->height = 1 + _greater_children_height(node);
    u->height = 1 + _greater_children_height(u);

    return u;
  }

  Node<T> *_rotate_right(Node<T> *node) {
    Node<T> *u = node->left;
    node->left = u->right;
    u->right = node;

    node->height = 1 + _greater_children_height(node);
    u->height = 1 + _greater_children_height(u);

    return u;
  }

  Node<T> *_fixup_node(Node<T> *node, T key) {
    int bal = _balance(node);

    // Right heavy
    if (bal > 1) {
      if (key > node->right->key) {
        return _rotate_left(node);
      } else {
        node->right = _rotate_right(node->right);
        return _rotate_left(node);
      }
    }
    // Left heavy
    else if (bal < -1) {
      if (key < node->left->key) {
        return _rotate_right(node);
      } else {
        node->left = _rotate_left(node->left);
        return _rotate_right(node);
      }
    }

    node->height = 1 + _greater_children_height(node);
    return node;
  }

  Node<T> *_fixup_deletion(Node<T> *node) {
    int bal = _balance(node);

    if (bal > 1) {
      if (_balance(node->right) >= 0) {
        return _rotate_left(node);
      } else {
        node->right = _rotate_right(node);
        return _rotate_left(node);
      }
    }

    else if (bal < -1) {
      if (_balance(node->left) >= 0) {
        return _rotate_right(node);
      } else {
        node->left = _rotate_left(node->left);
        return _rotate_right(node);
      }
    }

    node->height = 1 + _greater_children_height(node);

    return node;
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

  Node<T> *_insert(Node<T> *node, T value) {
    if (node == nullptr)
      return new Node<T>(value);

    if (node->key == value)
      return node;
    if (node->key > value)
      node->left = _insert(node->left, value);
    else if (node->key < value)
      node->right = _insert(node->right, value);

    node = _fixup_node(node, value);

    return node;
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

  Node<T> *_remove(Node<T> *node, T value) {
    if (node == nullptr)
      return nullptr;

    if (value < node->key) {
      node->left = _remove(node->left, value);
    } else if (value > node->key) {
      node->right = _remove(node->right, value);
    } else {
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

  void _reverse_pre_order(Node<T> *node, const callback &func) const {
    if (node == nullptr)
      return;

    func(node);
    _reverse_pre_order(node->right, func);
    _reverse_pre_order(node->left, func);
  }

  void _pre_order(Node<T> *node, const callback &func) const {
    if (node == nullptr)
      return;

    func(node);
    _pre_order(node->left, func);
    _pre_order(node->right, func);
  }

  void _post_order(Node<T> *node, const callback &func) const {
    if (node == nullptr)
      return;

    _post_order(node->left, func);
    _post_order(node->right, func);
    func(node);
  }

  void _in_order(Node<T> *node, const callback &func) const {
    if (node == nullptr)
      return;

    _in_order(node->left, func);
    func(node);
    _in_order(node->right, func);
  }
};

#endif
