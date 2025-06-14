#ifndef TREE_HPP
#define TREE_HPP

#include "../core/DataStructure.hpp"
#include "../core/Node.hpp"
#include "../core/SortedNavigable.hpp"
#include <functional>

template <typename T>
class Tree : public DataStructure<T>, public SortedNavigable<T> {
public:
  using Callback = std::function<void(Node<T> *)>;

  virtual Node<T> *getRoot() const = 0;

  virtual int height() = 0;
  virtual int size() = 0;
  virtual void BFS() = 0;
  virtual ~Tree() {}

  // default implementation, so we don't need to redo.
  virtual void in_order(const Callback &func) const {
    inOrder(getRoot(), func);
  }

  virtual void pre_order(const Callback &func) const {
    preOrder(getRoot(), func);
  }

  virtual void post_order(const Callback &func) const {
    postOrder(getRoot(), func);
  }

  virtual void reverse_pre_order(const Callback &func) const {
    reversePreOrder(getRoot(), func);
  }

  // I removes all these code in all trees because are the same code to all
  // implementations of this interface, i'm thinking if now this is a
  // interface..
  static void preOrder(Node<T> *node, const Callback &func) {
    if (!node)
      return;
    func(node);
    preOrder(node->left, func);
    preOrder(node->right, func);
  }

  static void inOrder(Node<T> *node, const Callback &func) {
    if (!node)
      return;
    inOrder(node->left, func);
    func(node);
    inOrder(node->right, func);
  }

  static void postOrder(Node<T> *node, const Callback &func) {
    if (!node)
      return;
    postOrder(node->left, func);
    postOrder(node->right, func);
    func(node);
  }

  static void reversePreOrder(Node<T> *node, const Callback &func) {
    if (!node)
      return;
    func(node);
    reversePreOrder(node->right, func);
    reversePreOrder(node->left, func);
  }
};

#endif
