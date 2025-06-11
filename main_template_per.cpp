#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <random>
#include <chrono>
#include <iomanip>
using namespace std;

// ===== Game Constants =====
const int BOARD_SIZE = 7;
const int ANIMAL_TYPES = 5;
const int INITIAL_MOVES = 30;
const int MIN_MATCH = 3;

enum AnimalType
{
    DOG,
    CAT,
    RABBIT,
    PANDA,
    KOALA,
    NONE
}; // [INEFF#4]
const string ANIMAL_EMOJI[] = {"🐶", "🐱", "🐰", "🐼", "🐨", "  "};

class AnipangGame
{
private:
    AnimalType board[BOARD_SIZE][BOARD_SIZE]; // [INEFF#4]
    int score;
    int movesLeft;
    mt19937 rng;

    void clearScreen() const
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // [INEFF#2] Iterative DFS (stack) to count connected animals
    int countConnectedAnimals(int row, int col, AnimalType animal, bool visited[BOARD_SIZE][BOARD_SIZE]) const
    {
        if (animal == NONE)
            return 0;
        int cnt = 0;
        vector<pair<int, int>> stack;

        stack.push_back({row, col});
        while (!stack.empty())
        {
            auto [r, c] = stack.back();
            stack.pop_back();
            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE)
                continue;
            if (visited[r][c] || board[r][c] != animal)
                continue;
            visited[r][c] = true;
            cnt++;
            static const int dr[] = {-1, 1, 0, 0};
            static const int dc[] = {0, 0, -1, 1};
            for (int d = 0; d < 4; ++d)
            {
                stack.push_back({r + dr[d], c + dc[d]});
            }
        }
        return cnt;
    }

    // [INEFF#5] Loop-based, direction vector match checking
    bool checkMatchAtPosition(int row, int col) const
    {
        if (board[row][col] == NONE)
            return false;
        static const int dirs[2][2] = {{0, 1}, {1, 0}}; // horizontal, vertical
        for (int d = 0; d < 2; ++d)
        {
            int cnt = 1;
            for (int sign = -1; sign <= 1; sign += 2)
            {
                int r = row + dirs[d][0] * sign, c = col + dirs[d][1] * sign;
                while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == board[row][col])
                {
                    cnt++;
                    r += dirs[d][0] * sign;
                    c += dirs[d][1] * sign;
                }
            }
            if (cnt >= MIN_MATCH)
                return true;
        }
        return false;
    }

    // [INEFF#7] Unified match detection, set for duplicates
    set<pair<int, int>> findAllMatches() const
    {
        set<pair<int, int>> matched;
        static const int dirs[2][2] = {{0, 1}, {1, 0}};
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            for (int j = 0; j < BOARD_SIZE; ++j)
            {
                if (board[i][j] == NONE)
                    continue;
                for (int d = 0; d < 2; ++d)
                {
                    vector<pair<int, int>> temp;
                    temp.push_back({i, j});
                    int ni = i + dirs[d][0], nj = j + dirs[d][1];
                    while (ni >= 0 && ni < BOARD_SIZE && nj >= 0 && nj < BOARD_SIZE && board[ni][nj] == board[i][j])
                    {
                        temp.push_back({ni, nj});
                        ni += dirs[d][0];
                        nj += dirs[d][1];
                    }
                    if (temp.size() >= MIN_MATCH)
                    {
                        matched.insert(temp.begin(), temp.end());
                    }
                }
            }
        }
        return matched;
    }

    // [INEFF#3] Efficient gravity: two-pointer method
    void dropBlocks()
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            int writeRow = BOARD_SIZE - 1;
            for (int row = BOARD_SIZE - 1; row >= 0; --row)
            {
                if (board[row][col] != NONE)
                {
                    board[writeRow--][col] = board[row][col];
                }
            }
            while (writeRow >= 0)
            {
                board[writeRow--][col] = NONE;
            }
        }
    }

    // [INEFF#8] Only fill empty spaces, no full scan
    void fillEmptySpaces()
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            for (int i = 0; i < BOARD_SIZE; ++i)
            {
                if (board[i][j] == NONE)
                {
                    board[i][j] = static_cast<AnimalType>(rng() % ANIMAL_TYPES);
                }
            }
        }
    }

    // [INEFF#1] Pattern-based valid move check, no full copy
    bool hasValidMoves() const
    {
        static const int dr[] = {0, 1};
        static const int dc[] = {1, 0};
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            for (int j = 0; j < BOARD_SIZE; ++j)
            {
                for (int d = 0; d < 2; ++d)
                {
                    int ni = i + dr[d], nj = j + dc[d];
                    if (ni < BOARD_SIZE && nj < BOARD_SIZE && board[i][j] != board[ni][nj])
                    {
                        // swap
                        AnimalType t = board[i][j];
                        const_cast<AnimalType &>(board[i][j]) = board[ni][nj];
                        const_cast<AnimalType &>(board[ni][nj]) = t;
                        bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(ni, nj);
                        t = board[i][j];
                        const_cast<AnimalType &>(board[i][j]) = board[ni][nj];
                        const_cast<AnimalType &>(board[ni][nj]) = t;
                        if (match)
                            return true;
                    }
                }
            }
        }
        return false;
    }

    // [INEFF#6] Set 사용, 중복 제거 및 효율적 처리
    void removeMatches(const set<pair<int, int>> &matched)
    {
        for (auto [i, j] : matched)
        {
            board[i][j] = NONE;
            score += 10;
        }
    }

public:
    AnipangGame() : score(0), movesLeft(INITIAL_MOVES), rng(random_device{}())
    {
        initializeBoard();
    }

    // [INEFF#8] 효율적 초기화: 매치가 없을 때까지 반복
    void initializeBoard()
    {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j)
                board[i][j] = static_cast<AnimalType>(rng() % ANIMAL_TYPES);

        while (true)
        {
            auto matched = findAllMatches();
            if (matched.empty())
                break;
            removeMatches(matched);
            dropBlocks();
            fillEmptySpaces();
        }
    }

    void display() const
    {
        clearScreen();
        cout << "\n===== ANIPANG GAME =====" << endl;
        cout << "Score: " << score << " | Moves Left: " << movesLeft << endl;
        cout << "\n   ";
        for (int j = 0; j < BOARD_SIZE; j++)
            cout << setw(4) << j;
        cout << endl
             << "  ┌";
        for (int j = 0; j < BOARD_SIZE; j++)
            cout << "────";
        cout << "┐" << endl;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            cout << setw(2) << i << "│";
            for (int j = 0; j < BOARD_SIZE; j++)
                cout << " " << ANIMAL_EMOJI[board[i][j]] << " ";
            cout << "│" << endl;
        }
        cout << "  └";
        for (int j = 0; j < BOARD_SIZE; j++)
            cout << "────";
        cout << "┘" << endl;
    }

    // [INEFF#7] makeMove에서 전체 보드 스캔 대신 변화 영역만 처리
    bool makeMove(int r1, int c1, int r2, int c2)
    {
        if (!((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1)))
        {
            cout << "Not adjacent positions!" << endl;
            return false;
        }
        swap(board[r1][c1], board[r2][c2]);
        auto matched = findAllMatches();
        if (matched.empty())
        {
            swap(board[r1][c1], board[r2][c2]);
            cout << "No match found!" << endl;
            return false;
        }
        movesLeft--;
        while (!matched.empty())
        {
            removeMatches(matched);
            dropBlocks();
            fillEmptySpaces();
            matched = findAllMatches();
        }
        return true;
    }

    bool isGameOver() const { return movesLeft <= 0 || !hasValidMoves(); }
    int getScore() const { return score; }
    int getMovesLeft() const { return movesLeft; }
};

// ===== Main Game Loop with Timing (for measurement) =====
int main()
{
    AnipangGame game;

    cout << "Welcome to Anipang!" << endl;
    cout << "Match 3 or more of the same animals!" << endl;
    cout << "Input format: row1 col1 row2 col2" << endl;
    cout << "Example: 2 3 2 4 (swap positions (2,3) and (2,4))" << endl;
    cout << "Press Enter to start...";
    cin.get();

    while (!game.isGameOver())
    {
        game.display();

        int r1, c1, r2, c2;
        cout << "\nEnter swap positions (row1 col1 row2 col2): ";

        auto start = chrono::high_resolution_clock::now(); // [INEFF#Performance Measurement]

        if (!(cin >> r1 >> c1 >> r2 >> c2))
        {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (r1 < 0 || r1 >= BOARD_SIZE || c1 < 0 || c1 >= BOARD_SIZE ||
            r2 < 0 || r2 >= BOARD_SIZE || c2 < 0 || c2 >= BOARD_SIZE)
        {
            cout << "Out of bounds!" << endl;
            continue;
        }

        game.makeMove(r1, c1, r2, c2);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Execution time: " << duration.count() << " μs" << endl;

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    game.display();
    cout << "\n===== GAME OVER =====" << endl;
    cout << "Final Score: " << game.getScore() << endl;

    return 0;
}
