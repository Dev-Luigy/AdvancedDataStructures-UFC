#ifndef MAKE_STRUCTURY_HPP
#define MAKE_STRUCTURY_HPP

#include "../structures/Data/ExternHashMap.hpp"
#include "../structures/Data/OpenHashMap.hpp"
#include "../structures/Trees/AVLTree.hpp"
#include "../structures/Trees/RedBlack.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct IFreqDS {
  virtual ~IFreqDS() = default;
  virtual void insert(const std::pair<std::string, int> &p) = 0;
  virtual bool contains(const std::pair<std::string, int> &p) const = 0;
  virtual Node<std::pair<std::string, int>> *
  getNode(const std::pair<std::string, int> &p) = 0;
  virtual void show() = 0;
  virtual std::vector<std::pair<std::string, int>> getOrderedContent() = 0;
};

struct AVLWrapper : IFreqDS {
  AVLTree<std::pair<std::string, int>> tree;
  void insert(const std::pair<std::string, int> &p) override { tree.insert(p); }
  bool contains(const std::pair<std::string, int> &p) const override {
    return tree.contains(p);
  }
  Node<std::pair<std::string, int>> *
  getNode(const std::pair<std::string, int> &p) override {
    return tree.getNode(p);
  }
  void show() override { tree.show(); }
  std::vector<std::pair<std::string, int>> getOrderedContent() override {
    return tree.getOrderedContent();
  }
};

struct RBWrapper : IFreqDS {
  RedBlack<std::pair<std::string, int>> tree;
  void insert(const std::pair<std::string, int> &p) override { tree.insert(p); }
  bool contains(const std::pair<std::string, int> &p) const override {
    return tree.contains(p);
  }
  Node<std::pair<std::string, int>> *
  getNode(const std::pair<std::string, int> &p) override {
    return tree.getNode(p);
  }
  void show() override { tree.show(); }
  std::vector<std::pair<std::string, int>> getOrderedContent() override {
    return tree.getOrderedContent();
  }
};

struct OpenHashWrapper : IFreqDS {
  OpenHashMap<std::pair<std::string, int>> map;
  void insert(const std::pair<std::string, int> &p) override { map.insert(p); }
  bool contains(const std::pair<std::string, int> &p) const override {
    return map.contains(p);
  }
  Node<std::pair<std::string, int>> *
  getNode(const std::pair<std::string, int> &p) override {
    return map.getNode(p);
  }
  void show() override { map.show(); }
  std::vector<std::pair<std::string, int>> getOrderedContent() override {
    return map.getOrderedContent();
  }
};

struct ExternHashWrapper : IFreqDS {
  ExternHashMap<std::pair<std::string, int>> map;
  void insert(const std::pair<std::string, int> &p) override { map.insert(p); }
  bool contains(const std::pair<std::string, int> &p) const override {
    return map.contains(p);
  }
  Node<std::pair<std::string, int>> *
  getNode(const std::pair<std::string, int> &p) override {
    return map.getNode(p);
  }
  void show() override { map.show(); }
  std::vector<std::pair<std::string, int>> getOrderedContent() override {
    return map.getOrderedContent();
  }
};

std::unique_ptr<IFreqDS> createStructure(const std::string &type);

#endif
