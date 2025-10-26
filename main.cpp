#include "Benchmark.h"
#include <iostream>
#include <fstream>

std::string generate_random_text(size_t n) {
    std::string text;
    text.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        text += 'a' + (rand() % 4);
    }
    return text;
}

int main() {
    std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
    std::string pattern = "abc";

    for (size_t n : sizes) {
        std::cout << "\n=== Test con n = " << n << " ===\n";
        std::string text = generate_random_text(n);

        auto results = Benchmark::compare_all(text, pattern);
        Benchmark::print_results(results);
        Benchmark::export_csv(results, "results_" + std::to_string(n) + ".csv");
    }

    return 0;
}
