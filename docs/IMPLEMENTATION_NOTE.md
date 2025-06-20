## 💻 Implementation Notes

### 1. Data Structure Changes
### 1.1. Animal Representation  
**Before**: `string board[7][7]` (1,568 bytes)  
**After**: `enum Animal board[7][7]` (196 bytes)  

**Code Example**:
```cpp
enum Animal { EMPTY, DOG, CAT, ... };
Animal board[7][7];
```

**Rationale**: Originally, each board cell stored a `std::string` representing an emoji (e.g., "🐶", "🐱"), which consumed significantly more memory and required slower string-based comparison during matching.

By replacing strings with an `enum`:
- We reduced per-cell memory usage from 224 bits (string) to 8 bits (enum).
- Integer comparisons are much faster than string comparisons, leading to a notable performance gain.
- Cache locality and memory alignment improved, accelerating board traversal and match checking operations.
- It also simplified switch-case or matching logic by enabling direct comparison of discrete values.


### 1.2. DFS Implementation  
**Before**: Recursive with risk of stack overflow  
**After**: Iterative using `std::stack`  

**Code Example**:
```cpp
stack<pair<int, int>> s;
s.push({row, col});
while (!s.empty()) {
    auto [r, c] = s.top(); s.pop();
    // visit neighbors and push
}
```

**Rationale**: The recursive DFS worked but was unsafe for large clusters, as it risked exceeding the call stack. Refactoring it to an iterative version using an explicit stack mitigated this risk, eliminated function call overhead, and improved scalability.


### 1.3. Various Data Structure Implementation
**Before**: Revisit or check matched blocks redundantly

**After**: \
• unordered map<Animal, int> – count of each animal on board\
• set<pair<int,int>> visited – prevents re-visiting during BFS\
• queue<pair<int,int>> matchQueue – BFS queue for match detection

**Code Example**: 
```cpp
class AnipangGame {
private:
    ...
    unordered_map<Animal, int> animalCounts; 
    set<pair<int, int>> visitedPositions;     
    queue<pair<int, int>> matchQueue; 
    ...
```

**Rationale**: Improved search efficiency and helped us managed game state more cleanly
### 1.4. Minimizing Scan Overhead
**Before**: Every cell on the board was checked every time

**After**: Operations is performed only on updated regions by tracking

**Code Example**:
```cpp
class AnipangGame {
private:
    ...     
    set<pair<int, int>> changedRegions;
```

**Rationale**: Efficiency(especially during chain reactions or localized actions) was significantly improved


### 2. Algorithm Optimizations
### 2.1. Time Complexity  
**Before**:  
Created a deep copy of the board and called `checkWholeBoard()` after each swap → \( O(n^4) \)  

**After**:  
Performed in-place swap and only checked two local positions for match → \( O(n^2) \)

**Code Example**:
```cpp
swap(board[i][j], board[i][j+1]);
bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(i, j+1);
swap(board[i][j], board[i][j+1]);
```

**Rationale**: By removing deep copies and global matching, performance improved dramatically. The localized check avoided scanning unaffected areas, which eliminated redundant operations and reduced complexity by two orders of magnitude.


### 2.2. Bubble Sort-like Gravity  
**Before**: Bubble sort-like multi-pass cell shifting → \( O(n^3) \)  
**After**: Two-pointer method with single pass → \( O(n^2) \)

**Code Example**:
```cpp
int write = BOARD_SIZE - 1;
for (int read = BOARD_SIZE - 1; read >= 0; read--) {
    if (board[read][col] != EMPTY)
        board[write--][col] = board[read][col];
}
```

**Rationale**: The original approach shifted one block per pass and required multiple iterations per column. The optimized two-pointer method aggregates non-empty blocks from bottom-up in a single pass, significantly reducing iterations and memory writes.


### 2.3. Hardcoded Pattern Check
**Before**: Separate loops were used to check for matches in each direction

**After**: The logic was unified by directional vectors and both directions were handled in a single loop

**Code Example**:
```cpp
bool checkMatchAtPosition(int row, int col) const {
    if (board[row][col] == EMPTY) return false;
    Animal type = board[row][col];

    const int directions[2][2] = {{0, 1}, {1, 0}}; 
    
    for (const auto& dir : directions) {
        int count = 1;
        for (int sign = -1; sign <= 1; sign += 2) {
            int r = row + dir[0] * sign;
            int c = col + dir[1] * sign;
            while (r>=0 && r<BOARD_SIZE && c>=0 && c<BOARD_SIZE && board[r][c]==type) {
                count++;
                r += dir[0] * sign;
                c += dir[1] * sign;
            }
        }
        if (count >= MIN_MATCH) return true;
    }
    return false;
}
```

**Rationale**: Readability and scalability were improved and the likelihood of bugs was reduced

### 2.4. Code Simplification
**Before**: The original code scanned linear directions and could miss more complex shapes like T-or L-shaped patterns. It also repeated logic across directions.

**After**: We switched to a BFS-based approach to explore all connected cells of the same type.

**Code Example**:
```cpp
void removeMatchesAt(int row, int col) {
        Animal type = board[row][col];
        set<pair<int, int>> matches;
        queue<pair<int, int>> q;
        q.push({row, col});

        while (!q.empty()) {
            auto [currentRow, currentCol] = q.front();
            q.pop();

            if (currentRow < 0 || currentRow >= BOARD_SIZE || 
                currentCol < 0 || currentCol >= BOARD_SIZE ||
                board[currentRow][currentCol] != type ||
                matches.find({currentRow, currentCol}) != matches.end()) {
                continue;
            }

            matches.insert({currentRow, currentCol});

            const int dx[] = {-1, 1, 0, 0};
            const int dy[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; i++) {
                int newRow = currentRow + dx[i];
                int newCol = currentCol + dy[i];
                q.push({newRow, newCol});
            }
        }

        if (matches.size() >= MIN_MATCH) {
            for (const auto& pos : matches) {
                board[pos.first][pos.second] = EMPTY;
                animalCounts[type]--;
                changedRegions.insert(pos); 
            }
            score += matches.size() * 10;
        }
    }
```

**Rationale**: Detection was improved for more general match patterns, and the block removal logic was simplified. 


### 3. Challenges and Solutions
- What problems did you encounter?\
We encountered several performance bottlenecks in the original implementation, including excessive time complexity due to full-board scans, risk of stack overflow from recursive DFS, and inefficient memory usage from string-based board representation. Additionally, there was redundant code, hardcoded logic, and lack of proper data structure usage, which limited scalability and maintainability.


- How did you solve them?\
We addressed these issues through algorithmic and structural optimizations. This included replacing recursive DFS with an iterative stack-based approach, applying two-pointer techniques for gravity simulation, using enums instead of strings, and incorporating STL containers for efficient tracking and match removal. Direction vectors and region-based updates further reduced code duplication and unnecessary scanning.


- What trade-offs did you make?\
Some optimizations increased code complexity or required additional data structures, which slightly raised memory usage. Also, in the interest of performance, we focused more on functionality and efficiency rather than keeping the code minimal or beginner-friendly. These trade-offs were acceptable as they led to significantly better scalability and runtime behavior.

