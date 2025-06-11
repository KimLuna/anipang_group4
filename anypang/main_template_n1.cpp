#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>

using namespace std;

namespace version1
{
    // ===== Game Constants =====
    const int BOARD_SIZE = 7;
    const int ANIMAL_TYPES = 5;
    const string ANIMALS[] = {"🐶", "🐱", "🐰", "🐼", "🐨"};
    const string EMPTY = "  ";
    const int INITIAL_MOVES = 30;
    const int MIN_MATCH = 3;

    class AnipangGame
    {
        // ... existing code ...
    };

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
            game.dropBlocks();
        auto end2 = chrono::high_resolution_clock::now();
        auto dur2 = chrono::duration_cast<chrono::microseconds>(end2 - start2).count() / ITER;

        logStream << "dropBlocks avg time: " << dur2 << " μs" << endl;
    }
}

// ===== Main Game Loop =====
int main()
{
    version1::AnipangGame game;
    // ... rest of the main function ...
}