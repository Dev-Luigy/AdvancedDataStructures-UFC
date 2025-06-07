#ifndef SET_HPP
#define SET_HPP
#include "../../models/Set.hpp"
#include "../../models/Tree.hpp"
#include "../Trees/AVLTree.hpp"

template <typename T, typename TreeType = AVLTree<T>>
class BTreeSet : public DataStructure<T>, public Set<T, Set<T, TreeType>> {
  static_assert(std::is_base_of<Tree<T>, TreeType>::value,
                "TreeType must be derived from Tree<T>");

public:
  BTreeSet() : _elements() {}
  ~BTreeSet() {};

  // Métodos da estrutura de dados
  void insert(T value) override { _elements.insert(value); };
  void remove(T value) override { _elements.remove(value); };
  bool contains(T k) const override { return _elements.contains(k); };
  bool isEmpty() override { return _elements.isEmpty(); };
  void show() override { _elements.show(); };
  void clear() override { _elements.clear(); };

  // Operadores de conjuntos
  BTreeSet<T, TreeType>
  operator+(const BTreeSet<T, TreeType> &otherSet) const override {
    BTreeSet<T, TreeType> result;
    _elements.in_order(
        [&result](const Node<T> *node) { result.insert(node->key); });
    otherSet._elements.in_order(
        [&result](const Node<T> *node) { result.insert(node->key); });
    return result;
  }

  BTreeSet<T, TreeType>
  operator-(const BTreeSet<T, TreeType> &otherSet) const override {
    BTreeSet<T, TreeType> result;
    _elements.in_order([&](const Node<T> *node) {
      if (!otherSet.contains(node->key)) {
        result.insert(node->key);
      }
    });
    return result;
  }

  BTreeSet<T, TreeType>
  operator&(const BTreeSet<T, TreeType> &otherSet) const override {
    BTreeSet<T, TreeType> result;
    _elements.in_order([&](const Node<T> *node) {
      if (otherSet.contains(node->key)) {
        result.insert(node->key);
      }
    });
    return result;
  }

  BTreeSet<T, TreeType>
  symmetricDifference(const BTreeSet<T, TreeType> &otherSet) const override {
    BTreeSet<T, TreeType> result;
    _elements.in_order([&](const Node<T> *node) {
      if (!otherSet.contains(node->key)) {
        result.insert(node->key);
      }
    });
    otherSet._elements.in_order([&](const Node<T> *node) {
      if (!this->contains(node->key)) {
        result.insert(node->key);
      }
    });
    return result;
  }

  bool operator==(const BTreeSet<T, TreeType> &otherSet) const override {
    std::vector<T> elems1, elems2;
    _elements.in_order(
        [&](const Node<T> *node) { elems1.push_back(node->key); });
    otherSet._elements.in_order(
        [&](const Node<T> *node) { elems2.push_back(node->key); });
    return elems1 == elems2;
  }

  bool isSubset(const BTreeSet<T, TreeType> &otherSet) const override {
    bool subset = true;
    _elements.in_order([&](const Node<T> *node) {
      if (!otherSet.contains(node->key)) {
        subset = false;
      }
    });
    return subset;
  }

private:
  TreeType _elements;
};

#endif
