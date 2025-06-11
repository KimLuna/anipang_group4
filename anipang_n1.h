#ifndef ANIPANG_N1_H
#define ANIPANG_N1_H

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

    void setBoard(const string newBoard[BOARD_SIZE][BOARD_SIZE])
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                board[i][j] = newBoard[i][j];
            }
        }
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
    }

    string getAnimal(int row, int col) const
    {
        return board[row][col];
    }

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

    void dropBlocks()
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            int writeRow = BOARD_SIZE - 1;
            for (int row = BOARD_SIZE - 1; row >= 0; row--)
            {
                if (board[row][col] != EMPTY)
                {
                    board[writeRow][col] = board[row][col];
                    if (writeRow != row)
                        board[row][col] = EMPTY;
                    writeRow--;
                }
            }
        }
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
};

#endif // ANIPANG_N1_H