#include <iostream>
#include <string>

#include <unicode/normalizer2.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "factory/makeStructury.hpp"

std::string normalizeAndLower(const std::string &input) {
  UErrorCode status = U_ZERO_ERROR;
  const icu::Normalizer2 *normalizer = icu::Normalizer2::getNFCInstance(status);
  if (U_FAILURE(status))
    return input;
  icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(input);
  icu::UnicodeString normalized = normalizer->normalize(ustr, status);
  if (U_FAILURE(status))
    return input;
  normalized.toLower();
  std::string result;
  normalized.toUTF8String(result);
  return result;
}

template <typename DataStructure>
void loadFrequencies(DataStructure &ds,
                     const std::unordered_map<std::string, int> &freqs) {
  for (const auto &[word, count] : freqs) {
    ds.insert({word, count});
  }
}

int main(int argc, char *argv[]) {
  // AVLTree<std::pair<std::string, int>> avlWords;
  // RedBlack<std::pair<std::string, int>> rbWords;
  // AVLTree<int> avlInt;
  // RedBlack<int> rbInt;
  // OpenHashMap<std::pair<std::string, int>> openHash;
  // ExternHashMap<std::pair<std::string, int>> externHash;
  //
  // std::vector<int> valores = {30, 20, 40, 10, 25, 35, 50, 5, 15};
  // std::vector<std::string> words = {
  //     "livro",    "texto",    "palavra",  "árvore",   "código",   "programa",
  //     "dados",    "função",   "variável", "classe",   "livro",    "texto",
  //     "programa", "dados",    "livro",    "função",   "código",   "variável",
  //     "classe",   "palavra",  "texto",    "árvore",   "dados",    "programa",
  //     "função",   "livro",    "variável", "código",   "classe",   "palavra",
  //     "programa", "dados",    "texto",    "livro",    "função",   "variável",
  //     "classe",   "palavra",  "código",   "árvore",   "dados",    "livro",
  //     "texto",    "programa", "função",   "variável", "classe",   "palavra",
  //     "código",   "livro",    "árvore",   "texto",    "dados",    "função",
  //     "programa", "classe",   "variável", "palavra",  "código",   "livro",
  //     "texto",    "função",   "programa", "dados",    "variável", "classe",
  //     "palavra",  "árvore",   "código",   "livro",    "texto",    "programa",
  //     "função",   "dados",    "classe",   "variável", "palavra",  "código",
  //     "livro",    "árvore",   "texto",    "dados",    "função",   "programa",
  //     "classe",   "variável", "palavra",  "código",   "livro",    "texto",
  //     "função",   "programa", "dados",    "variável", "classe",   "palavra",
  //     "árvore",   "código",   "livro",    "texto",    "programa", "função",
  //     "dados",    "classe",   "variável", "palavra",  "código",   "livro",
  //     "árvore",   "texto",    "dados",    "função",   "programa", "classe",
  //     "variável", "palavra",  "código",   "livro",    "texto",    "função",
  //     "programa", "dados",    "variável", "classe",   "palavra",  "árvore",
  //     "código",   "livro",    "texto",    "programa", "função",   "dados",
  //     "classe",   "variável", "palavra",  "código",   "livro",    "árvore",
  //     "texto",    "dados",    "função",   "programa", "classe",   "variável",
  //     "palavra",  "código",   "livro",    "texto",    "função",   "programa",
  //     "dados",    "variável", "classe",   "palavra",  "árvore",   "código",
  //     "livro",    "texto",    "programa", "função",   "dados",    "classe",
  //     "variável", "palavra",  "código",   "livro",    "árvore",   "texto",
  //     "dados",    "função",   "programa", "classe",   "variável", "palavra",
  //     "código",   "livro",    "texto",    "função",   "programa", "dados",
  //     "variável", "classe",   "palavra",  "árvore",   "código",   "livro",
  //     "texto",    "programa", "função",   "dados",    "classe",   "variável",
  //     "palavra",  "código",   "livro",    "árvore",   "texto",    "dados",
  //     "função",   "programa"};
  //
  // DataStructure<std::pair<std::string, int>>::execute(avlWords, words);
  // DataStructure<std::pair<std::string, int>>::execute(rbWords, words);
  //
  // DataStructure<int>::execute(avlInt, valores);
  // DataStructure<int>::execute(rbInt, valores);
  //
  // for (const auto &word : words) {
  //   std::pair<std::string, int> wordPair = {word, 1};
  //   openHash.insert(wordPair);
  //   externHash.insert(wordPair);
  // }
  //
  // for (int v : valores) {
  //   avlInt.insert(v);
  //   rbInt.insert(v);
  // }
  //
  // std::cout << "AVL: Remove 30:" << std::endl;
  // avlInt.remove(30);
  // avlInt.show();
  //
  // std::cout << "RB: Remove 30:" << std::endl;
  // rbInt.remove(30);
  // rbInt.show();
  //
  // std::cout << "RB: Remove 'variável':" << std::endl;
  // rbWords.remove({"variável", 0});
  // rbWords.show();
  //
  // std::cout << "AVL: Remove 'variável':" << std::endl;
  // avlWords.remove({"variável", 0});
  // avlWords.show();
  //
  // std::cout << "OpenHashMap: contém 'função'? "
  //           << (openHash.contains({"função", 0}) ? "sim" : "não") << '\n';
  // openHash.show();
  // openHash.remove({"função", 0});
  // std::cout << "OpenHashMap: removeu 'função'. Contém agora? "
  //           << (openHash.contains({"função", 0}) ? "sim" : "não") << '\n';
  // openHash.show();
  //
  // std::cout << "ExternHashMap: contém 'livro'? "
  //           << (externHash.contains({"livro", 0}) ? "sim" : "não") << '\n';
  // externHash.show();
  // externHash.remove({"livro", 0});
  // std::cout << "ExternHashMap: removeu 'livro'. Contém agora? "
  //           << (externHash.contains({"livro", 0}) ? "sim" : "não") << '\n';
  // externHash.show();
  //

  if (argc != 3) {
    std::cerr << "Uso: freq <avl|rb|open|extern> <arquivo_texto>\n";
    return 1;
  }

  std::string structType = argv[1];
  std::string filename = argv[2];

  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Erro ao abrir arquivo: " << filename << "\n";
    return 1;
  }

  auto ds = createStructure(structType);
  if (!ds) {
    std::cerr << "Tipo de estrutura inválido: " << structType << "\n";
    return 1;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
      std::string normalized = normalizeAndLower(word);
      normalized.erase(
          std::remove_if(normalized.begin(), normalized.end(),
                         [](unsigned char c) { return !std::isalnum(c); }),
          normalized.end());
      if (normalized.empty())
        continue;

      std::pair<std::string, int> probe = {normalized, 0};
      if (ds->contains(probe)) {
        auto *node = ds->getNode(probe);
        if (node)
          node->key.second += 1;
      } else {
        ds->insert({normalized, 1});
      }
    }
  }

  ds->show();
  return 0;
}
