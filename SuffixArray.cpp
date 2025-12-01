#include "SuffixArray.h"
#include <algorithm>

// Constructor: inicializa la cadena y construye el Suffix Array
SuffixArray::SuffixArray(const std::string &s) : s_(s) { sa_ = build_sa(s_); }

// Devuelve el arreglo de sufijos
const std::vector<int> &SuffixArray::sa() const { return sa_; }

// Devuelve la longitud de la cadena
int SuffixArray::n() const { return static_cast<int>(s_.size()); }

// Busca un patrón en la cadena usando búsqueda binaria sobre el Suffix Array
// La búsqueda binaria es posible porque los sufijos están ordenados
// lexicográficamente. Para cada posición media, comparamos el patrón con el
// sufijo que comienza en sa_[mid].
// Complejidad: O(m log n) donde m = longitud del patrón, n = longitud del texto
bool SuffixArray::search(const std::string &pattern) const {
  int m = static_cast<int>(pattern.size());
  int left = 0, right = n() - 1;

  // Búsqueda binaria estándar
  while (left <= right) {
    int mid = (left + right) / 2;
    int pos = sa_[mid]; // Posición del sufijo en la cadena original

    // Comparamos el patrón con el sufijo que empieza en pos
    int cmp = 0;
    for (int i = 0; i < m && pos + i < n(); ++i) {
      if (pattern[i] < s_[pos + i]) {
        cmp = -1; // El patrón es menor, buscar a la izquierda
        break;
      } else if (pattern[i] > s_[pos + i]) {
        cmp = 1; // El patrón es mayor, buscar a la derecha
        break;
      }
    }

    // Si llegamos aquí con cmp == 0, encontramos el patrón completo
    if (cmp == 0 && pos + m <= n()) {
      return true;
    }

    // Ajustamos los límites de búsqueda
    if (cmp < 0) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  return false;
}

// Construye el Suffix Array usando el algoritmo de duplicación (Prefix
// Doubling) Este algoritmo ordena los sufijos en O(n log n) comparando prefijos
// cada vez más largos. En cada iteración k, comparamos prefijos de longitud 2^k
// usando los rangos de la iteración anterior. Fases del algoritmo:
// 1. Inicialización: ordenar por el primer carácter
// 2. Duplicación: en cada paso k, ordenar por prefijos de longitud 2^k
// 3. Usar counting sort (O(n)) en lugar de comparison sort para eficiencia
// Complejidad total: O(n log n)
std::vector<int> SuffixArray::build_sa(const std::string &s) {
  int n = static_cast<int>(s.size());

  // Casos base
  if (n == 0)
    return {};
  if (n == 1)
    return {0};

  // Arreglos necesarios para el algoritmo
  std::vector<int> sa(n);       // Suffix Array (resultado final)
  std::vector<int> rank(n);     // Ranking de cada sufijo
  std::vector<int> tmp_sa(n);   // Suffix Array temporal para el ordenamiento
  std::vector<int> tmp_rank(n); // Ranking temporal para actualizar

  // Inicialización: asignar ranking basado en el primer carácter
  for (int i = 0; i < n; ++i) {
    sa[i] = i; // Inicialmente cada sufijo está en su posición original
    rank[i] = static_cast<unsigned char>(s[i]); // Ranking = valor ASCII
  }

  // max_val rastrea el valor máximo de ranking (para optimizar counting sort)
  int max_val = 256; // Inicialmente 256 porque usamos ASCII
  std::vector<int> cnt(std::max(max_val, n)); // Array de conteo para sorting

  // Primera pasada: ordenar por el primer carácter usando counting sort
  for (int i = 0; i < n; ++i)
    cnt[rank[i]]++;
  for (int i = 1; i < max_val; ++i)
    cnt[i] += cnt[i - 1];
  for (int i = n - 1; i >= 0; --i)
    sa[--cnt[rank[i]]] = i;

  // Algoritmo de duplicación: en cada iteración comparamos prefijos de longitud
  // 2k
  for (int k = 1; k < n; k *= 2) {
    // Fase 1: Ordenar por la segunda mitad (sufijo que empieza en pos+k)
    std::fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; ++i) {
      // Si pos+k está fuera de límites, su ranking es 0 (menor que cualquier
      // carácter)
      int idx = (sa[i] + k < n) ? rank[sa[i] + k] : 0;
      cnt[idx]++;
    }
    // Acumular conteos
    for (int i = 1; i < std::max(max_val, n); ++i)
      cnt[i] += cnt[i - 1];
    // Construir el arreglo temporal ordenado por la segunda mitad
    for (int i = n - 1; i >= 0; --i) {
      int idx = (sa[i] + k < n) ? rank[sa[i] + k] : 0;
      tmp_sa[--cnt[idx]] = sa[i];
    }

    // Fase 2: Ordenar por la primera mitad (manteniendo el orden de la segunda)
    std::fill(cnt.begin(), cnt.end(), 0);
    for (int i = 0; i < n; ++i)
      cnt[rank[tmp_sa[i]]]++;
    for (int i = 1; i < std::max(max_val, n); ++i)
      cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i)
      sa[--cnt[rank[tmp_sa[i]]]] = tmp_sa[i];

    // Recalcular rankings basados en el nuevo orden
    tmp_rank[sa[0]] = 0; // El primer sufijo tiene ranking 0
    for (int i = 1; i < n; ++i) {
      int p1 = sa[i - 1], p2 = sa[i]; // Posiciones de dos sufijos consecutivos

      // Verificar si ambos sufijos son iguales en sus primeros 2k caracteres
      bool same = (rank[p1] == rank[p2]); // Primera mitad igual?
      if (p1 + k < n && p2 + k < n) {
        same = same && (rank[p1 + k] == rank[p2 + k]); // Segunda mitad igual?
      } else {
        same = same && (p1 + k >= n && p2 + k >= n); // Ambos terminan?
      }

      // Si son iguales, mantener el mismo ranking; si no, incrementar
      tmp_rank[sa[i]] = tmp_rank[sa[i - 1]] + (same ? 0 : 1);
    }

    // Actualizar rankings para la siguiente iteración
    rank = tmp_rank;
    max_val = rank[sa[n - 1]] + 1;

    // Optimización: si todos los sufijos tienen rankings diferentes, ya
    // terminamos
    if (max_val >= n)
      break;
  }

  return sa;
}
