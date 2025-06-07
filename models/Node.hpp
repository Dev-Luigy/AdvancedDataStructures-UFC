#ifndef NODE_HPP
#define NODE_HPP

#include "enum/NodeColor.hpp"
template <typename T> struct Node {
  T key;
  unsigned int height{1};
  Node<T> *left{nullptr};
  Node<T> *right{nullptr};
  Node<T> *parent{nullptr};
  NodeColor color{RED};

  Node(T key) : key(key) {};
  Node(T key, NodeColor color) : key(key), color(color) {};
  Node(T key, Node<T> *parent, NodeColor color)
      : key(key), parent(parent), color(color) {};
  Node(T key, Node<T> *left) : key(key), left(left) {};
  Node(T key, Node<T> *left, Node<T> *right)
      : key(key), left(left), right(right) {};

  Node(T key, Node<T> *left, Node<T> *right, Node<T> *parent)
      : key(key), left(left), right(right), parent(parent) {};
  Node(T key, Node<T> *left, Node<T> *right, Node<T> *parent, NodeColor color)
      : key(key), left(left), right(right), parent(parent), color(color) {};
  ~Node() {};
};

#endif // !NODE_HPP
