#include "Benchmark.h"
#include <iostream>

// Genera un texto aleatorio de n caracteres
// Usa un alfabeto pequeño (4 letras) para aumentar la probabilidad de
// coincidencias
std::string generate_random_text(size_t n) {
  std::string text;
  text.reserve(n);
  for (size_t i = 0; i < n; ++i) {
    text += 'a' + (rand() % 4); // Alfabeto: {a, b, c, d}
  }
  return text;
}

// Programa principal de benchmarking
// Compara el desempeño de Suffix Array vs KMP vs Boyer-Moore
// con diferentes tamaños de texto
int main() {
  // Tamaños de prueba: de 1K a 1M caracteres
  std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
  std::string pattern = "abc"; // Patrón simple a buscar

  for (size_t n : sizes) {
    std::cout << "\n=== Test con n = " << n << " ===\n";
    std::string text = generate_random_text(n);

    // Ejecutar y comparar los tres algoritmos
    auto results = Benchmark::compare_all(text, pattern);
    Benchmark::print_results(results);

    // Exportar resultados a CSV para análisis posterior
    Benchmark::export_csv(results, "results_" + std::to_string(n) + ".csv");
  }

  return 0;
}
