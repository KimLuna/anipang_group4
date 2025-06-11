#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <array>
#include <stack>
#include <set>
#include <queue>

using namespace std;

// [4] 구조 최적화: string 대신 enum 사용
enum Animal { DOG, CAT, RABBIT, PANDA, KOALA, EMPTY };

const array<string, 6> ANIMAL_EMOJIS = {"🐶", "🐱", "🐰", "🐼", "🐨", "  "};
const int BOARD_SIZE = 7;
const int ANIMAL_TYPES = 5;
const int INITIAL_MOVES = 30;
const int MIN_MATCH = 3;

const vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // [5] 방향 벡터 사용

class AnipangGame {
private:
    Animal board[BOARD_SIZE][BOARD_SIZE]; // [4] 구조 최적화 적용됨
    int score;
    int movesLeft;

    void clearScreen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    bool isInBounds(int r, int c) {
        return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
    }

    // [5] checkMatchAtPosition 개선: 중복 제거, 방향 벡터 적용
    bool checkMatchAtPosition(int row, int col) {
        Animal curr = board[row][col];
        if (curr == EMPTY) return false;

        for (auto [dr, dc] : directions) {
            int cnt = 1;
            for (int i = 1; i < 3; ++i) {
                int nr = row + dr * i, nc = col + dc * i;
                if (!isInBounds(nr, nc) || board[nr][nc] != curr) break;
                cnt++;
            }
            for (int i = 1; i < 3; ++i) {
                int nr = row - dr * i, nc = col - dc * i;
                if (!isInBounds(nr, nc) || board[nr][nc] != curr) break;
                cnt++;
            }
            if (cnt >= MIN_MATCH) return true;
        }
        return false;
    }

    // [2] 반복 DFS (stack 기반)
    int countConnectedAnimals(int row, int col, Animal animal, vector<vector<bool>>& visited) {
        stack<pair<int, int>> s;
        s.push({row, col});
        visited[row][col] = true;
        int count = 0;

        while (!s.empty()) {
            auto [r, c] = s.top(); s.pop();
            count++;
            for (auto [dr, dc] : directions) {
                int nr = r + dr, nc = c + dc;
                if (isInBounds(nr, nc) && !visited[nr][nc] && board[nr][nc] == animal) {
                    visited[nr][nc] = true;
                    s.push({nr, nc});
                }
            }
        }
        return count;
    }

    // [3] dropBlocks 최적화: 1-pass gravity
    void dropBlocks() {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            int emptyRow = BOARD_SIZE - 1;
            for (int row = BOARD_SIZE - 1; row >= 0; --row) {
                if (board[row][col] != EMPTY) {
                    board[emptyRow][col] = board[row][col];
                    if (emptyRow != row) board[row][col] = EMPTY;
                    emptyRow--;
                }
            }
        }
    }

    // [7] removeMatchesAt 최적화: 중복 제거 및 연결 요소 통합
    int removeAllMatches() {
        set<pair<int, int>> toRemove; // [6] 중복 방지를 위한 set 사용

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                Animal a = board[i][j];
                if (a == EMPTY) continue;

                for (auto [dr, dc] : directions) {
                    int cnt = 1;
                    vector<pair<int, int>> match = {{i, j}};

                    for (int k = 1; k < BOARD_SIZE; ++k) {
                        int ni = i + dr * k, nj = j + dc * k;
                        if (!isInBounds(ni, nj) || board[ni][nj] != a) break;
                        match.push_back({ni, nj});
                        cnt++;
                    }
                    if (cnt >= MIN_MATCH) toRemove.insert(match.begin(), match.end());
                }
            }
        }

        for (auto [r, c] : toRemove) board[r][c] = EMPTY;
        return toRemove.size();
    }

    // [1] hasValidMoves 최적화: 전체 복사 제거 + 영향 위치만 검사
    bool hasValidMoves() {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                for (auto [dr, dc] : directions) {
                    int ni = i + dr, nj = j + dc;
                    if (!isInBounds(ni, nj)) continue;
                    swap(board[i][j], board[ni][nj]);
                    if (checkMatchAtPosition(i, j) || checkMatchAtPosition(ni, nj)) {
                        swap(board[i][j], board[ni][nj]);
                        return true;
                    }
                    swap(board[i][j], board[ni][nj]);
                }
            }
        }
        return false;
    }

public:
    AnipangGame() : score(0), movesLeft(INITIAL_MOVES) {
        srand(static_cast<unsigned>(time(nullptr)));
        initializeBoard();
    }

    void initializeBoard() {
        do {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    board[i][j] = static_cast<Animal>(rand() % ANIMAL_TYPES);
                }
            }
        } while (removeAllMatches() > 0); // [8] 초기 보드 중복 매치 제거만 1회 수행
    }

    void display() const {
        clearScreen();
        cout << "\n===== ANIPANG GAME =====" << endl;
        cout << "Score: " << score << " | Moves Left: " << movesLeft << endl;
        cout << "\n   ";
        for (int j = 0; j < BOARD_SIZE; j++) cout << setw(4) << j;
        cout << endl << "  ┌";
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
};
    int main() {
    AnipangGame game;
    game.display();  // 초기 보드 출력
    return 0;
}
