#include "Benchmark.h"
#include "SuffixArray.h"
#include "KMP.h"
#include "BoyerMoore.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std::chrono;

std::vector<BenchmarkResult> Benchmark::compare_all(const std::string& text, const std::string& pattern) {
    std::vector<BenchmarkResult> results;

    auto start = high_resolution_clock::now();
    SuffixArray sa(text);
    sa.search(pattern);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    size_t memory_sa = text.size() * sizeof(int); // sa_
    results.push_back({
        "Suffix Array",
        text.size(),
        static_cast<double>(duration.count()) / 1000.0,
        memory_sa
    });

    start = high_resolution_clock::now();
    auto matches_kmp = KMP::search(text, pattern);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    size_t memory_kmp = pattern.size() * sizeof(int); // lps array
    results.push_back({
        "KMP",
        text.size(),
        static_cast<double>(duration.count()) / 1000.0,
        memory_kmp
    });

    start = high_resolution_clock::now();
    auto matches_bm = BoyerMoore::search(text, pattern);
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start);

    size_t memory_bm = 256 * sizeof(int); // bad_char array
    results.push_back({
        "Boyer-Moore",
        text.size(),
        static_cast<double>(duration.count()) / 1000.0,
        memory_bm
    });

    return results;
}

void Benchmark::print_results(const std::vector<BenchmarkResult>& results) {
    std::cout << std::setw(15) << "Algorithm"
              << std::setw(15) << "Input Size"
              << std::setw(15) << "Time (ms)"
              << std::setw(15) << "Memory (KB)" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& result : results) {
        std::cout << std::setw(15) << result.algorithm
                  << std::setw(15) << result.input_size
                  << std::setw(15) << std::fixed << std::setprecision(3) << result.time_ms
                  << std::setw(15) << std::fixed << std::setprecision(3) << (result.memory_bytes / 1024.0) << "\n";
    }
}

void Benchmark::export_csv(const std::vector<BenchmarkResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    file << "Algorithm,Input Size,Time (ms),Memory (KB)\n";
    for (const auto& result : results) {
        file << result.algorithm << ","
             << result.input_size << ","
             << result.time_ms << ","
             << (result.memory_bytes / 1024.0) << "\n";
    }
    file.close();
}
