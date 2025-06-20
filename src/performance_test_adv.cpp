#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>
#include <sys/resource.h>
#include <functional>
#define UNIT_TEST
#include "main_template_n8.cpp"

using namespace std;
using namespace chrono;

struct PerfResult {
    string name;
    double avgTimeMs;
};

// 메모리 사용량 측정 (Linux 기준)
long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

// 성능 측정 도우미
double measureTimeMs(function<void()> func, int iterations = 1000) {
    double total = 0;
    for (int i = 0; i < iterations; ++i) {
        auto start = high_resolution_clock::now();
        func();
        auto end = high_resolution_clock::now();
        total += duration_cast<nanoseconds>(end - start).count();
    }
    return total / iterations / 1'000'000.0;  // ns → ms
}

// enum 기반 보드 수정 함수
void setBlock(AnipangGame& game, int row, int col, Animal val) {
    Animal (*board)[7][7] = (Animal(*)[7][7]) &game;
    (*board)[row][col] = val;
}

PerfResult testHasValidMoves() {
    AnipangGame game;
    double avg = measureTimeMs([&]() { game.hasValidMoves(); }, 1000);
    return {"hasValidMoves()", avg};
}

PerfResult testDropBlocks() {
    AnipangGame game;
    setBlock(game, 6, 0, EMPTY);
    setBlock(game, 5, 0, EMPTY);
    double avg = measureTimeMs([&]() { game.dropBlocks(); }, 1000);
    return {"dropBlocks()", avg};
}

PerfResult testCountConnectedAnimals() {
    AnipangGame game;
    setBlock(game, 0, 0, DOG);
    setBlock(game, 0, 1, DOG);
    setBlock(game, 1, 0, DOG);

    double total = 0;
    const int iterations = 1000;
    for (int i = 0; i < iterations; ++i) {
        bool visited[7][7] = {};
        auto start = high_resolution_clock::now();
        game.countConnectedAnimals(0, 0, DOG, visited);
        auto end = high_resolution_clock::now();
        total += duration_cast<nanoseconds>(end - start).count();
    }
    return {"countConnectedAnimals()", total / iterations / 1'000'000.0};
}

long testMemoryUsage() {
    AnipangGame game;
    return getMemoryUsageKB();
}

int main() {
    vector<PerfResult> results;
    cout << "=== Anipang Advanced Performance Test ===" << endl;

    results.push_back(testHasValidMoves());
    results.push_back(testDropBlocks());
    results.push_back(testCountConnectedAnimals());

    long memUsage = testMemoryUsage();

    for (const auto& res : results) {
        cout << setw(25) << left << res.name
             << ": " << scientific << setprecision(3) << res.avgTimeMs << " ms" << endl;
    }
    cout << "Memory Usage (RSS): " << memUsage << " KB" << endl;

    ofstream out("benchmark_results_adv.txt",ios::app);
    out << "# Anipang Performance Benchmark (Enum-Based Version)\n";
    for (const auto& res : results) {
        out << res.name << ": " << res.avgTimeMs << " ms\n";
    }
    out << "Memory Usage (RSS): " << memUsage << " KB\n";
    out.close();

    return 0;
}
