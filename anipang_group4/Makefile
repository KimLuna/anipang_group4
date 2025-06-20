CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: performance_test

performance_test: performance_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f performance_test benchmark_results_*.txt

.PHONY: all clean 