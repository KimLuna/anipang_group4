#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <iomanip>
#include <sys/resource.h>
#include <functional>
#define UNIT_TEST
#include "main_template_org.cpp"  // 원본 또는 최적화 버전으로 교체

using namespace std;
using namespace chrono;

struct PerfResult {
    string name;
    double avgTimeMs;
};

struct MemResult {
    string name;
    long memDeltaKB;
};

// 메모리 사용량 측정 (Linux 기준)
long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

// 함수 실행 전후 메모리 사용량 차이
long getPeakMemoryDuring(function<void()> func) {
    long before = getMemoryUsageKB();
    func();
    long after = getMemoryUsageKB();
    return after - before;
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
    return total / iterations / 1'000'000.0;  // ns -> ms
}

// 테스트용 보드 수동 설정 함수
void setBlock(AnipangGame& game, int row, int col, const string& val) {
    string (*board)[7][7] = (string(*)[7][7]) &game;
    (*board)[row][col] = val;
}

// 시간 측정 함수들
PerfResult testHasValidMoves() {
    AnipangGame game;
    double avg = measureTimeMs([&]() { game.hasValidMoves(); }, 1000);
    return {"hasValidMoves()", avg};
}

PerfResult testDropBlocks() {
    AnipangGame game;
    setBlock(game, 6, 0, "  ");
    setBlock(game, 5, 0, "  ");
    double avg = measureTimeMs([&]() { game.dropBlocks(); }, 1000);
    return {"dropBlocks()", avg};
}

PerfResult testCountConnectedAnimals() {
    AnipangGame game;
    setBlock(game, 0, 0, "🐶");
    setBlock(game, 0, 1, "🐶");
    setBlock(game, 0, 2, "🐶");
    setBlock(game, 1, 0, "🐶");

    double total = 0;
    const int iterations = 1000;
    for (int i = 0; i < iterations; ++i) {
        bool visited[7][7] = {};
        auto start = high_resolution_clock::now();
        game.countConnectedAnimals(0, 0, "🐶", visited);
        auto end = high_resolution_clock::now();
        total += duration_cast<nanoseconds>(end - start).count();
    }
    return {"countConnectedAnimals()", total / iterations / 1'000'000.0};
}

// 메모리 변화 측정 함수들
MemResult testMemoryDuringHasValidMoves() {
    AnipangGame game;
    long delta = getPeakMemoryDuring([&]() {
        for (int i = 0; i < 1000; ++i) game.hasValidMoves();
    });
    return {"hasValidMoves()", delta};
}

MemResult testMemoryDuringDropBlocks() {
    AnipangGame game;
    setBlock(game, 6, 0, "  ");
    setBlock(game, 5, 0, "  ");
    long delta = getPeakMemoryDuring([&]() {
        for (int i = 0; i < 1000; ++i) game.dropBlocks();
    });
    return {"dropBlocks()", delta};
}

MemResult testMemoryDuringCountConnectedAnimals() {
    AnipangGame game;
    setBlock(game, 0, 0, "🐶");
    setBlock(game, 0, 1, "🐶");
    setBlock(game, 0, 2, "🐶");
    setBlock(game, 1, 0, "🐶");
    long delta = getPeakMemoryDuring([&]() {
        for (int i = 0; i < 1000; ++i) {
            bool visited[7][7] = {};
            game.countConnectedAnimals(0, 0, "🐶", visited);
        }
    });
    return {"countConnectedAnimals()", delta};
}

int main() {
    vector<PerfResult> timeResults;
    vector<MemResult> memResults;

    cout << "=== Anipang Original Performance Test ===" << endl;

    // 시간 측정
    timeResults.push_back(testHasValidMoves());
    timeResults.push_back(testDropBlocks());
    timeResults.push_back(testCountConnectedAnimals());

    // 메모리 변화 측정
    memResults.push_back(testMemoryDuringHasValidMoves());
    memResults.push_back(testMemoryDuringDropBlocks());
    memResults.push_back(testMemoryDuringCountConnectedAnimals());

    // 출력
    cout << "\n--- Average Execution Time ---" << endl;
    for (const auto& res : timeResults) {
        cout << setw(25) << left << res.name
             << ": " << scientific << setprecision(3) << res.avgTimeMs << " ms" << endl;
    }

    cout << "\n--- Memory Delta (Function Execution) ---" << endl;
    for (const auto& res : memResults) {
        cout << setw(25) << left << res.name
             << ": " << res.memDeltaKB << " KB" << endl;
    }

    // 파일 출력
    ofstream out("benchmark_results.txt", ios::app);
    out << "\n# Anipang Performance Benchmark (Original)\n";

    for (const auto& res : timeResults) {
        out << res.name << ": " << res.avgTimeMs << " ms\n";
    }

    out << "\n# Memory Delta During Execution\n";
    for (const auto& res : memResults) {
        out << res.name << ": " << res.memDeltaKB << " KB\n";
    }

    out.close();

    return 0;
}
