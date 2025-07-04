#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <unicode/normalizer2.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>

#include "PerformanceTracker.hpp"
#include "factory/makeStructury.hpp"

std::string normalizeAndRemoveAccents(const std::string &word) {
  UErrorCode status = U_ZERO_ERROR;
  const icu::Normalizer2 *normalizer = icu::Normalizer2::getNFDInstance(status);
  if (U_FAILURE(status))
    return word;

  icu::UnicodeString ustr = icu::UnicodeString::fromUTF8(word);
  icu::UnicodeString normalized = normalizer->normalize(ustr, status);
  if (U_FAILURE(status))
    return word;

  icu::UnicodeString result;
  for (int32_t i = 0; i < normalized.length();) {
    UChar32 c = normalized.char32At(i);
    int32_t charLength = U16_LENGTH(c);
    if (u_charType(c) != U_NON_SPACING_MARK)
      result.append(c);
    i += charLength;
  }

  result.toLower();
  std::string out;
  result.toUTF8String(out);
  return out;
}

size_t wordCounter(const std::string &filename) {
  std::ifstream file(filename);
  if (!file)
    return 0;

  size_t total = 0;
  std::string word;
  while (file >> word)
    ++total;
  return total;
}

std::vector<size_t> generateLim(size_t total) {
  std::vector<double> percentuais = {0.01, 0.05, 0.10, 0.25, 0.5, 0.75, 1.0};
  std::vector<size_t> limites;
  for (double p : percentuais) {
    size_t limite = static_cast<size_t>(std::ceil(p * total));
    if (limite > 0 && (limites.empty() || limite != limites.back()))
      limites.push_back(limite);
  }
  return limites;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Uso: " << argv[0]
              << " freq <avl|rb|open|extern> <arquivo_texto>\n";
    return 1;
  }

  std::string command = argv[1];
  if (command != "freq") {
    std::cerr << "Comando inválido. Use:\n"
              << argv[0] << " freq <avl|rb|open|extern> <arquivo_texto>\n";
    return 1;
  }

  std::string structType = argv[2];
  std::string filename = argv[3];

  size_t total_palavras = wordCounter(filename);
  if (total_palavras == 0) {
    std::cerr << "Arquivo vazio ou não pôde ser lido.\n";
    return 1;
  }

  std::vector<size_t> word_limits = generateLim(total_palavras);
  std::cout << "Total de palavras no arquivo: " << total_palavras << "\n";

  std::ofstream csv_file("freq_run_results.csv");
  csv_file
      << "Operation,Execution_Time_ms,Rotations,Key_Comparisons,Insertion_"
         "Fixups,Deletion_Fixups,Search_Depth,Nodes_Visited,Color_Changes\n";

  for (size_t limit : word_limits) {
    std::ifstream input(filename);
    if (!input) {
      std::cerr << "Erro ao reabrir o arquivo.\n";
      break;
    }

    auto ds = createStructure(structType);
    if (!ds) {
      std::cerr << "Tipo de estrutura inválido: " << structType << "\n";
      return 1;
    }

    PERF_TRACKER.startOperation("freq_" + std::to_string(limit));

    size_t word_count = 0;
    std::string word;
    while (input >> word && word_count < limit) {
      std::string cleaned = normalizeAndRemoveAccents(word);
      cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(),
                                   [](unsigned char c) {
                                     return !(std::isalnum(c) || c == '-');
                                   }),
                    cleaned.end());

      if (!cleaned.empty()) {
        std::pair<std::string, int> probe{cleaned, 0};
        if (ds->contains(probe)) {
          auto *node = ds->getNode(probe);
          if (node)
            node->key.second++;
        } else {
          ds->insert({cleaned, 1});
        }
        ++word_count;
      }
    }

    PERF_TRACKER.endOperation();
    auto stats = PERF_TRACKER.getStats();

    csv_file << "freq_" << limit << "," << stats.execution_time_ms << ","
             << stats.rotations << "," << stats.key_comparisons << ","
             << stats.insertion_fixups << "," << stats.deletion_fixups << ","
             << stats.search_depth << "," << stats.nodes_visited << ","
             << stats.color_changes << "\n";

    std::cout << "✔ Benchmark com " << limit << " palavras finalizado.\n";
  }

  csv_file.close();
  std::cout << "\n✅ Resultados salvos em freq_run_results.csv\n";
  return 0;
}
