CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g -Wno-unused-but-set-variable

TARGET_BENCHMARK = benchmark
TARGET_MAIN = main

SOURCES_BENCHMARK = benchmark.cpp
SOURCES_MAIN = main.cpp

INCLUDES = -I./interfaces -I./structures

all: $(TARGET_BENCHMARK) $(TARGET_MAIN)

$(TARGET_BENCHMARK): $(SOURCES_BENCHMARK)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(TARGET_MAIN): $(SOURCES_MAIN)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Limpeza
clean:
	rm -f $(TARGET_BENCHMARK) $(TARGET_MAIN) performance_results.csv

# Execução
run-benchmark: $(TARGET_BENCHMARK)
	./$<

run-main: $(TARGET_MAIN)
	./$<

.PHONY: all clean run-benchmark run-main
