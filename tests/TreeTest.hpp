#include <iostream>
#include <string>

template <typename C> class TreeTest {
private:
  C *tree{nullptr};

  void printResult(const std::string &testName, bool passed) const {
    std::cout << "[ " << testName << " \t ] \t"
              << (passed ? "✅ PASSED" : "❌ FAILED") << std::endl;
  }

public:
  TreeTest(C *tree) : tree(tree) {}

  void isEmpty(bool expected) const {
    bool result = tree->isEmpty();
    printResult("IsEmpty", result == expected);
  }

  void size(int expected) const {
    int result = tree->size();
    printResult("Size", result == expected);
  }

  void height(int expected) const {
    int result = tree->height();
    printResult("Height", result == expected);
  }

  void contains(typename C::value_type value, bool expected) const {
    bool result = tree->contains(value);
    printResult("Contains(" + std::to_string(value) + ")", result == expected);
  }

  void insertAndContains(typename C::value_type value) {
    tree->insert(value);
    bool result = tree->contains(value);
    printResult("Insert(" + std::to_string(value) + ")", result);
  }

  void removeAndContains(typename C::value_type value) {
    tree->remove(value);
    bool result = tree->contains(value);
    printResult("Remove(" + std::to_string(value) + ")", result);
  }

  void printBFS() const {
    std::cout << "BFS traversal: ";
    tree->BFS();
    std::cout << std::endl;
  }
};
