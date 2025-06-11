#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <sys/resource.h> // for getrusage

using namespace std;
using namespace chrono;

long getMemoryUsageKB()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // KB on Linux
}

// Abstract Interface
class AnipangInterface
{
public:
    virtual void runTest() = 0;
    virtual ~AnipangInterface() = default;
};

typedef unique_ptr<AnipangInterface> GamePtr;

// Declare external creation functions
GamePtr loadGameInstance(const string &version); // dynamic loader (below)

void runBenchmark(const string &filename, const vector<string> &versions, int iterations = 100)
{
    ofstream out("data/benchmark_results.txt");
    out << "Benchmark Results for Anipang\n\n";
    out << "Version\tTime(ms)\tMemory(KB)\n";

    for (const auto &version : versions)
    {
        cout << "[*] Running: " << version << endl;
        double totalTime = 0;
        long memoryBefore = getMemoryUsageKB();

        for (int i = 0; i < iterations; ++i)
        {
            auto game = loadGameInstance(version);
            auto start = high_resolution_clock::now();
            game->runTest();
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end - start);
            totalTime += duration.count();
        }

        long memoryAfter = getMemoryUsageKB();
        double avgTime = totalTime / iterations;
        long memUsed = memoryAfter - memoryBefore;

        out << version << "\t" << avgTime << "\t" << memUsed << endl;
        cout << "[✓] " << version << ": " << avgTime << " ms, " << memUsed << " KB" << endl;
    }

    out.close();
}

// Example: game loader (manually switch based on version)
#include "main_template_n1.cpp"
#include "main_template_n2.cpp"
#include "main_template_n3.cpp"
#include "main_template_n4.cpp"
#include "main_template_n5.cpp"
#include "main_template_n6.cpp"
#include "main_template_n7.cpp"
#include "main_template_n8.cpp"

// Adapter class templates
template <typename GameClass>
class GameRunner : public AnipangInterface
{
public:
    void runTest() override
    {
        GameClass game;
        game.hasValidMoves();
        game.dropBlocks();
        game.fillEmptySpaces();
    }
};

GamePtr loadGameInstance(const string &version)
{
    if (version == "main_template_n1")
        return make_unique<GameRunner<AnipangGame_n1>>();
    if (version == "main_template_n2")
        return make_unique<GameRunner<AnipangGame_n2>>();
    if (version == "main_template_n3")
        return make_unique<GameRunner<AnipangGame_n3>>();
    if (version == "main_template_n4")
        return make_unique<GameRunner<AnipangGame_n4>>();
    if (version == "main_template_n5")
        return make_unique<GameRunner<AnipangGame_n5>>();
    if (version == "main_template_n6")
        return make_unique<GameRunner<AnipangGame_n6>>();
    if (version == "main_template_n7")
        return make_unique<GameRunner<AnipangGame_n7>>();
    if (version == "main_template_n8")
        return make_unique<GameRunner<AnipangGame_n8>>();
    throw invalid_argument("Unknown version: " + version);
}

int main()
{
    vector<string> versions = {
        "main_template_n1",
        "main_template_n2",
        "main_template_n3",
        "main_template_n4",
        "main_template_n5",
        "main_template_n6",
        "main_template_n7",
        "main_template_n8"};

    runBenchmark("benchmark_results.txt", versions, 100);
    return 0;
}
