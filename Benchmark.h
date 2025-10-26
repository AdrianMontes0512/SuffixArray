//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_BENCHMARK_H
#define UNTITLED_BENCHMARK_H


#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <functional>

struct BenchmarkResult {
    std::string algorithm;
    size_t input_size;
    double construction_time_ms;
    double search_time_ms;
    double total_time_ms;
    size_t memory_bytes;
};

class Benchmark {
public:
    static std::vector<BenchmarkResult> compare_all(
        const std::string& text,
        const std::string& pattern
    );

    static void print_results(const std::vector<BenchmarkResult>& results);
    static void export_csv(const std::vector<BenchmarkResult>& results,
                          const std::string& filename);

private:
    template<typename Func>
    static double measure_time(Func&& func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
};



#endif //UNTITLED_BENCHMARK_H