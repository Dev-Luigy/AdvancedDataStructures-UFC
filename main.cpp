#include "structures/Trees/AVLTree.hpp"
#include "structures/Trees/RedBlack.hpp"
#include <string>
#include <utility>
int main() {
  AVLTree<std::pair<std::string, int>> avlWords;
  RedBlack<std::pair<std::string, int>> rbWords;
  AVLTree<int> avlInt;
  RedBlack<int> rbInt;

  std::vector<int> valores = {30, 20, 40, 10, 25, 35, 50, 5, 15};
  std::vector<std::string> words = {
      "livro",    "texto",    "palavra",  "árvore",   "código",   "programa",
      "dados",    "função",   "variável", "classe",   "livro",    "texto",
      "programa", "dados",    "livro",    "função",   "código",   "variável",
      "classe",   "palavra",  "texto",    "árvore",   "dados",    "programa",
      "função",   "livro",    "variável", "código",   "classe",   "palavra",
      "programa", "dados",    "texto",    "livro",    "função",   "variável",
      "classe",   "palavra",  "código",   "árvore",   "dados",    "livro",
      "texto",    "programa", "função",   "variável", "classe",   "palavra",
      "código",   "livro",    "árvore",   "texto",    "dados",    "função",
      "programa", "classe",   "variável", "palavra",  "código",   "livro",
      "texto",    "função",   "programa", "dados",    "variável", "classe",
      "palavra",  "árvore",   "código",   "livro",    "texto",    "programa",
      "função",   "dados",    "classe",   "variável", "palavra",  "código",
      "livro",    "árvore",   "texto",    "dados",    "função",   "programa",
      "classe",   "variável", "palavra",  "código",   "livro",    "texto",
      "função",   "programa", "dados",    "variável", "classe",   "palavra",
      "árvore",   "código",   "livro",    "texto",    "programa", "função",
      "dados",    "classe",   "variável", "palavra",  "código",   "livro",
      "árvore",   "texto",    "dados",    "função",   "programa", "classe",
      "variável", "palavra",  "código",   "livro",    "texto",    "função",
      "programa", "dados",    "variável", "classe",   "palavra",  "árvore",
      "código",   "livro",    "texto",    "programa", "função",   "dados",
      "classe",   "variável", "palavra",  "código",   "livro",    "árvore",
      "texto",    "dados",    "função",   "programa", "classe",   "variável",
      "palavra",  "código",   "livro",    "texto",    "função",   "programa",
      "dados",    "variável", "classe",   "palavra",  "árvore",   "código",
      "livro",    "texto",    "programa", "função",   "dados",    "classe",
      "variável", "palavra",  "código",   "livro",    "árvore",   "texto",
      "dados",    "função",   "programa", "classe",   "variável", "palavra",
      "código",   "livro",    "texto",    "função",   "programa", "dados",
      "variável", "classe",   "palavra",  "árvore",   "código",   "livro",
      "texto",    "programa", "função",   "dados",    "classe",   "variável",
      "palavra",  "código",   "livro",    "árvore",   "texto",    "dados",
      "função",   "programa"};

  DataStructure<std::pair<std::string, int>>::execute(avlWords, words);
  DataStructure<std::pair<std::string, int>>::execute(rbWords, words);

  DataStructure<int>::execute(avlInt, valores);
  DataStructure<int>::execute(rbInt, valores);

  return 0;
}
