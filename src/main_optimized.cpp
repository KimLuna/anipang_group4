#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
// #include <windows.h>
#include <stack>
#include <queue>
#include <set>
#include <unordered_map>

using namespace std;

const int BOARD_SIZE = 7;
const int ANIMAL_TYPES = 5;
const int INITIAL_MOVES = 30;
const int MIN_MATCH = 3;

// [IE#4] Replaced string with enum for better performance
enum Animal { DOG, CAT, RABBIT, PANDA, KOALA, EMPTY };
const string ANIMAL_EMOJIS[] = {"🐶", "🐱", "🐰", "🐼", "🐨", "  "};

class AnipangGame {
private:
    Animal board[BOARD_SIZE][BOARD_SIZE];
    int score;
    int movesLeft;
    // [IE#6] Data Structure for Optimization
    unordered_map<Animal, int> animalCounts; 
    set<pair<int, int>> visitedPositions;     
    queue<pair<int, int>> matchQueue;   
    // [IE#8] Changed Region Tracking      
    set<pair<int, int>> changedRegions;

    void clearScreen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

public:
    AnipangGame() : score(0), movesLeft(INITIAL_MOVES) {
        srand(time(nullptr));
        initializeBoard();
    }

    void initializeBoard() {
        animalCounts.clear();
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = static_cast<Animal>(rand() % ANIMAL_TYPES);
                animalCounts[board[i][j]]++;
            }
        }

        bool hasMatch = true;
        while (hasMatch) {
            hasMatch = false;
            visitedPositions.clear();
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    pair<int, int> pos(i, j);
                    if (visitedPositions.find(pos) == visitedPositions.end() && checkMatchAtPosition(i, j)) {
                        hasMatch = true;
                        removeMatchesAt(i, j);
                    }
                }
            }
            fillEmptySpaces();
        }
    }

    void display() const {
        clearScreen();
        cout << "\n===== ANIPANG GAME =====" << endl;
        cout << "Score: " << score << " | Moves Left: " << movesLeft << endl;
        cout << "\n   ";
        for (int j = 0; j < BOARD_SIZE; j++) cout << setw(4) << j;
        cout << endl;

        cout << "  ┌";
        for (int j = 0; j < BOARD_SIZE; j++) cout << "────";
        cout << "┐" << endl;

        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << setw(2) << i << "│";
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << " " << ANIMAL_EMOJIS[board[i][j]] << " ";
            }
            cout << "│" << endl;
        }

        cout << "  └";
        for (int j = 0; j < BOARD_SIZE; j++) cout << "────";
        cout << "┘" << endl;
    }

    // [IE#2] Iterative DFS
    int countConnectedAnimals(int startRow, int startCol, Animal type, bool visited[BOARD_SIZE][BOARD_SIZE]) {
        int count = 0;
        stack<pair<int, int>> stk;
        stk.push({startRow, startCol});

        while (!stk.empty()) {
            auto [row, col] = stk.top(); stk.pop();
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) continue;
            if (visited[row][col] || board[row][col] != type) continue;

            visited[row][col] = true;
            count++;

            stk.push({row - 1, col});
            stk.push({row + 1, col});
            stk.push({row, col - 1});
            stk.push({row, col + 1});
        }

        return count;
    }

    // [IE#5] Hardcoded Pattern Checking
    bool checkMatchAtPosition(int row, int col) const {
        if (board[row][col] == EMPTY) return false;
        Animal type = board[row][col];
    
        const int directions[2][2] = {{0, 1}, {1, 0}}; 
        
        for (const auto& dir : directions) {
            int count = 1;
            for (int sign = -1; sign <= 1; sign += 2) {
                int r = row + dir[0] * sign;
                int c = col + dir[1] * sign;
                while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == type) {
                    count++;
                    r += dir[0] * sign;
                    c += dir[1] * sign;
                }
            }
            if (count >= MIN_MATCH) return true;
        }
        return false;
    }

    // [IE#7] Using BFS
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

    // [IE#3] Optimized drop
    void dropBlocks() {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int writeRow = BOARD_SIZE - 1;
            for (int row = BOARD_SIZE - 1; row >= 0; row--) {
                if (board[row][col] != EMPTY) {
                    if (writeRow != row) {
                        board[writeRow][col] = board[row][col];
                        board[row][col] = EMPTY;
                        changedRegions.insert({writeRow, col});
                        changedRegions.insert({row, col});
                    }
                    writeRow--;
                }
            }
        }
    }

    void fillEmptySpaces() {
        for (const auto& pos : changedRegions) {
            if (board[pos.first][pos.second] == EMPTY) {
                Animal newAnimal = static_cast<Animal>(rand() % ANIMAL_TYPES);
                board[pos.first][pos.second] = newAnimal;
                animalCounts[newAnimal]++;
            }
        }
        changedRegions.clear();
    }

    // [IE#1] Optimized hasValidMoves
    bool hasValidMoves() const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (j < BOARD_SIZE - 1) {
                    swap(const_cast<Animal&>(board[i][j]), const_cast<Animal&>(board[i][j + 1]));
                    bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(i, j + 1);
                    swap(const_cast<Animal&>(board[i][j]), const_cast<Animal&>(board[i][j + 1]));
                    if (match) return true;
                }
                if (i < BOARD_SIZE - 1) {
                    swap(const_cast<Animal&>(board[i][j]), const_cast<Animal&>(board[i + 1][j]));
                    bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(i + 1, j);
                    swap(const_cast<Animal&>(board[i][j]), const_cast<Animal&>(board[i + 1][j]));
                    if (match) return true;
                }
            }
        }
        return false;
    }

    bool makeMove(int r1, int c1, int r2, int c2) {
        if (!((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1))) {
            cout << "Not adjacent positions!" << endl;
            return false;
        }

        changedRegions.clear();  
        changedRegions.insert({r1, c1});
        changedRegions.insert({r2, c2});

        swap(board[r1][c1], board[r2][c2]);
        bool foundMatch = false;

        while (true) {
            bool matchFound = false;
            set<pair<int, int>> checkRegions = changedRegions;
            for (const auto& pos : checkRegions) {
                if (checkMatchAtPosition(pos.first, pos.second)) {
                    removeMatchesAt(pos.first, pos.second);
                    matchFound = true;
                    foundMatch = true;
                }
            }
            if (!matchFound) break;
            dropBlocks();
            fillEmptySpaces();
        }

        if (!foundMatch) {
            swap(board[r1][c1], board[r2][c2]);
            cout << "No match found!" << endl;
            return false;
        }

        movesLeft--;
        return true;
    }

    bool isGameOver() const {
        return movesLeft <= 0 || !hasValidMoves();
    }

    int getScore() const { return score; }
    int getMovesLeft() const { return movesLeft; }
};

#ifndef UNIT_TEST
int main() {
    AnipangGame game;
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    cout << "Welcome to Anipang!" << endl;
    cout << "Match 3 or more of the same animals!" << endl;
    cout << "Input format: row1 col1 row2 col2" << endl;
    cout << "Example: 2 3 2 4 (swap positions (2,3) and (2,4))" << endl;
    cout << "Press Enter to start...";
    cin.get();

    while (!game.isGameOver()) {
        game.display();
        int r1, c1, r2, c2;
        cout << "\nEnter swap positions (row1 col1 row2 col2): ";
        if (!(cin >> r1 >> c1 >> r2 >> c2)) {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (r1 < 0 || r1 >= BOARD_SIZE || c1 < 0 || c1 >= BOARD_SIZE ||
            r2 < 0 || r2 >= BOARD_SIZE || c2 < 0 || c2 >= BOARD_SIZE) {
            cout << "Out of bounds!" << endl;
            continue;
        }
        game.makeMove(r1, c1, r2, c2);
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    game.display();
    cout << "\n===== GAME OVER =====" << endl;
    cout << "Final Score: " << game.getScore() << endl;
    return 0;
}
#endif