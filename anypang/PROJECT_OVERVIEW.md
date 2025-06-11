# Anipang Project Overview

## 🎯 Project Summary

This project provides an intentionally inefficient implementation of the Anipang (match-3 puzzle) game. Students will learn practical data structure applications by optimizing the provided code and measuring real performance improvements.

### Learning Goals
- Apply data structures (Stack, Queue, Set, Priority Queue) to real problems
- Understand time/space complexity through hands-on optimization
- Experience code refactoring practices
- Learn performance measurement and analysis techniques

### Project Structure
```
anypang/
├── main_hardcoded.cpp     # Inefficient version for students
├── CODE_ANALYSIS.md       # Detailed inefficiency analysis
└── PROJECT_OVERVIEW.md    # guides
```

## Performance Measurement Guide

### 1. Basic Time Measurement

#### Using chrono Library
```cpp
#include <chrono>
#include <iostream>

// Function to measure
void functionToMeasure() {
    auto start = chrono::high_resolution_clock::now();
    
    // Code to measure
    hasValidMoves();  // Example function
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    cout << "Execution time: " << duration.count() << " μs" << endl;
}
```

#### Measuring Multiple Runs
```cpp
const int NUM_RUNS = 1000;
long long totalTime = 0;

for (int i = 0; i < NUM_RUNS; i++) {
    auto start = chrono::high_resolution_clock::now();
    
    // Function to test
    dropBlocks();
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    totalTime += duration.count();
}

cout << "Average time: " << (totalTime / NUM_RUNS) << " μs" << endl;
```

### 2. Memory Usage Measurement

#### Simple Memory Tracking
```cpp
#include <sys/resource.h>  // Linux/Mac
// or
#include <windows.h>       // Windows
#include <psapi.h>

// Linux/Mac version
long getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // KB on Linux, bytes on Mac
}

// Usage example
long memBefore = getMemoryUsage();
// Run your code
long memAfter = getMemoryUsage();
cout << "Memory used: " << (memAfter - memBefore) << " KB" << endl;
```

#### Memory Profiling for String vs Enum
```cpp
// Test 1: String-based board
void testStringMemory() {
    string board[BOARD_SIZE][BOARD_SIZE];
    // Each string: ~32 bytes (typical)
    // Total: 49 * 32 = 1,568 bytes
}

// Test 2: Enum-based board
void testEnumMemory() {
    enum Animal { DOG, CAT, RABBIT, PANDA, KOALA, EMPTY };
    Animal board[BOARD_SIZE][BOARD_SIZE];
    // Each enum: 4 bytes
    // Total: 49 * 4 = 196 bytes
    // Savings: 87.5%!
}
```

### 3. Creating a Performance Test Suite

```cpp
class PerformanceTest {
private:
    AnipangGame game;
    
public:
    void runAllTests() {
        cout << "=== Performance Test Results ===" << endl;
        testHasValidMoves();
        testDropBlocks();
        testFindMatches();
        testMemoryUsage();
    }
    
    void testHasValidMoves() {
        const int ITERATIONS = 100;
        auto start = chrono::high_resolution_clock::now();
        
        for (int i = 0; i < ITERATIONS; i++) {
            game.hasValidMoves();
        }
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "hasValidMoves() - " << ITERATIONS << " calls: " 
             << duration.count() << " ms" << endl;
        cout << "Average per call: " 
             << (duration.count() / (double)ITERATIONS) << " ms" << endl;
    }
    
    void testDropBlocks() {
        // Setup: Create board with gaps
        setupBoardWithGaps();
        
        auto start = chrono::high_resolution_clock::now();
        game.dropBlocks();
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "dropBlocks(): " << duration.count() << " μs" << endl;
    }
};
```

### 4. Comparative Analysis Template

```cpp
void compareImplementations() {
    cout << "\n=== Implementation Comparison ===" << endl;
    cout << "Function        | Original  | Optimized | Improvement" << endl;
    cout << "----------------|-----------|-----------|------------" << endl;
    
    // Test hasValidMoves
    double originalTime = measureOriginal();
    double optimizedTime = measureOptimized();
    double improvement = ((originalTime - optimizedTime) / originalTime) * 100;
    
    cout << "hasValidMoves() | " << fixed << setprecision(2) 
         << originalTime << " ms | " 
         << optimizedTime << " ms | " 
         << improvement << "%" << endl;
}
```

### 5. Profiling Tools Integration

#### Using gprof (Linux/Mac)
```bash
# Compile with profiling
g++ -pg main_hardcoded.cpp -o anipang_prof

# Run the program
./anipang_prof

# Generate profile report
gprof anipang_prof gmon.out > analysis.txt
```

#### Using Valgrind (Memory profiling)
```bash
# Memory leak detection
valgrind --leak-check=full ./anipang

# Memory usage profiling
valgrind --tool=massif ./anipang
ms_print massif.out.* > memory_report.txt
```

### 6. Performance Metrics to Track

| Metric | How to Measure | Expected Improvement |
|--------|----------------|---------------------|
| **Execution Time** | chrono library | 70-95% faster |
| **Memory Usage** | Memory profilers | 70% less memory |
| **Cache Misses** | perf tool | 50% reduction |
| **Function Calls** | gprof | 80% fewer calls |
| **Stack Depth** | Custom counter | From recursive to O(1) |

### 7. Visualization of Results

```cpp
void printPerformanceChart(const vector<double>& original, 
                          const vector<double>& optimized) {
    cout << "\nPerformance Comparison Chart:" << endl;
    cout << "Original  : ";
    for (int i = 0; i < original[0] / 10; i++) cout << "█";
    cout << " " << original[0] << " ms" << endl;
    
    cout << "Optimized : ";
    for (int i = 0; i < optimized[0] / 10; i++) cout << "█";
    cout << " " << optimized[0] << " ms" << endl;
}
```

### 8. Automated Performance Report

```cpp
class PerformanceReport {
public:
    void generateReport(const string& filename) {
        ofstream report(filename);
        report << "# Anipang Performance Report" << endl;
        report << "Date: " << getCurrentDate() << endl;
        report << "\n## Test Results" << endl;
        
        // Run all tests and write results
        auto results = runAllBenchmarks();
        
        for (const auto& result : results) {
            report << "- " << result.name << ": " 
                   << result.time << " ms "
                   << "(" << result.improvement << "% improvement)" << endl;
        }
        
        report.close();
    }
};
```

## 🎓 Student Guidelines

### Before Optimization
1. Run performance tests on the original code
2. Record baseline measurements
3. Identify bottlenecks using profiling

### During Optimization
1. Make one change at a time
2. Measure after each change
3. Document what improved and why

### After Optimization
1. Create comparison charts
2. Write a performance report
3. Explain the algorithmic improvements

## 📈 Expected Results

Students should achieve:
- **95%+ improvement** in `hasValidMoves()` (O(n⁴) → O(n²))
- **85%+ improvement** in `dropBlocks()` (O(n³) → O(n))
- **70%+ memory savings** (String → Enum)
- **Stack safety** in recursive functions
- **50%+ reduction** in code duplication

## 🔧 Quick Start

1. **Compile both versions**:
   ```bash
   g++ -O0 main_hardcoded.cpp -o anipang_slow
   g++ -O2 main_emoji.cpp -o anipang_fast
   ```

2. **Add timing code** to critical functions

3. **Run benchmarks** before and after optimization

4. **Create performance report** with your findings

---