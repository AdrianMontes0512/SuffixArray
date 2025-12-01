#ifndef UNTITLED_BENCHMARK_H
#define UNTITLED_BENCHMARK_H

#pragma once
#include <chrono>
#include <functional>
#include <string>
#include <vector>

// Estructura que almacena los resultados de un benchmark
// Permite comparar el desempeño de diferentes algoritmos de búsqueda
struct BenchmarkResult {
  std::string
      algorithm;     // Nombre del algoritmo (Suffix Array, KMP, Boyer-Moore)
  size_t input_size; // Tamaño del texto analizado
  double construction_time_ms; // Tiempo de preprocesamiento en milisegundos
  double search_time_ms;       // Tiempo de búsqueda en milisegundos
  size_t memory_bytes;         // Memoria utilizada en bytes
};

// Clase Benchmark - Compara el desempeño de algoritmos de búsqueda de patrones
//
// Compara tres algoritmos:
// - Suffix Array: O(n log n) construcción, O(m log n) búsqueda
// - KMP: O(m) construcción, O(n) búsqueda
// - Boyer-Moore: O(m + σ) construcción, O(n) búsqueda (σ = tamaño del alfabeto)
//
// El Suffix Array es especialmente eficiente cuando se realizan múltiples
// búsquedas sobre el mismo texto, ya que el costo de construcción se amortiza.
class Benchmark {
public:
  // Compara todos los algoritmos sobre un texto y patrón dados
  // @param text Texto donde buscar
  // @param pattern Patrón a buscar
  // @return Vector con los resultados de cada algoritmo
  static std::vector<BenchmarkResult> compare_all(const std::string &text,
                                                  const std::string &pattern);

  // Imprime los resultados en formato tabla
  // @param results Vector de resultados a imprimir
  static void print_results(const std::vector<BenchmarkResult> &results);

  // Exporta los resultados a un archivo CSV
  // @param results Vector de resultados
  // @param filename Nombre del archivo de salida
  static void export_csv(const std::vector<BenchmarkResult> &results,
                         const std::string &filename);

private:
  // Función template para medir el tiempo de ejecución de una función
  // @param func Función a medir
  // @return Tiempo de ejecución en milisegundos
  template <typename Func> static double measure_time(Func &&func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
  }
};

#endif // UNTITLED_BENCHMARK_H