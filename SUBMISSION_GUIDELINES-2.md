# Anipang Project Submission Guidelines

## Overview

This document outlines the requirements and guidelines for submitting your optimized Anipang game implementation. Please read carefully and ensure all components are included in your submission.

## Required Submission Components

### 1. Source Code Files
- **`main_optimized.cpp`** - Your optimized version of the game
- **`performance_test.cpp`** - Your performance testing code
- **`Makefile`** 

### 2. Documentation Files
- **`PERFORMANCE_REPORT`** - Detailed performance analysis report
- **`IMPLEMENTATION_NOTES`** - Explanation of your optimizations

### 3. Performance Data
- **`benchmark_results.txt`** - Raw performance measurement data

## Performance Report Requirements

Your `PERFORMANCE_REPORT.md` must include:

### 1. Executive Summary
```markdown
## Executive Summary
- Original performance: X ms average
- Optimized performance: Y ms average
- Overall improvement: Z%
- Key optimizations: List top 3 changes
```

### 2. Detailed Performance Analysis
```markdown
## Performance Analysis

### Function-by-Function Comparison
| Function | Original Time | Optimized Time | Improvement | Complexity Change |
|----------|--------------|----------------|-------------|-------------------|
| hasValidMoves() | 245.3 ms | 2.1 ms | 95.1% | O(n⁴) → O(n²) |
| dropBlocks() | 18.7 ms | 1.2 ms | 93.6% | O(n³) → O(n) |
| ... | ... | ... | ... | ... |
```

### 3. Memory Usage Analysis
```markdown
## Memory Usage
- Original: X KB
- Optimized: Y KB
- Reduction: Z%
- Key changes: String → Enum, removed redundant copies
```

### 4. Testing Methodology
```markdown
## Testing Methodology
- Test environment: OS, compiler, hardware specs
- Number of iterations: 1000
- Test scenarios: random boards, worst-case scenarios
- Statistical analysis: mean, median, standard deviation
```

## 💻 Implementation Notes Requirements

Your `IMPLEMENTATION_NOTES` must explain:

### 1. Data Structure Changes
```markdown
## Data Structure Improvements

### 1. Animal Representation
**Before**: `string board[7][7]` (1,568 bytes)
**After**: `enum Animal board[7][7]` (196 bytes)
**Rationale**: Reduced memory usage and faster comparisons

### 2. DFS Implementation
**Before**: Recursive with risk of stack overflow
**After**: Iterative using std::stack
**Code Example**:
```cpp
// Your optimized code snippet
```
```

### 2. Algorithm Optimizations
- Explain each major algorithmic improvement
- Include time complexity analysis
- Provide code snippets showing before/after

### 3. Challenges and Solutions
- What problems did you encounter?
- How did you solve them?
- What trade-offs did you make?

## Code Quality Requirements

Your optimized code must:

### 1. Maintain Functionality

### 2. Include Performance Measurements
```cpp
// Example of integrated timing code
class AnipangGame {
public:
    void hasValidMoves() {
        #ifdef MEASURE_PERFORMANCE
        auto start = chrono::high_resolution_clock::now();
        #endif
        
        // Your optimized code
        
        #ifdef MEASURE_PERFORMANCE
        auto end = chrono::high_resolution_clock::now();
        recordTime("hasValidMoves", start, end);
        #endif
    }
};
```

## Evaluation Criteria

Your submission will be evaluated based on:

### 1. Performance Improvement (40%)
- Execution time reduction
- Memory usage optimization
- Algorithmic complexity improvements

### 2. Code Quality (30%)
- Readability and maintainability
- Proper use of data structures
- Absence of bugs

### 3. Documentation (30%)
- Clarity of performance report
- Thoroughness of implementation notes
- Quality of analysis

## Submission Format

### Directory Structure
```
anipang_[YourTeam]/
├── src/
│   ├── main_optimized.cpp
│   └── performance_test.cpp
├── docs/
│   ├── PERFORMANCE_REPORT.md
│   ├── IMPLEMENTATION_NOTES.md
│   └── README.md
├── data/
│   └── benchmark_results.txt
└── Makefile
```

### Compression
- Create a ZIP or TAR.GZ archive
- Name: `anipang_[Team].zip`

