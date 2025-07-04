CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Wextra -O0 -Wno-unused -fdiagnostics-all-candidates

INCLUDES = -Iinterfaces -Iinterfaces/core -Iinterfaces/enum -Iinterfaces/structures \
           -Iinterfaces/trees -Iinterfaces/trees/rotatable \
           -Istructures -Istructures/Data -Istructures/Trees \
           -Istructures/Trees/contexts/AVLTree -Istructures/Trees/contexts/RedBlack \
           -Istructures/Trees/utils -Itests -Ifactory

LIBS = -licuuc -licui18n

TARGET_BENCHMARK = benchmark
TARGET_MAIN = maine

SOURCES_BENCHMARK = benchmark.cpp
SOURCES_MAIN = main.cpp factory/makeStructury.cpp structures/Trees/utils/treeUtils.cpp

all: $(TARGET_BENCHMARK) $(TARGET_MAIN)

$(TARGET_BENCHMARK): $(SOURCES_BENCHMARK)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $@ $^

$(TARGET_MAIN): $(SOURCES_MAIN)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o $@ $^

clean:
	rm -f $(TARGET_BENCHMARK) $(TARGET_MAIN) performance_results.csv freq_run_results.csv

run-benchmark: $(TARGET_BENCHMARK)
	./$(TARGET_BENCHMARK) texto.txt

run-main: $(TARGET_MAIN)
	./$(TARGET_MAIN) texto.txt

.PHONY: all clean run-benchmark run-main
