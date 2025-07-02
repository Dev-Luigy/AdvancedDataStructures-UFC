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
    // Generate test data
    std::vector<int> generateRandomInts(size_t count, int min_val = 1, int max_val = 10000) {
        std::vector<int> data;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min_val, max_val);
        
        for (size_t i = 0; i < count; ++i) {
            data.push_back(dis(gen));
        }
        return data;
    }
    
    std::vector<std::string> generateRandomStrings(size_t count, size_t string_length = 8) {
        std::vector<std::string> data;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis('a', 'z');
        
        for (size_t i = 0; i < count; ++i) {
            std::string str;
            for (size_t j = 0; j < string_length; ++j) {
                str += static_cast<char>(dis(gen));
            }
            data.push_back(str);
        }
        return data;
    }
    
    std::vector<std::pair<std::string, int>> generateStringPairs(const std::vector<std::string>& strings) {
        std::vector<std::pair<std::string, int>> pairs;
        for (const auto& str : strings) {
            pairs.emplace_back(str, 1);
        }
        return pairs;
    }
    
    // Test tree structures with integers
    template<typename TreeType>
    void testTreeWithInts(const std::string& tree_name, const std::vector<int>& data) {
        std::cout << "\n=== Testing " << tree_name << " with integers ===" << std::endl;
        
        TreeType tree;
        
        // Test insertions
        PERF_TRACKER.startOperation(tree_name + " - Insert " + std::to_string(data.size()) + " integers");
        for (const auto& value : data) {
            tree.insert(value);
        }
        PERF_TRACKER.endOperation();
        
        auto stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Insert", stats.execution_time_ms, stats.rotations, stats.key_comparisons, 
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth, 
                          stats.nodes_visited, stats.color_changes, data.size()});
        PERF_TRACKER.printStats();
        
        // Test search operations
        std::vector<int> search_data = data;
        std::shuffle(search_data.begin(), search_data.end(), std::mt19937{std::random_device{}()});
        search_data.resize(std::min(search_data.size(), size_t(100))); // Test with first 100 elements
        
        PERF_TRACKER.startOperation(tree_name + " - Search " + std::to_string(search_data.size()) + " integers");
        int found_count = 0;
        for (const auto& value : search_data) {
            if (tree.contains(value)) {
                found_count++;
            }
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Search", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, search_data.size()});
        PERF_TRACKER.printStats();
        
        std::cout << "Found " << found_count << " out of " << search_data.size() << " elements" << std::endl;
        
        // Test removals
        std::vector<int> remove_data = data;
        std::shuffle(remove_data.begin(), remove_data.end(), std::mt19937{std::random_device{}()});
        remove_data.resize(std::min(remove_data.size(), size_t(50))); // Remove first 50 elements
        
        PERF_TRACKER.startOperation(tree_name + " - Remove " + std::to_string(remove_data.size()) + " integers");
        for (const auto& value : remove_data) {
            tree.remove(value);
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Remove", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, remove_data.size()});
        PERF_TRACKER.printStats();
    }
    
    // Test tree structures with string pairs
    template<typename TreeType>
    void testTreeWithStringPairs(const std::string& tree_name, const std::vector<std::pair<std::string, int>>& data) {
        std::cout << "\n=== Testing " << tree_name << " with string pairs ===" << std::endl;
        
        TreeType tree;
        
        // Test insertions
        PERF_TRACKER.startOperation(tree_name + " - Insert " + std::to_string(data.size()) + " string pairs");
        for (const auto& value : data) {
            tree.insert(value);
        }
        PERF_TRACKER.endOperation();
        
        auto stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Insert", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, data.size()});
        PERF_TRACKER.printStats();
        
        // Test search operations
        auto search_data = data;
        std::shuffle(search_data.begin(), search_data.end(), std::mt19937{std::random_device{}()});
        search_data.resize(std::min(search_data.size(), size_t(100))); // Test with first 100 elements
        
        PERF_TRACKER.startOperation(tree_name + " - Search " + std::to_string(search_data.size()) + " string pairs");
        int found_count = 0;
        for (const auto& value : search_data) {
            if (tree.contains(value)) {
                found_count++;
            }
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Search", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, search_data.size()});
        PERF_TRACKER.printStats();
        
        std::cout << "Found " << found_count << " out of " << search_data.size() << " elements" << std::endl;
        
        // Test removals
        auto remove_data = data;
        std::shuffle(remove_data.begin(), remove_data.end(), std::mt19937{std::random_device{}()});
        remove_data.resize(std::min(remove_data.size(), size_t(50))); // Remove first 50 elements
        
        PERF_TRACKER.startOperation(tree_name + " - Remove " + std::to_string(remove_data.size()) + " string pairs");
        for (const auto& value : remove_data) {
            tree.remove(value);
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({tree_name, "Remove", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, remove_data.size()});
        PERF_TRACKER.printStats();
    }
    
    // Test hash map structures
    template<typename HashMapType>
    void testHashMap(const std::string& hashmap_name, const std::vector<std::pair<std::string, int>>& data) {
        std::cout << "\n=== Testing " << hashmap_name << " ===" << std::endl;
        
        HashMapType hashmap;
        
        // Test insertions
        PERF_TRACKER.startOperation(hashmap_name + " - Insert " + std::to_string(data.size()) + " string pairs");
        for (const auto& value : data) {
            hashmap.insert(value);
        }
        PERF_TRACKER.endOperation();
        
        auto stats = PERF_TRACKER.getStats();
        results.push_back({hashmap_name, "Insert", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, data.size()});
        PERF_TRACKER.printStats();
        
        // Test search operations
        auto search_data = data;
        std::shuffle(search_data.begin(), search_data.end(), std::mt19937{std::random_device{}()});
        search_data.resize(std::min(search_data.size(), size_t(100))); // Test with first 100 elements
        
        PERF_TRACKER.startOperation(hashmap_name + " - Search " + std::to_string(search_data.size()) + " string pairs");
        int found_count = 0;
        for (const auto& value : search_data) {
            if (hashmap.contains(value)) {
                found_count++;
            }
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({hashmap_name, "Search", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, search_data.size()});
        PERF_TRACKER.printStats();
        
        std::cout << "Found " << found_count << " out of " << search_data.size() << " elements" << std::endl;
        
        // Test removals
        auto remove_data = data;
        std::shuffle(remove_data.begin(), remove_data.end(), std::mt19937{std::random_device{}()});
        remove_data.resize(std::min(remove_data.size(), size_t(50))); // Remove first 50 elements
        
        PERF_TRACKER.startOperation(hashmap_name + " - Remove " + std::to_string(remove_data.size()) + " string pairs");
        for (const auto& value : remove_data) {
            hashmap.remove(value);
        }
        PERF_TRACKER.endOperation();
        
        stats = PERF_TRACKER.getStats();
        results.push_back({hashmap_name, "Remove", stats.execution_time_ms, stats.rotations, stats.key_comparisons,
                          stats.insertion_fixups, stats.deletion_fixups, stats.search_depth,
                          stats.nodes_visited, stats.color_changes, remove_data.size()});
        PERF_TRACKER.printStats();
    }
    
    void runAllTests() {
        std::cout << "========================================" << std::endl;
        std::cout << "    ADVANCED DATA STRUCTURES BENCHMARK" << std::endl;
        std::cout << "========================================" << std::endl;
        
        // Generate test data
        std::vector<size_t> data_sizes = {100, 500, 1000};
        
        for (auto size : data_sizes) {
            std::cout << "\n\n*** TESTING WITH DATA SIZE: " << size << " ***\n" << std::endl;
            
            auto int_data = generateRandomInts(size);
            auto string_data = generateRandomStrings(size);
            auto string_pair_data = generateStringPairs(string_data);
            
            // Test trees with integers
            testTreeWithInts<AVLTree<int>>("AVL Tree", int_data);
            testTreeWithInts<RedBlack<int>>("RedBlack Tree", int_data);
            
            // Test trees with string pairs
            testTreeWithStringPairs<AVLTree<std::pair<std::string, int>>>("AVL Tree", string_pair_data);
            testTreeWithStringPairs<RedBlack<std::pair<std::string, int>>>("RedBlack Tree", string_pair_data);
            
            // Test hash maps
            testHashMap<OpenHashMap<std::pair<std::string, int>>>("OpenHashMap", string_pair_data);
            testHashMap<ExternHashMap<std::pair<std::string, int>>>("ExternHashMap", string_pair_data);
        }
        
        // Generate summary report
        generateReport();
    }
    
    void generateReport() {
        std::cout << "\n\n========================================" << std::endl;
        std::cout << "           PERFORMANCE SUMMARY" << std::endl;
        std::cout << "========================================" << std::endl;
        
        std::cout << std::left << std::setw(20) << "Structure"
                  << std::setw(12) << "Operation"
                  << std::setw(10) << "Size"
                  << std::setw(15) << "Time (ms)"
                  << std::setw(12) << "Rotations"
                  << std::setw(15) << "Comparisons" << std::endl;
        std::cout << std::string(84, '-') << std::endl;
        
        for (const auto& result : results) {
            std::cout << std::left << std::setw(20) << result.structure_name
                      << std::setw(12) << result.operation
                      << std::setw(10) << result.data_size
                      << std::setw(15) << std::fixed << std::setprecision(3) << result.execution_time_ms
                      << std::setw(12) << result.rotations
                      << std::setw(15) << result.key_comparisons << std::endl;
        }
        
        // Save to CSV file
        std::ofstream csv_file("performance_results.csv");
        csv_file << "Structure,Operation,Data_Size,Execution_Time_ms,Rotations,Key_Comparisons,Insertion_Fixups,Deletion_Fixups,Search_Depth,Nodes_Visited,Color_Changes\n";
        for (const auto& result : results) {
            csv_file << result.structure_name << ","
                     << result.operation << ","
                     << result.data_size << ","
                     << result.execution_time_ms << ","
                     << result.rotations << ","
                     << result.key_comparisons << ","
                     << result.insertion_fixups << ","
                     << result.deletion_fixups << ","
                     << result.search_depth << ","
                     << result.nodes_visited << ","
                     << result.color_changes << "\n";
        }
        csv_file.close();
        
        std::cout << "\nResults saved to performance_results.csv" << std::endl;
    }
};

int main() {
    Benchmark benchmark;
    benchmark.runAllTests();
    return 0;
}
