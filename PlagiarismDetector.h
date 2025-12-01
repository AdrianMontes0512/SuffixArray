#ifndef PLAGIARISMDETECTOR_H
#define PLAGIARISMDETECTOR_H

#pragma once
#include "SuffixArray.h"
#include <string>
#include <utility>
#include <vector>

// Clase PlagiarismDetector - Detecta similitudes entre textos usando Suffix
// Array
//
// Esta aplicación creativa demuestra el poder del Suffix Array para resolver
// problemas del mundo real. Detecta fragmentos copiados entre documentos
// mediante la búsqueda eficiente de subcadenas comunes.
//
// Funcionamiento:
// 1. Combina ambos textos con un separador único
// 2. Construye el Suffix Array y el LCP Array (Longest Common Prefix)
// 3. Identifica subcadenas comunes de longitud mínima
// 4. Calcula métricas de similitud
class PlagiarismDetector {
public:
  // Representa un fragmento de texto común entre dos documentos
  struct Match {
    int pos1;         // Posición en el primer documento
    int pos2;         // Posición en el segundo documento
    int length;       // Longitud del fragmento común
    std::string text; // Texto del fragmento
  };

  // Resultados del análisis de plagio
  struct Report {
    double similarity_percentage; // Porcentaje de similitud (0-100)
    std::vector<Match> matches;   // Lista de fragmentos comunes
    int total_matched_chars;      // Total de caracteres coincidentes
    int longest_match;            // Longitud del fragmento más largo
  };

  // Constructor
  // text1: Primer documento a comparar
  // text2: Segundo documento a comparar
  // min_match_length: Longitud mínima para considerar un fragmento como plagio
  // (por defecto 10)
  PlagiarismDetector(const std::string &text1, const std::string &text2,
                     int min_match_length = 10);

  // Ejecuta el análisis de plagio y genera un reporte
  // Complejidad: O(n log n) para construcción + O(n) para análisis
  Report analyze();

  // Obtiene el porcentaje de similitud entre los dos textos
  double get_similarity_percentage() const;

private:
  std::string text1_;    // Primer documento
  std::string text2_;    // Segundo documento
  std::string combined_; // Textos combinados con separador
  int separator_pos_;    // Posición del separador en combined_
  int min_match_length_; // Longitud mínima de coincidencia
  SuffixArray *sa_;      // Suffix Array del texto combinado

  // Construye el LCP Array (Longest Common Prefix)
  // El LCP[i] almacena la longitud del prefijo común más largo
  // entre los sufijos en las posiciones sa[i] y sa[i-1]
  // Complejidad: O(n)
  std::vector<int> build_lcp_array();

  // Verifica si dos posiciones pertenecen a documentos diferentes
  bool from_different_texts(int pos1, int pos2) const;

  // Extrae subcadena del texto combinado
  std::string extract_substring(int pos, int len) const;
};

#endif // PLAGIARISMDETECTOR_H
