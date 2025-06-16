#include "structures/Trees/RedBlack.hpp"

using namespace std;

int main() {
  RedBlack<int> tree;
  RedBlack<int> tree1;

  std::vector<int> numbers = {11, 2, 14, 1, 7, 5, 8, 15, 4};
  for (int i = 0; i < (int)numbers.size(); i++) {
    tree.insert(numbers[i]);
    tree.show();
  }

  for (int i = 0; i < (int)numbers.size(); i++) {
    std::cout << "Sucessor de: " << numbers[i]
              << " é: " << tree.successor(numbers[i]) << std::endl;
    std::cout << "predecessor de: " << numbers[i]
              << " é: " << tree.predecessor(numbers[i]) << std::endl;
  }
}
