CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC_DIR = src

all: performance_test

performance_test: $(SRC_DIR)/performance_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f performance_test performance_test_org ./data/benchmark_results_adv.txt

.PHONY: all clean org
