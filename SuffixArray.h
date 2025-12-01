//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_SUFFIXARRAY_H
#define UNTITLED_SUFFIXARRAY_H

#pragma once
#include <string>
#include <vector>

// Clase SuffixArray - Estructura de datos para búsqueda eficiente de patrones
// Un Suffix Array es un arreglo de índices que representa todos los sufijos
// de una cadena ordenados lexicográficamente. Permite realizar búsquedas de
// patrones de manera muy eficiente después de un preprocesamiento.
// Complejidades:
// - Construcción: O(n log n) usando el algoritmo de duplicación
// - Búsqueda: O(m log n) donde m es la longitud del patrón
// - Espacio: O(n) para almacenar el arreglo de sufijos
class SuffixArray {
public:
  // Constructor: crea el Suffix Array a partir de una cadena
  // Complejidad: O(n log n)
  explicit SuffixArray(const std::string &s);

  // Obtiene el arreglo de sufijos construido
  const std::vector<int> &sa() const;

  // Obtiene el tamaño de la cadena
  int n() const;

  // Busca un patrón en la cadena usando búsqueda binaria
  // Complejidad: O(m log n) donde m es la longitud del patrón
  bool search(const std::string &pattern) const;

  // Construye el Suffix Array usando el algoritmo de duplicación (doubling)
  // Este es un método estático que puede usarse independientemente
  // Complejidad: O(n log n)
  static std::vector<int> build_sa(const std::string &s);

private:
  std::string s_;       // Cadena original
  std::vector<int> sa_; // Arreglo de sufijos (índices ordenados)
};

#endif // UNTITLED_SUFFIXARRAY_H