CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
TARGET_BENCHMARK = benchmark
TARGET_MAIN = main
SOURCES_BENCHMARK = benchmark.cpp
SOURCES_MAIN = main.cpp

# Include directories
INCLUDES = -I./interfaces -I./structures

all: $(TARGET_BENCHMARK) $(TARGET_MAIN)

$(TARGET_BENCHMARK): $(SOURCES_BENCHMARK)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET_BENCHMARK) $(SOURCES_BENCHMARK)

$(TARGET_MAIN): $(SOURCES_MAIN)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET_MAIN) $(SOURCES_MAIN)

clean:
	rm -f $(TARGET_BENCHMARK) $(TARGET_MAIN) performance_results.csv

run-benchmark: $(TARGET_BENCHMARK)
	./$(TARGET_BENCHMARK)

run-main: $(TARGET_MAIN)
	./$(TARGET_MAIN)

.PHONY: all clean run-benchmark run-main
