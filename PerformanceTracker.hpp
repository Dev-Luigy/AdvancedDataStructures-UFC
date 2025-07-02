#ifndef PERFORMANCE_TRACKER_HPP
#define PERFORMANCE_TRACKER_HPP

#include <chrono>
#include <iostream>
#include <string>

class PerformanceTracker {
public:
  // Statistics structure
  struct Stats {
    double execution_time_ms = 0.0;
    unsigned long rotations = 0;
    unsigned long key_comparisons = 0;
    std::string operation_name = "";

    void reset() {
      execution_time_ms = 0.0;
      rotations = 0;
      key_comparisons = 0;
      operation_name = "";
    }

    void print() const {
      std::cout << "=== Performance Stats for " << operation_name
                << " ===" << std::endl;
      std::cout << "Execution Time: " << execution_time_ms << " ms"
                << std::endl;
      std::cout << "Rotations: " << rotations << std::endl;
      std::cout << "Key Comparisons: " << key_comparisons << std::endl;
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

  void addRotations(unsigned long count) { current_stats.rotations += count; }

  void addComparisons(unsigned long count) {
    current_stats.key_comparisons += count;
  }

  const Stats &getStats() const { return current_stats; }

  void printStats() const { current_stats.print(); }
};

// Macro for easier access
#define PERF_TRACKER PerformanceTracker::getInstance()

#endif // PERFORMANCE_TRACKER_HPP
