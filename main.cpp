#include "structures/Trees/RedBlack.hpp"

using namespace std;

int main() {
  RedBlack<int> tree;
  RedBlack<int> tree1;

  std::vector<int> numbers = {15, 10, 20, 8, 12, 25, 6, 11, 13, 22, 27};
  for (int i = 0; i < (int)numbers.size(); i++) {
    tree.insert(numbers[i]);
    tree.show();
  }
}
