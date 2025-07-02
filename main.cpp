#include "structures/Data/ExternHashMap.hpp"
#include "structures/Data/OpenHashMap.hpp"
#include "structures/Trees/AVLTree.hpp"
#include "structures/Trees/RedBlack.hpp"
#include <string>
#include <utility>
int main() {
  AVLTree<std::pair<std::string, int>> avlWords;
  RedBlack<std::pair<std::string, int>> rbWords;
  AVLTree<int> avlInt;
  RedBlack<int> rbInt;
  OpenHashMap<std::pair<std::string, int>> openHash;
  ExternHashMap<std::pair<std::string, int>> externHash;

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

  for (const auto &word : words) {
    std::pair<std::string, int> wordPair = {word, 1};
    openHash.insert(wordPair);
    externHash.insert(wordPair);
  }

  for (int v : valores) {
    avlInt.insert(v);
    rbInt.insert(v);
  }

  std::cout << "AVL: Remove 30:" << std::endl;
  avlInt.remove(30);
  avlInt.show();

  std::cout << "RB: Remove 30:" << std::endl;
  rbInt.remove(30);
  rbInt.show();

  std::cout << "RB: Remove 'variável':" << std::endl;
  rbWords.remove({"variável", 0});
  rbWords.show();

  std::cout << "AVL: Remove 'variável':" << std::endl;
  avlWords.remove({"variável", 0});
  avlWords.show();

  std::cout << "OpenHashMap: contém 'função'? "
            << (openHash.contains({"função", 0}) ? "sim" : "não") << '\n';
  openHash.show();
  openHash.remove({"função", 0});
  std::cout << "OpenHashMap: removeu 'função'. Contém agora? "
            << (openHash.contains({"função", 0}) ? "sim" : "não") << '\n';
  openHash.show();

  std::cout << "ExternHashMap: contém 'livro'? "
            << (externHash.contains({"livro", 0}) ? "sim" : "não") << '\n';
  externHash.show();
  externHash.remove({"livro", 0});
  std::cout << "ExternHashMap: removeu 'livro'. Contém agora? "
            << (externHash.contains({"livro", 0}) ? "sim" : "não") << '\n';
  externHash.show();

  return 0;
}
