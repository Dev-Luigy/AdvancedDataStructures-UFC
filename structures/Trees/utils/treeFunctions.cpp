#ifndef TREEFUNCTIONS_HPP
#define TREEFUNCTIONS_HPP

#include "../../../interfaces/core/Node.hpp"
#include <iostream>
#include <stack>
#include <vector>

template <typename T> static void printValues(Node<T> *node) {
  if (node) {
    std::cout << node->key << " ";
  }
}
template <typename T>
static void insertNodesInStack(Node<T> *node, std::stack<Node<T> *> &stack) {
  if (node) {
    stack.push(node);
  }
}

template <typename T>
static void insertNodesInVector(Node<T> *node, std::vector<Node<T> *> &vector) {
  if (node) {
    vector.push_back(node);
  }
}

#endif
