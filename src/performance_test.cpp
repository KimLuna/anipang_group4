#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <iomanip>
#include "main_template.cpp" // 여기서 직접 포함

using namespace std;
using namespace chrono;

struct PerformanceResult
{
    string function_name;
    double avg_time;
    double min_time;
    double max_time;
};

void measurePerformance(const string &function_name,
                        function<void()> test_func,
                        int iterations,
                        vector<PerformanceResult> &results)
{
    vector<double> times;
    times.reserve(iterations);

    for (int i = 0; i < iterations; ++i)
    {
        auto start = high_resolution_clock::now();
        test_func();
        auto end = high_resolution_clock::now();
        times.push_back(duration_cast<microseconds>(end - start).count());
    }

    double avg = 0, min = times[0], max = times[0];
    for (double t : times)
    {
        avg += t;
        if (t < min)
            min = t;
        if (t > max)
            max = t;
    }
    avg /= iterations;

    results.push_back({function_name, avg, min, max});
}

void printResults(const vector<PerformanceResult> &results)
{
    cout << "\n=== Performance Results for main_template.cpp ===\n";
    cout << setw(20) << "Function" << " | "
         << setw(15) << "Avg Time (μs)" << " | "
         << setw(15) << "Min Time (μs)" << " | "
         << setw(15) << "Max Time (μs)" << "\n";
    cout << string(70, '-') << "\n";

    for (const auto &r : results)
    {
        cout << setw(20) << r.function_name << " | "
             << setw(15) << fixed << setprecision(2) << r.avg_time << " | "
             << setw(15) << r.min_time << " | "
             << setw(15) << r.max_time << "\n";
    }
}

void saveResults(const vector<PerformanceResult> &results)
{
    ofstream file("../data/benchmark_results.txt");

    file << "=== Performance Results for main_template.cpp ===\n";
    file << setw(20) << "Function" << " | "
         << setw(15) << "Avg Time (μs)" << " | "
         << setw(15) << "Min Time (μs)" << " | "
         << setw(15) << "Max Time (μs)" << "\n";
    file << string(70, '-') << "\n";

    for (const auto &r : results)
    {
        file << setw(20) << r.function_name << " | "
             << setw(15) << fixed << setprecision(2) << r.avg_time << " | "
             << setw(15) << r.min_time << " | "
             << setw(15) << r.max_time << "\n";
    }

    file.close();
}

int main()
{
    const int ITERATIONS = 100;
    vector<PerformanceResult> results;

    AnipangGame game;

    measurePerformance("hasValidMoves", [&]()
                       { game.hasValidMoves(); }, ITERATIONS, results);
    measurePerformance("dropBlocks", [&]()
                       { game.dropBlocks(); }, ITERATIONS, results);
    measurePerformance("fillEmptySpaces", [&]()
                       { game.fillEmptySpaces(); }, ITERATIONS, results);

    printResults(results);
    saveResults(results);

    return 0;
}
