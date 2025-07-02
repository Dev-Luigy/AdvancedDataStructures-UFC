#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    if (u_charType(c) != U_NON_SPACING_MARK) {
      result.append(c);
    }
    i += charLength;
  }

  result.toLower();

  std::string out;
  result.toUTF8String(out);
  return out;
}

std::string removeAccents(const std::string &word) {
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
    if (u_charType(c) != U_NON_SPACING_MARK) {
      result.append(c);
    }
    i += charLength;
  }

  std::string out;
  result.toUTF8String(out);
  return out;
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

  PERF_TRACKER.startOperation("freq_inserts_and_updates");

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
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
      }
    }
  }

  PERF_TRACKER.endOperation();

  ds->show();

  auto stats = PERF_TRACKER.getStats();

  std::ofstream csv_file("freq_run_results.csv");
  csv_file
      << "Operation,Execution_Time_ms,Rotations,Key_Comparisons,Insertion_"
         "Fixups,Deletion_Fixups,Search_Depth,Nodes_Visited,Color_Changes\n";
  csv_file << "freq_inserts_and_updates," << stats.execution_time_ms << ","
           << stats.rotations << "," << stats.key_comparisons << ","
           << stats.insertion_fixups << "," << stats.deletion_fixups << ","
           << stats.search_depth << "," << stats.nodes_visited << ","
           << stats.color_changes << "\n";
  csv_file.close();

  std::cout << "\nBenchmark da execução freq salvo em freq_run_results.csv\n";

  return 0;
}
