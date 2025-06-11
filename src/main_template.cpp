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

extern "C" void runPerformanceTests(std::ostream &logStream)
{
    const int ITER = 100;

    AnipangGame game;

    // Time: hasValidMoves
    auto start1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITER; i++)
        game.hasValidMoves();
    auto end1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count() / ITER;

    logStream << "hasValidMoves avg time: " << dur1 << " μs" << endl;

    // Time: dropBlocks
    auto start2 = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITER; i++)
        game.dropBlocks(); // or setup test case
    auto end2 = chrono::high_resolution_clock::now();
    auto dur2 = chrono::duration_cast<chrono::microseconds>(end2 - start2).count() / ITER;

    logStream << "dropBlocks avg time: " << dur2 << " μs" << endl;
}

class AnipangGame
{
private:
    string board[BOARD_SIZE][BOARD_SIZE];
    int score;
    int movesLeft;

    void clearScreen() const
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

public:
    AnipangGame() : score(0), movesLeft(INITIAL_MOVES)
    {
        srand(time(nullptr));
        initializeBoard();
    }

    void initializeBoard()
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                board[i][j] = ANIMALS[rand() % ANIMAL_TYPES];
            }
        }

        bool hasMatch = true;
        while (hasMatch)
        {
            hasMatch = false;
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    if (checkMatchAtPosition(i, j))
                    {
                        hasMatch = true;
                        removeMatchesAt(i, j);
                    }
                }
            }
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
        {
            cout << setw(4) << j;
        }
        cout << endl;

        cout << "  ┌";
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            cout << "────";
        }
        cout << "┐" << endl;

        for (int i = 0; i < BOARD_SIZE; i++)
        {
            cout << setw(2) << i << "│";
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                cout << " " << board[i][j] << " ";
            }
            cout << "│" << endl;
        }

        cout << "  └";
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            cout << "────";
        }
        cout << "┘" << endl;
    }

    int countConnectedAnimals(int row, int col, string animal, bool visited[BOARD_SIZE][BOARD_SIZE])
    {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
            return 0;
        if (visited[row][col] || board[row][col] != animal)
            return 0;

        visited[row][col] = true;
        return 1 +
               countConnectedAnimals(row - 1, col, animal, visited) +
               countConnectedAnimals(row + 1, col, animal, visited) +
               countConnectedAnimals(row, col - 1, animal, visited) +
               countConnectedAnimals(row, col + 1, animal, visited);
    }

    // ✅ [Fixed const qualifier]
    bool checkMatchAtPosition(int row, int col) const
    {
        if (board[row][col] == EMPTY)
            return false;

        int count = 1;
        if (col > 0 && board[row][col - 1] == board[row][col])
        {
            count++;
            if (col > 1 && board[row][col - 2] == board[row][col])
            {
                count++;
                if (col > 2 && board[row][col - 3] == board[row][col])
                    count++;
            }
        }
        if (col < BOARD_SIZE - 1 && board[row][col + 1] == board[row][col])
        {
            count++;
            if (col < BOARD_SIZE - 2 && board[row][col + 2] == board[row][col])
            {
                count++;
                if (col < BOARD_SIZE - 3 && board[row][col + 3] == board[row][col])
                    count++;
            }
        }
        if (count >= MIN_MATCH)
            return true;

        count = 1;
        if (row > 0 && board[row - 1][col] == board[row][col])
        {
            count++;
            if (row > 1 && board[row - 2][col] == board[row][col])
            {
                count++;
                if (row > 2 && board[row - 3][col] == board[row][col])
                    count++;
            }
        }
        if (row < BOARD_SIZE - 1 && board[row + 1][col] == board[row][col])
        {
            count++;
            if (row < BOARD_SIZE - 2 && board[row + 2][col] == board[row][col])
            {
                count++;
                if (row < BOARD_SIZE - 3 && board[row + 3][col] == board[row][col])
                    count++;
            }
        }
        return count >= MIN_MATCH;
    }

    void removeMatchesAt(int row, int col)
    {
        string animal = board[row][col];
        int left = col, right = col;

        while (left > 0 && board[row][left - 1] == animal)
            left--;
        while (right < BOARD_SIZE - 1 && board[row][right + 1] == animal)
            right++;

        if (right - left + 1 >= MIN_MATCH)
        {
            for (int c = left; c <= right; c++)
            {
                board[row][c] = EMPTY;
            }
            score += (right - left + 1) * 10;
        }

        int top = row, bottom = row;
        while (top > 0 && board[top - 1][col] == animal)
            top--;
        while (bottom < BOARD_SIZE - 1 && board[bottom + 1][col] == animal)
            bottom++;

        if (bottom - top + 1 >= MIN_MATCH)
        {
            for (int r = top; r <= bottom; r++)
            {
                board[r][col] = EMPTY;
            }
            score += (bottom - top + 1) * 10;
        }
    }

    void dropBlocks()
    {
        bool moved = true;
        while (moved)
        {
            moved = false;
            for (int col = 0; col < BOARD_SIZE; col++)
            {
                for (int row = BOARD_SIZE - 2; row >= 0; row--)
                {
                    if (board[row][col] != EMPTY && board[row + 1][col] == EMPTY)
                    {
                        board[row + 1][col] = board[row][col];
                        board[row][col] = EMPTY;
                        moved = true;
                    }
                }
            }
        }
    }

    void fillEmptySpaces()
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == EMPTY)
                {
                    board[i][j] = ANIMALS[rand() % ANIMAL_TYPES];
                }
            }
        }
    }

    // ✅ [IE#1] Optimized hasValidMoves with const correctness
    bool hasValidMoves() const
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (j < BOARD_SIZE - 1)
                {
                    swap(const_cast<string &>(board[i][j]), const_cast<string &>(board[i][j + 1]));
                    bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(i, j + 1);
                    swap(const_cast<string &>(board[i][j]), const_cast<string &>(board[i][j + 1]));
                    if (match)
                        return true;
                }
                if (i < BOARD_SIZE - 1)
                {
                    swap(const_cast<string &>(board[i][j]), const_cast<string &>(board[i + 1][j]));
                    bool match = checkMatchAtPosition(i, j) || checkMatchAtPosition(i + 1, j);
                    swap(const_cast<string &>(board[i][j]), const_cast<string &>(board[i + 1][j]));
                    if (match)
                        return true;
                }
            }
        }
        return false;
    }

    bool makeMove(int r1, int c1, int r2, int c2)
    {
        if (!((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1)))
        {
            cout << "Not adjacent positions!" << endl;
            return false;
        }

        string temp = board[r1][c1];
        board[r1][c1] = board[r2][c2];
        board[r2][c2] = temp;

        bool foundMatch = false;
        while (true)
        {
            bool matchFound = false;
            for (int i = 0; i < BOARD_SIZE; i++)
            {
                for (int j = 0; j < BOARD_SIZE; j++)
                {
                    if (checkMatchAtPosition(i, j))
                    {
                        removeMatchesAt(i, j);
                        matchFound = true;
                        foundMatch = true;
                    }
                }
            }
            if (!matchFound)
                break;
            dropBlocks();
            fillEmptySpaces();
        }

        if (!foundMatch)
        {
            temp = board[r1][c1];
            board[r1][c1] = board[r2][c2];
            board[r2][c2] = temp;
            cout << "No match found!" << endl;
            return false;
        }

        movesLeft--;
        return true;
    }

    bool isGameOver() const
    {
        return movesLeft <= 0 || !hasValidMoves();
    }

    int getScore() const { return score; }
    int getMovesLeft() const { return movesLeft; }
};

// ===== Main Game Loop =====
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
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
    game.display();
    cout << "\n===== GAME OVER =====" << endl;
    cout << "Final Score: " << game.getScore() << endl;
    return 0;
}
