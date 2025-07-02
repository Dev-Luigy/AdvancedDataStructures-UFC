
#ifndef PERFORMANCE_TRACKER_HPP
#define PERFORMANCE_TRACKER_HPP

#include <chrono>
#include <iostream>
#include <string>

class PerformanceTracker {
public:
  struct Stats {
    double execution_time_ms = 0.0;
    unsigned long rotations = 0;
    unsigned long key_comparisons = 0;
    unsigned long insertion_fixups = 0;
    unsigned long deletion_fixups = 0;
    unsigned long search_depth = 0;
    unsigned long nodes_visited = 0;
    unsigned long color_changes = 0;
    std::string operation_name = "";

    void reset() {
      execution_time_ms = 0.0;
      rotations = 0;
      key_comparisons = 0;
      insertion_fixups = 0;
      deletion_fixups = 0;
      search_depth = 0;
      nodes_visited = 0;
      color_changes = 0;
      operation_name = "";
    }

    void print() const {
      std::cout << "=== Performance Stats for " << operation_name << " ===\n";
      std::cout << "Execution Time: " << execution_time_ms << " ms\n";
      std::cout << "Rotations: " << rotations << "\n";
      std::cout << "Key Comparisons: " << key_comparisons << "\n";
      std::cout << "Insertion Fixups: " << insertion_fixups << "\n";
      std::cout << "Deletion Fixups: " << deletion_fixups << "\n";
      std::cout << "Search Depth: " << search_depth << "\n";
      std::cout << "Nodes Visited: " << nodes_visited << "\n";
      std::cout << "Color Changes: " << color_changes << "\n";
      std::cout << "=====================================\n" << std::endl;
    }
  };

private:
  Stats current_stats;
  std::chrono::high_resolution_clock::time_point start_time;

public:
  static PerformanceTracker &getInstance() {
    static PerformanceTracker instance;
    return instance;
  }

  void startOperation(const std::string &operation_name) {
    current_stats.reset();
    current_stats.operation_name = operation_name;
    start_time = std::chrono::high_resolution_clock::now();
  }

  void endOperation() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - start_time);
    current_stats.execution_time_ms = duration.count() / 1000.0;
  }

  void incrementRotations() { current_stats.rotations++; }
  void incrementComparisons() { current_stats.key_comparisons++; }
  void incrementInsertionFixups() { current_stats.insertion_fixups++; }
  void incrementDeletionFixups() { current_stats.deletion_fixups++; }
  void incrementNodesVisited() { current_stats.nodes_visited++; }
  void incrementColorChanges() { current_stats.color_changes++; }
  void incrementSearchDepth() { current_stats.search_depth++; }

  void addRotations(unsigned long count) { current_stats.rotations += count; }
  void addComparisons(unsigned long count) {
    current_stats.key_comparisons += count;
  }
  void addInsertionFixups(unsigned long count) {
    current_stats.insertion_fixups += count;
  }
  void addDeletionFixups(unsigned long count) {
    current_stats.deletion_fixups += count;
  }
  void addSearchDepth(unsigned long depth) {
    current_stats.search_depth += depth;
  }
  void addNodesVisited(unsigned long count) {
    current_stats.nodes_visited += count;
  }
  void addColorChanges(unsigned long count) {
    current_stats.color_changes += count;
  }

  const Stats &getStats() const { return current_stats; }
  void printStats() const { current_stats.print(); }
};

#define PERF_TRACKER PerformanceTracker::getInstance()

#endif // PERFORMANCE_TRACKER_HPP
