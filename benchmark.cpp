#include "PerformanceTracker.hpp"
#include "structures/Data/ExternHashMap.hpp"
#include "structures/Data/OpenHashMap.hpp"
#include "structures/Trees/AVLTree.hpp"
#include "structures/Trees/RedBlack.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>
#include <vector>

class Benchmark {
private:
  struct TestResult {
    std::string structure_name;
    std::string operation;
    double execution_time_ms;
    unsigned long rotations;
    unsigned long key_comparisons;
    unsigned long insertion_fixups;
    unsigned long deletion_fixups;
    unsigned long search_depth;
    unsigned long nodes_visited;
    unsigned long color_changes;
    size_t data_size;
  };

  std::vector<TestResult> results;

public:
  std::vector<std::pair<std::string, int>>
  readStringPairsFromFile(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<std::pair<std::string, int>> data;
    if (!file) {
      std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
      return data;
    }
    std::string line, word;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      while (iss >> word) {
        data.emplace_back(word, 1);
      }
    }
    return data;
  }

  template <typename TreeType>
  void testTreeWithStringPairs(
      const std::string &tree_name,
      const std::vector<std::pair<std::string, int>> &data) {
    std::cout << "\n=== Testing " << tree_name
              << " with string pairs (size = " << data.size()
              << ") ===" << std::endl;

    TreeType tree;

    PERF_TRACKER.startOperation(tree_name + " - Insert " +
                                std::to_string(data.size()) + " string pairs");
    for (const auto &value : data) {
      tree.insert(value);
    }
    PERF_TRACKER.endOperation();

    auto stats = PERF_TRACKER.getStats();
    results.push_back({tree_name, "Insert", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, data.size()});
    PERF_TRACKER.printStats();

    auto search_data = data;
    std::shuffle(search_data.begin(), search_data.end(),
                 std::mt19937{std::random_device{}()});
    search_data.resize(std::min(search_data.size(), size_t(100)));

    PERF_TRACKER.startOperation(tree_name + " - Search " +
                                std::to_string(search_data.size()) +
                                " string pairs");
    int found_count = 0;
    for (const auto &value : search_data) {
      if (tree.contains(value)) {
        found_count++;
      }
    }
    PERF_TRACKER.endOperation();

    stats = PERF_TRACKER.getStats();
    results.push_back({tree_name, "Search", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, search_data.size()});
    PERF_TRACKER.printStats();

    std::cout << "Found " << found_count << " out of " << search_data.size()
              << " elements" << std::endl;

    auto remove_data = data;
    std::shuffle(remove_data.begin(), remove_data.end(),
                 std::mt19937{std::random_device{}()});
    remove_data.resize(std::min(remove_data.size(), size_t(50)));

    PERF_TRACKER.startOperation(tree_name + " - Remove " +
                                std::to_string(remove_data.size()) +
                                " string pairs");
    for (const auto &value : remove_data) {
      tree.remove(value);
    }
    PERF_TRACKER.endOperation();

    stats = PERF_TRACKER.getStats();
    results.push_back({tree_name, "Remove", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, remove_data.size()});
    PERF_TRACKER.printStats();
  }

  template <typename HashMapType>
  void testHashMap(const std::string &hashmap_name,
                   const std::vector<std::pair<std::string, int>> &data) {
    std::cout << "\n=== Testing " << hashmap_name << " (size = " << data.size()
              << ") ===" << std::endl;

    HashMapType hashmap;

    PERF_TRACKER.startOperation(hashmap_name + " - Insert " +
                                std::to_string(data.size()) + " string pairs");
    for (const auto &value : data) {
      hashmap.insert(value);
    }
    PERF_TRACKER.endOperation();

    auto stats = PERF_TRACKER.getStats();
    results.push_back({hashmap_name, "Insert", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, data.size()});
    PERF_TRACKER.printStats();

    auto search_data = data;
    std::shuffle(search_data.begin(), search_data.end(),
                 std::mt19937{std::random_device{}()});
    search_data.resize(std::min(search_data.size(), size_t(100)));

    PERF_TRACKER.startOperation(hashmap_name + " - Search " +
                                std::to_string(search_data.size()) +
                                " string pairs");
    int found_count = 0;
    for (const auto &value : search_data) {
      if (hashmap.contains(value)) {
        found_count++;
      }
    }
    PERF_TRACKER.endOperation();

    stats = PERF_TRACKER.getStats();
    results.push_back({hashmap_name, "Search", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, search_data.size()});
    PERF_TRACKER.printStats();

    std::cout << "Found " << found_count << " out of " << search_data.size()
              << " elements" << std::endl;

    auto remove_data = data;
    std::shuffle(remove_data.begin(), remove_data.end(),
                 std::mt19937{std::random_device{}()});
    remove_data.resize(std::min(remove_data.size(), size_t(50)));

    PERF_TRACKER.startOperation(hashmap_name + " - Remove " +
                                std::to_string(remove_data.size()) +
                                " string pairs");
    for (const auto &value : remove_data) {
      hashmap.remove(value);
    }
    PERF_TRACKER.endOperation();

    stats = PERF_TRACKER.getStats();
    results.push_back({hashmap_name, "Remove", stats.execution_time_ms,
                       stats.rotations, stats.key_comparisons,
                       stats.insertion_fixups, stats.deletion_fixups,
                       stats.search_depth, stats.nodes_visited,
                       stats.color_changes, remove_data.size()});
    PERF_TRACKER.printStats();
  }

  void runAllTests(const std::string &filename) {
    std::cout << "========================================" << std::endl;
    std::cout << "    ADVANCED DATA STRUCTURES BENCHMARK" << std::endl;
    std::cout << "========================================" << std::endl;

    auto string_pair_data = readStringPairsFromFile(filename);
    if (string_pair_data.empty()) {
      std::cerr << "Nenhum dado válido encontrado no arquivo." << std::endl;
      return;
    }

    size_t total_size = string_pair_data.size();

    std::vector<size_t> progressive_sizes;
    size_t min_size = 32;
    while (min_size < total_size) {
      progressive_sizes.push_back(min_size);
      min_size *= 2;
    }

    if (progressive_sizes.empty() || progressive_sizes.back() != total_size) {
      progressive_sizes.push_back(total_size);
    }

    for (size_t size : progressive_sizes) {
      if (size > total_size)
        break;
      std::vector<std::pair<std::string, int>> subset_data(
          string_pair_data.begin(), string_pair_data.begin() + size);

      std::cout << "\n\n*** TESTING WITH DATA SIZE: " << size << " ***\n"
                << std::endl;

      testTreeWithStringPairs<AVLTree<std::pair<std::string, int>>>(
          "AVL Tree", subset_data);
      testTreeWithStringPairs<RedBlack<std::pair<std::string, int>>>(
          "RedBlack Tree", subset_data);
      testHashMap<OpenHashMap<std::pair<std::string, int>>>("OpenHashMap",
                                                            subset_data);
      testHashMap<ExternHashMap<std::pair<std::string, int>>>("ExternHashMap",
                                                              subset_data);
    }

    generateReport();
  }

  void generateReport() {
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "           PERFORMANCE SUMMARY" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << std::left << std::setw(20) << "Structure" << std::setw(12)
              << "Operation" << std::setw(10) << "Size" << std::setw(15)
              << "Time (ms)" << std::setw(12) << "Rotations" << std::setw(15)
              << "Comparisons" << std::endl;
    std::cout << std::string(84, '-') << std::endl;

    for (const auto &result : results) {
      std::cout << std::left << std::setw(20) << result.structure_name
                << std::setw(12) << result.operation << std::setw(10)
                << result.data_size << std::setw(15) << std::fixed
                << std::setprecision(3) << result.execution_time_ms
                << std::setw(12) << result.rotations << std::setw(15)
                << result.key_comparisons << std::endl;
    }

    std::ofstream csv_file("performance_results.csv");
    csv_file << "Structure,Operation,Data_Size,Execution_Time_ms,Rotations,Key_"
                "Comparisons,Insertion_Fixups,Deletion_Fixups,Search_Depth,"
                "Nodes_Visited,Color_Changes\n";
    for (const auto &result : results) {
      csv_file << result.structure_name << "," << result.operation << ","
               << result.data_size << "," << result.execution_time_ms << ","
               << result.rotations << "," << result.key_comparisons << ","
               << result.insertion_fixups << "," << result.deletion_fixups
               << "," << result.search_depth << "," << result.nodes_visited
               << "," << result.color_changes << "\n";
    }
    csv_file.close();

    std::cout << "\nResults saved to performance_results.csv" << std::endl;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <arquivo_texto>\n";
    return 1;
  }
  std::string filename = argv[1];
  Benchmark benchmark;
  benchmark.runAllTests(filename);
  return 0;
}
