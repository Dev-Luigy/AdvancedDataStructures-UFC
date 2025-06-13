#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP
#include "../../interfaces/core/Node.hpp"
#include "../../interfaces/trees/Tree.hpp"
#include "utils/treeFunctions.cpp"
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stack>

template <typename T> class BinaryTree : public Tree<T> {
  using callback = std::function<void(Node<T> *)>;

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
  void show() override { _show(m_root, ""); };
  void clear() override { m_root = _clear(m_root); };

  // Tree methods
  int size() override { return _size(m_root); };
  int height() override { return _height(m_root); };
  void BFS() override { _BFS(m_root); };

  // Other Methods
  void orderTreeValues() { _orderTreeValues(); };

  void pre_order(const callback &func = &printValues) const {
    _pre_order(m_root, func);
  };
  void iteractive_pre_order(const callback &func = &printValues) const {
    _iteractive_pre_order(func);
  };
  void reverse_pre_order(const callback &func = &printValues) const {
    _reverse_pre_order(m_root, func);
  }

  void post_order(const callback &func = &printValues) const {
    _post_order(m_root, func);
  };
  void iteractive_post_order(const callback &func = &printValues) const {
    _iteractive_post_order(func);
  }

  void in_order(const callback &func = &printValues) const {
    _in_order(m_root, func);
  };
  void iteractive_in_order(const callback &func = &printValues) const {
    _iteractive_in_order(func);
  }

private:
  Node<T> *m_root{nullptr};

  void _show(Node<T> *node, std::string heranca) {
    if (node != nullptr && (node->left != nullptr || node->right != nullptr))
      _show(node->right, heranca + "r");
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
      _show(node->left, heranca + "l");
  }

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

    // 1. Encontra o nó com o valor desejado
    while (current && current->key != value) {
      if (value < current->key) {
        current = current->left;
      } else {
        predecessor =
            current; // Atualiza predecessor quando vamos para a direita
        current = current->right;
      }
    }

    // 2. Se o valor não existe na árvore
    if (!current) {
      throw std::runtime_error("Value not found in tree");
    }

    // 3. Se existe subárvore esquerda, predecessor é o máximo dela
    if (current->left) {
      Node<T> *temp = current->left;
      while (temp->right) {
        temp = temp->right;
      }
      return temp;
    }

    // 4. Caso contrário, retorna o predecessor salvo (que pode ser o pai ou
    // outro ancestral)
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

  void _reverse_pre_order(Node<T> *node, const callback &func) {
    if (node == nullptr)
      return;

    func(node);
    _reverse_pre_order(node->right, func);
    _reverse_pre_order(node->left, func);
  }

  void _pre_order(Node<T> *node, const callback &func) {
    if (node == nullptr)
      return;

    func(node);
    _pre_order(node->left, func);
    _pre_order(node->right, func);
  }

  void _iteractive_pre_order(const callback &func) {
    Node<T> *node = m_root;
    std::stack<Node<T> *> stack;

    while (node) {
      func(node);
      if (node->right)
        stack.push(node->right);

      if (node->left)
        node = node->left;
      else if (!stack.empty()) {
        node = stack.top();
        stack.pop();
      } else {
        return;
      }
    }
  }

  void _post_order(Node<T> *node, const callback &func) {
    if (node == nullptr)
      return;

    _post_order(node->left, func);
    _post_order(node->right, func);
    func(node);
  }

  void _iteractive_post_order(const callback &func) {
    if (!m_root)
      return;

    std::stack<Node<T> *> stack;

    auto collectNodes = [&stack](Node<T> *node) {
      insertNodesInStack(node, stack);
    };

    _reverse_pre_order(m_root, collectNodes);

    while (!stack.empty()) {
      Node<T> *node = stack.top();
      stack.pop();

      func(node);
    }
  }

  void _in_order(Node<T> *node, const callback &func) {
    if (node == nullptr)
      return;

    _in_order(node->left, func);
    func(node);
    _in_order(node->right, func);
  }

  void _iteractive_in_order(const callback &func) {
    Node<T> *node = m_root;
    std::stack<Node<T> *> stack;

    while (node || !stack.empty()) {
      if (node) {
        stack.push(node);
        node = node->left;
      } else {
        node = stack.top();
        stack.pop();
        func(node);
        node = node->right;
      }
    }
  }

  void _orderTreeValues() {
    if (!size())
      return;

    std::vector<Node<T> *> values;

    auto collectNodes = [&values](Node<T> *node) {
      insertNodesInVector(node, values);
    };

    _in_order(m_root, collectNodes);

    m_root = _ordenate(values, 0, values.size() - 1);
  }

  Node<T> *_ordenate(std::vector<Node<T> *> &vector, int ini, int end) {
    if (ini > end)
      return nullptr;

    int mid = std::floor((ini + end) / 2);
    Node<T> *node = vector[mid];

    node->left = _ordenate(vector, ini, mid - 1);
    node->right = _ordenate(vector, mid + 1, end);

    return node;
  }
};

#endif
