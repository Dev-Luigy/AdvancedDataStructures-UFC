#include "structures/Trees/RedBlack.hpp"

using namespace std;

int main() {
  RedBlack<int> tree;

  std::vector<int> numbers = {11, 2, 14, 1, 7, 5, 8, 15, 4};
  for (int i = 0; i < (int)numbers.size(); i++) {
    tree.insert(numbers[i]);
    tree.show();
  }

  for (int i = 0; i < (int)numbers.size(); i++) {
    std::cout << "Removendo: " << numbers[i] << std::endl;
    tree.remove(numbers[i]);
    tree.show();
  }
}
