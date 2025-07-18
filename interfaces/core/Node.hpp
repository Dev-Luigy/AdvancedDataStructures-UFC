#ifndef NODE_HPP
#define NODE_HPP

#include "../enum/NodeColor.hpp"
#include <vector>
template <typename T> struct Node {
  T key;
  unsigned int height{1};
  Node<T> *left{nullptr};
  Node<T> *right{nullptr};
  Node<T> *parent{nullptr};
  NodeColor color{RED};
  bool leaf{true};

  // for binary
  Node(T key) : key(key) {};
  Node(T key, Node<T> *left) : key(key), left(left) {};
  Node(T key, Node<T> *left, Node<T> *right)
      : key(key), left(left), right(right) {};

  // for RedBlack
  Node(T key, NodeColor color) : key(key), color(color) {};
  Node(T key, Node<T> *parent, NodeColor color)
      : key(key), parent(parent), color(color) {};
  Node(T key, Node<T> *left, Node<T> *right, Node<T> *parent, NodeColor color)
      : key(key), left(left), right(right), parent(parent), color(color) {};

  // for AVL
  Node(T key, Node<T> *left, Node<T> *right, Node<T> *parent)
      : key(key), left(left), right(right), parent(parent) {};

  // destructor.
  ~Node() {};
};

// template <typename T> struct BNode {
//   Node<T> node{nullptr};
//   std::vector<BNode<T>> childrens{vector<BNode<T>>()};
// };
//
#endif // !NODE_HPP
