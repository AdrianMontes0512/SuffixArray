#include "Benchmark.h"
#include "BoyerMoore.h"
#include "KMP.h"
#include "SuffixArray.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std::chrono;

// Compara el desempeño de Suffix Array, KMP y Boyer-Moore
// Para cada algoritmo, medimos:
// 1. Tiempo de construcción/preprocesamiento
// 2. Tiempo de búsqueda
// 3. Memoria utilizada
// Nota importante: El Suffix Array tiene un costo de construcción alto,
// pero es muy eficiente cuando se realizan múltiples búsquedas sobre
// el mismo texto, ya que solo se construye una vez.
std::vector<BenchmarkResult>
Benchmark::compare_all(const std::string &text, const std::string &pattern) {
  std::vector<BenchmarkResult> results;

  // Benchmark de Suffix Array
  auto start_construction = high_resolution_clock::now();
  SuffixArray sa(text);
  auto end_construction = high_resolution_clock::now();
  auto construction_duration =
      duration_cast<microseconds>(end_construction - start_construction);

  auto start_search = high_resolution_clock::now();
  sa.search(pattern);
  auto end_search = high_resolution_clock::now();
  auto search_duration = duration_cast<microseconds>(end_search - start_search);

  // Memoria: almacena un arreglo de enteros del tamaño del texto
  size_t memory_sa = text.size() * sizeof(int);
  results.push_back(
      {"Suffix Array", text.size(),
       static_cast<double>(construction_duration.count()) / 1000.0,
       static_cast<double>(search_duration.count()) / 1000.0, memory_sa});

  // Benchmark de KMP
  start_construction = high_resolution_clock::now();
  auto lps = KMP::compute_lps(pattern); // Tabla de prefijos
  end_construction = high_resolution_clock::now();
  construction_duration =
      duration_cast<microseconds>(end_construction - start_construction);

  start_search = high_resolution_clock::now();
  auto matches_kmp = KMP::search(text, pattern);
  end_search = high_resolution_clock::now();
  search_duration = duration_cast<microseconds>(end_search - start_search);

  // Memoria: almacena un arreglo del tamaño del patrón
  size_t memory_kmp = pattern.size() * sizeof(int);
  results.push_back(
      {"KMP", text.size(),
       static_cast<double>(construction_duration.count()) / 1000.0,
       static_cast<double>(search_duration.count()) / 1000.0, memory_kmp});

  // Benchmark de Boyer-Moore
  start_construction = high_resolution_clock::now();
  auto bad_char =
      BoyerMoore::bad_character_table(pattern); // Tabla de caracteres malos
  end_construction = high_resolution_clock::now();
  construction_duration =
      duration_cast<microseconds>(end_construction - start_construction);

  start_search = high_resolution_clock::now();
  auto matches_bm = BoyerMoore::search(text, pattern);
  end_search = high_resolution_clock::now();
  search_duration = duration_cast<microseconds>(end_search - start_search);

  // Memoria: tabla de caracteres (256 entradas para ASCII)
  size_t memory_bm = 256 * sizeof(int);
  results.push_back(
      {"Boyer-Moore", text.size(),
       static_cast<double>(construction_duration.count()) / 1000.0,
       static_cast<double>(search_duration.count()) / 1000.0, memory_bm});

  return results;
}

// Imprime los resultados del benchmark en formato tabla
void Benchmark::print_results(const std::vector<BenchmarkResult> &results) {
  std::cout << std::setw(15) << "Algorithm" << std::setw(15) << "Input Size"
            << std::setw(18) << "Construction (ms)" << std::setw(15)
            << "Search (ms)" << std::setw(15) << "Memory (KB)" << "\n";
  std::cout << std::string(78, '-') << "\n";

  for (const auto &result : results) {
    std::cout << std::setw(15) << result.algorithm << std::setw(15)
              << result.input_size << std::setw(18) << std::fixed
              << std::setprecision(3) << result.construction_time_ms
              << std::setw(15) << std::fixed << std::setprecision(3)
              << result.search_time_ms << std::setw(15) << std::fixed
              << std::setprecision(3) << (result.memory_bytes / 1024.0) << "\n";
  }
}

// Exporta los resultados a un archivo CSV para análisis posterior
void Benchmark::export_csv(const std::vector<BenchmarkResult> &results,
                           const std::string &filename) {
  std::ofstream file(filename);
  file << "Algorithm,Input Size,Construction Time (ms),Search Time (ms),Memory "
          "(KB)\n";
  for (const auto &result : results) {
    file << result.algorithm << "," << result.input_size << ","
         << result.construction_time_ms << "," << result.search_time_ms << ","
         << (result.memory_bytes / 1024.0) << "\n";
  }
  file.close();
}
