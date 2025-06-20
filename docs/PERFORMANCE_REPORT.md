# Performance Report

## 1. Executive Summary
- **Original performance**: 1.3609641 µs average
- **Optimized performance**: 0.10998866333 µs average
- **Overall improvement**: 91.92%
- **Key optimizations**: List top 3 changes
    - In-place swaps and localized match checking (`hasValidMoves`) to avoid full board scans
    - 2-pointer gravity simulation (`dropBlocks`) to replace bubble sort
    - Iterative DFS with explicit stacks (`countConnectedAnimals`) to avoid stack overflows
    
<br/>

## 2. Performance Analysis
### 2-1. Function-by-Function Comparison
| Function | Original Time(µs) | Optimized Time(µs) | Improvement | Complexity Change |
|----------|--------------|----------------|-------------|-------------------|
| **hasValidMoves()** | 3.95488 | 0.0165523 | 99.58% | O(n⁴) → O(n²) |
| **dropBlocks()** | 0.074119 | 0.0156246 | 78.95% | O(n³) → O(n²) |
| **countConnectedAnimals()**| 0.538933 | 0.297783 | 44.76% | O(n²) → O(n) |


### 2-2. Plot of performance changes in versions
![img](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FcciJnO%2FbtsOKzzB2rs%2FWD5D4XX2jQUD3OmGHLl0t0%2Fimg.png)

### 2-3. Plot of memory usage change in versions
![img](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FbTRZTd%2FbtsOLiYsAsd%2F5KWBQI6FfqCQ9VJZQqQKPK%2Fimg.png)

<br/>

## 3. Memory Usage
- **Original**: 1804 KB
- **Optimized**: 1680 KB
- **Reduction**: 124KB (6.4%)
- **Key changes**: 
    - Replaced `std::string` with `enum` for board representation
    - Reduced memory overhead by eliminating string comparison and deep copies

<br/>

## 4. Testing Methodology
- **Test environment**
    - OS: Ubuntu 20.04(LTS)
    - compiler: g++ 11.4.0 with -O2 flag 
    - hardware specs: Kentech ai-gpu(cuda 11.7.64)
<br/>
- **Number of iterations**: 1000 per function
- **Test tool**: `performance_test_org.cpp` (org–v4), `performance_test.cpp` (v5–v8)
- **Test scenarios**: manually configured boards to simulate matching and gravity
- **Notes**: From v5 onward, enum-based changes required separate test logic due to type mismatch
- **Statistical analysis**: average of 1000 runs, no outlier filtering