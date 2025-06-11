#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct PerformanceResult
{
    string functionName;
    double avgTime;
    double minTime;
    double maxTime;
};

void measurePerformance(const string &version, const string &functionName,
                        function<void()> testFunc, int iterations,
                        vector<PerformanceResult> &results)
{
    vector<double> times;
    times.reserve(iterations);

    for (int i = 0; i < iterations; i++)
    {
        auto start = chrono::high_resolution_clock::now();
        testFunc();
        auto end = chrono::high_resolution_clock::now();
        double duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
        times.push_back(duration);
    }

    double avg = 0, min = times[0], max = times[0];
    for (double t : times)
    {
        avg += t;
        min = std::min(min, t);
        max = std::max(max, t);
    }
    avg /= iterations;

    results.push_back({functionName, avg, min, max});
}

void printResults(const string &version, const vector<PerformanceResult> &results)
{
    cout << "\n=== Performance Results for Version " << version << " ===\n";
    cout << setw(20) << "Function" << setw(15) << "Avg (μs)"
         << setw(15) << "Min (μs)" << setw(15) << "Max (μs)" << endl;
    cout << string(65, '-') << endl;

    for (const auto &result : results)
    {
        cout << setw(20) << result.functionName
             << setw(15) << fixed << setprecision(2) << result.avgTime
             << setw(15) << result.minTime
             << setw(15) << result.maxTime << endl;
    }
}

void saveResults(const string &version, const vector<PerformanceResult> &results)
{
    string filename = "benchmark_results_" + version + ".txt";
    ofstream file(filename);

    file << "=== Performance Results for Version " << version << " ===\n";
    file << setw(20) << "Function" << setw(15) << "Avg (μs)"
         << setw(15) << "Min (μs)" << setw(15) << "Max (μs)" << endl;
    file << string(65, '-') << endl;

    for (const auto &result : results)
    {
        file << setw(20) << result.functionName
             << setw(15) << fixed << setprecision(2) << result.avgTime
             << setw(15) << result.minTime
             << setw(15) << result.maxTime << endl;
    }
}

int main()
{
    string version;
    cout << "Enter version number (1, 2, or 3): ";
    cin >> version;

    if (version != "1" && version != "2" && version != "3")
    {
        cerr << "Invalid version number. Please enter 1, 2, or 3." << endl;
        return 1;
    }

    vector<PerformanceResult> results;
    const int ITERATIONS = 100;

    // 각 버전에 맞는 테스트 실행
    if (version == "1")
    {
        version1::AnipangGame game;
        measurePerformance(version, "hasValidMoves", [&]()
                           { game.hasValidMoves(); }, ITERATIONS, results);
        measurePerformance(version, "dropBlocks", [&]()
                           { game.dropBlocks(); }, ITERATIONS, results);
    }
    // version 2와 3에 대한 테스트도 비슷한 방식으로 추가

    printResults(version, results);
    saveResults(version, results);

    return 0;
}