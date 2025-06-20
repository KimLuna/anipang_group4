#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// ===== Game Constants =====
const int BOARD_SIZE = 7;
const int ANIMAL_TYPES = 5;
const string ANIMALS[] = {"🐶", "🐱", "🐰", "🐼", "🐨"};
const string EMPTY = "  ";
const int INITIAL_MOVES = 30;
const int MIN_MATCH = 3;

// ===== INEFFICIENT VERSION FOR STUDENTS TO IMPROVE =====
class AnipangGame {
private:
    string board[BOARD_SIZE][BOARD_SIZE]; 
    int score;
    int movesLeft;
    
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
        // Board initialization
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                board[i][j] = ANIMALS[rand() % ANIMAL_TYPES];
            }
        }
        
        // Remove initial matches
        bool hasMatch = true;
        while (hasMatch) {
            hasMatch = false;
            
            // Check matches at all positions
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (checkMatchAtPosition(i, j)) {
                        hasMatch = true;
                        // Clear matched parts
                        removeMatchesAt(i, j);
                    }
                }
            }
            
            // Fill empty spaces
            fillEmptySpaces();
        }
    }
    
    void display() const {
        clearScreen();
        cout << "\n===== ANIPANG GAME =====" << endl;
        cout << "Score: " << score << " | Moves Left: " << movesLeft << endl;
        cout << "\n   ";
        
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << setw(4) << j;
        }
        cout << endl;
        
        cout << "  ┌";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << "────";
        }
        cout << "┐" << endl;
        
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << setw(2) << i << "│";
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << " " << board[i][j] << " ";
            }
            cout << "│" << endl;
        }
        
        cout << "  └";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << "────";
        }
        cout << "┘" << endl;
    }
    
    // Implemented with recursion
    int countConnectedAnimals(int row, int col, string animal, bool visited[BOARD_SIZE][BOARD_SIZE]) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            return 0;
        }
        
        if (visited[row][col] || board[row][col] != animal) {
            return 0;
        }
        
        visited[row][col] = true;
        
        // Recursive calls
        return 1 + 
            countConnectedAnimals(row - 1, col, animal, visited) +
            countConnectedAnimals(row + 1, col, animal, visited) +
            countConnectedAnimals(row, col - 1, animal, visited) +
            countConnectedAnimals(row, col + 1, animal, visited);
    }
    
    bool checkMatchAtPosition(int row, int col) {
        if (board[row][col] == EMPTY) return false;
        
        // Horizontal check - hardcoded
        int count = 1;
        
        // Check left
        if (col > 0 && board[row][col-1] == board[row][col]) {
            count++;
            if (col > 1 && board[row][col-2] == board[row][col]) {
                count++;
                if (col > 2 && board[row][col-3] == board[row][col]) {
                    count++;
                }
            }
        }
        
        // Check right
        if (col < BOARD_SIZE-1 && board[row][col+1] == board[row][col]) {
            count++;
            if (col < BOARD_SIZE-2 && board[row][col+2] == board[row][col]) {
                count++;
                if (col < BOARD_SIZE-3 && board[row][col+3] == board[row][col]) {
                    count++;
                }
            }
        }
        
        if (count >= MIN_MATCH) return true;
        
        // Vertical check - same hardcoding
        count = 1;
        
        // Check up
        if (row > 0 && board[row-1][col] == board[row][col]) {
            count++;
            if (row > 1 && board[row-2][col] == board[row][col]) {
                count++;
                if (row > 2 && board[row-3][col] == board[row][col]) {
                    count++;
                }
            }
        }
        
        // Check down
        if (row < BOARD_SIZE-1 && board[row+1][col] == board[row][col]) {
            count++;
            if (row < BOARD_SIZE-2 && board[row+2][col] == board[row][col]) {
                count++;
                if (row < BOARD_SIZE-3 && board[row+3][col] == board[row][col]) {
                    count++;
                }
            }
        }
        
        return count >= MIN_MATCH;
    }
    
    void removeMatchesAt(int row, int col) {
        string animal = board[row][col];
        
        // Find and remove horizontal matches - hardcoded
        int left = col, right = col;
        
        // Expand left
        while (left > 0 && board[row][left-1] == animal) {
            left--;
        }
        
        // Expand right
        while (right < BOARD_SIZE-1 && board[row][right+1] == animal) {
            right++;
        }
        
        // Remove if 3 or more
        if (right - left + 1 >= MIN_MATCH) {
            for (int c = left; c <= right; c++) {
                board[row][c] = EMPTY;
            }
            score += (right - left + 1) * 10;
        }
        
        int top = row, bottom = row;
        
        while (top > 0 && board[top-1][col] == animal) {
            top--;
        }
        
        while (bottom < BOARD_SIZE-1 && board[bottom+1][col] == animal) {
            bottom++;
        }
        
        if (bottom - top + 1 >= MIN_MATCH) {
            for (int r = top; r <= bottom; r++) {
                board[r][col] = EMPTY;
            }
            score += (bottom - top + 1) * 10;
        }
    }
    
    void dropBlocks() {
        bool moved = true;
        
        while (moved) {
            moved = false;
            
            for (int col = 0; col < BOARD_SIZE; col++) {
                for (int row = BOARD_SIZE - 2; row >= 0; row--) {
                    if (board[row][col] != EMPTY && board[row + 1][col] == EMPTY) {
                        board[row + 1][col] = board[row][col];
                        board[row][col] = EMPTY;
                        moved = true;
                    }
                }
            }
        }
    }

    void fillEmptySpaces() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = ANIMALS[rand() % ANIMAL_TYPES];
                }
            }
        }
    }
    
    bool hasValidMoves() const {
        // Try all swaps at all positions
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                // Swap with right
                if (j < BOARD_SIZE - 1) {
                    // Copy temporary board 
                    string tempBoard[BOARD_SIZE][BOARD_SIZE];
                    for (int r = 0; r < BOARD_SIZE; r++) {
                        for (int c = 0; c < BOARD_SIZE; c++) {
                            tempBoard[r][c] = board[r][c];
                        }
                    }
                    
                    // Swap
                    string temp = tempBoard[i][j];
                    tempBoard[i][j] = tempBoard[i][j + 1];
                    tempBoard[i][j + 1] = temp;
                    
                    // Check for matches
                    bool foundMatch = false;
                    for (int r = 0; r < BOARD_SIZE && !foundMatch; r++) {
                        for (int c = 0; c < BOARD_SIZE && !foundMatch; c++) {
                            if (checkMatchAtPositionConst(r, c, tempBoard)) {
                                foundMatch = true;
                            }
                        }
                    }
                    
                    if (foundMatch) return true;
                }
                
                // Swap with bottom
                if (i < BOARD_SIZE - 1) {
                    // Copy temporary board
                    string tempBoard[BOARD_SIZE][BOARD_SIZE];
                    for (int r = 0; r < BOARD_SIZE; r++) {
                        for (int c = 0; c < BOARD_SIZE; c++) {
                            tempBoard[r][c] = board[r][c];
                        }
                    }
                    
                    // Swap
                    string temp = tempBoard[i][j];
                    tempBoard[i][j] = tempBoard[i + 1][j];
                    tempBoard[i + 1][j] = temp;
                    
                    // Check for matches
                    bool foundMatch = false;
                    for (int r = 0; r < BOARD_SIZE && !foundMatch; r++) {
                        for (int c = 0; c < BOARD_SIZE && !foundMatch; c++) {
                            if (checkMatchAtPositionConst(r, c, tempBoard)) {
                                foundMatch = true;
                            }
                        }
                    }
                    
                    if (foundMatch) return true;
                }
            }
        }
        return false;
    }
    
    // Const version of match check function
    bool checkMatchAtPositionConst(int row, int col, string tempBoard[BOARD_SIZE][BOARD_SIZE]) const {
        if (tempBoard[row][col] == EMPTY) return false;
        
        // Horizontal check
        int count = 1;
        
        // Check left
        if (col > 0 && tempBoard[row][col-1] == tempBoard[row][col]) {
            count++;
            if (col > 1 && tempBoard[row][col-2] == tempBoard[row][col]) {
                count++;
                if (col > 2 && tempBoard[row][col-3] == tempBoard[row][col]) {
                    count++;
                }
            }
        }
        
        // Check right
        if (col < BOARD_SIZE-1 && tempBoard[row][col+1] == tempBoard[row][col]) {
            count++;
            if (col < BOARD_SIZE-2 && tempBoard[row][col+2] == tempBoard[row][col]) {
                count++;
                if (col < BOARD_SIZE-3 && tempBoard[row][col+3] == tempBoard[row][col]) {
                    count++;
                }
            }
        }
        
        if (count >= MIN_MATCH) return true;
        
        // Vertical check
        count = 1;
        
        // Check up
        if (row > 0 && tempBoard[row-1][col] == tempBoard[row][col]) {
            count++;
            if (row > 1 && tempBoard[row-2][col] == tempBoard[row][col]) {
                count++;
                if (row > 2 && tempBoard[row-3][col] == tempBoard[row][col]) {
                    count++;
                }
            }
        }
        
        // Check down
        if (row < BOARD_SIZE-1 && tempBoard[row+1][col] == tempBoard[row][col]) {
            count++;
            if (row < BOARD_SIZE-2 && tempBoard[row+2][col] == tempBoard[row][col]) {
                count++;
                if (row < BOARD_SIZE-3 && tempBoard[row+3][col] == tempBoard[row][col]) {
                    count++;
                }
            }
        }
        
        return count >= MIN_MATCH;
    }
    
    bool makeMove(int r1, int c1, int r2, int c2) {
        // Adjacent check - hardcoded
        if (!((r1 == r2 && abs(c1 - c2) == 1) || 
              (c1 == c2 && abs(r1 - r2) == 1))) {
            cout << "Not adjacent positions!" << endl;
            return false;
        }
        
        // Swap
        string temp = board[r1][c1];
        board[r1][c1] = board[r2][c2];
        board[r2][c2] = temp;
        
        // Check matches - inefficient!
        bool foundMatch = false;
        
        while (true) {
            bool matchFound = false;
            
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (checkMatchAtPosition(i, j)) {
                        removeMatchesAt(i, j);
                        matchFound = true;
                        foundMatch = true;
                    }
                }
            }
            
            if (!matchFound) break;
            
            dropBlocks();
            fillEmptySpaces();
        }
        
        if (!foundMatch) {
            // Restore original
            temp = board[r1][c1];
            board[r1][c1] = board[r2][c2];
            board[r2][c2] = temp;
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

// ===== Main Game Loop =====
#ifndef UNIT_TEST
int main() {
    AnipangGame game;
    
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