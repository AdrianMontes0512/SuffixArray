#include "PlagiarismDetector.h"
#include <algorithm>
#include <set>

// Constructor: prepara los textos para el análisis
PlagiarismDetector::PlagiarismDetector(const std::string &text1,
                                       const std::string &text2,
                                       int min_match_length)
    : text1_(text1), text2_(text2), min_match_length_(min_match_length) {

  // Combinamos ambos textos con un separador especial que no aparece en ninguno
  // Formato: text1 + '#' + text2
  // El carácter '#' actúa como barrera para que no se encuentren coincidencias
  // falsas
  combined_ = text1_ + "#" + text2_;
  separator_pos_ = static_cast<int>(text1_.size());

  // Construimos el Suffix Array del texto combinado
  sa_ = new SuffixArray(combined_);
}

// Análisis principal de plagio usando Suffix Array y LCP Array
//
// Algoritmo:
// 1. Construir LCP array para encontrar prefijos comunes
// 2. Buscar en el SA pares de sufijos que:
//    - Provengan de documentos diferentes
//    - Tengan LCP >= longitud mínima
// 3. Filtrar coincidencias duplicadas
// 4. Calcular métricas de similitud
PlagiarismDetector::Report PlagiarismDetector::analyze() {
  Report report;
  report.similarity_percentage = 0.0;
  report.total_matched_chars = 0;
  report.longest_match = 0;

  // Construir el LCP array
  std::vector<int> lcp = build_lcp_array();

  // Set para evitar contar el mismo fragmento múltiples veces
  std::set<std::pair<int, int>> seen_matches;

  // Recorremos el LCP array buscando coincidencias
  for (int i = 1; i < static_cast<int>(lcp.size()); ++i) {
    int lcp_val = lcp[i];

    // Solo nos interesan coincidencias suficientemente largas
    if (lcp_val < min_match_length_)
      continue;

    int pos1 = sa_->sa()[i - 1];
    int pos2 = sa_->sa()[i];

    // Verificar que los sufijos vengan de documentos diferentes
    if (!from_different_texts(pos1, pos2))
      continue;

    // Asegurar que pos1 es del primer texto y pos2 del segundo
    if (pos1 > separator_pos_) {
      std::swap(pos1, pos2);
    }

    // Ajustar pos2 para que sea relativo al inicio del segundo texto
    int adjusted_pos2 = pos2 - separator_pos_ - 1;

    // Evitar duplicados
    auto match_key = std::make_pair(pos1, adjusted_pos2);
    if (seen_matches.count(match_key))
      continue;
    seen_matches.insert(match_key);

    // Crear el registro de coincidencia
    Match match;
    match.pos1 = pos1;
    match.pos2 = adjusted_pos2;
    match.length = lcp_val;
    match.text = extract_substring(pos1, lcp_val);

    report.matches.push_back(match);
    report.total_matched_chars += lcp_val;
    report.longest_match = std::max(report.longest_match, lcp_val);
  }

  // Calcular porcentaje de similitud
  // Usamos el texto más corto como base para evitar valores > 100%
  int min_length = std::min(static_cast<int>(text1_.size()),
                            static_cast<int>(text2_.size()));
  if (min_length > 0) {
    report.similarity_percentage =
        (100.0 * report.total_matched_chars) / min_length;
  }

  return report;
}

// Construye el LCP Array usando el algoritmo de Kasai
// El LCP array es fundamental para encontrar subcadenas comunes.
// LCP[i] = longitud del prefijo común más largo entre SA[i] y SA[i-1]
// Ejemplo:
// Texto: "banana"
// SA: [5, 3, 1, 0, 4, 2] → sufijos: ["a", "ana", "anana", "banana", "na",
// "nana"] LCP: [0, 1, 3, 0, 0, 2] → prefijos comunes entre sufijos consecutivos
// Complejidad: O(n) usando el algoritmo de Kasai
std::vector<int> PlagiarismDetector::build_lcp_array() {
  int n = sa_->n();
  std::vector<int> lcp(n, 0);
  std::vector<int> rank(n);

  // Construir el array de ranking (inverso del SA)
  // rank[i] = posición del sufijo i en el SA
  for (int i = 0; i < n; ++i) {
    rank[sa_->sa()[i]] = i;
  }

  // Algoritmo de Kasai para construir LCP en tiempo lineal
  int h = 0; // Longitud del LCP actual
  for (int i = 0; i < n; ++i) {
    if (rank[i] > 0) {
      int j = sa_->sa()[rank[i] - 1]; // Sufijo anterior en orden lexicográfico

      // Calcular LCP entre sufijos i y j
      while (i + h < n && j + h < n && combined_[i + h] == combined_[j + h]) {
        h++;
      }

      lcp[rank[i]] = h;

      // Optimización: el siguiente LCP será al menos h-1
      if (h > 0)
        h--;
    }
  }

  return lcp;
}

// Verifica si dos posiciones están en documentos diferentes
bool PlagiarismDetector::from_different_texts(int pos1, int pos2) const {
  // Una posición está antes del separador y otra después
  return (pos1 <= separator_pos_ && pos2 > separator_pos_) ||
         (pos2 <= separator_pos_ && pos1 > separator_pos_);
}

// Extrae una subcadena del texto combinado
std::string PlagiarismDetector::extract_substring(int pos, int len) const {
  // Asegurar que no extraemos el separador
  if (pos >= static_cast<int>(combined_.size()))
    return "";

  int actual_len = std::min(len, static_cast<int>(combined_.size()) - pos);
  std::string result = combined_.substr(pos, actual_len);

  // Eliminar el separador si está presente
  size_t sep_pos = result.find('#');
  if (sep_pos != std::string::npos) {
    result = result.substr(0, sep_pos);
  }

  return result;
}

// Obtiene el porcentaje de similitud
double PlagiarismDetector::get_similarity_percentage() const {
  // Nota: Este método no debería ser const porque analyze() modifica estado
  // interno Sin embargo, para mantener la interfaz const, creamos un objeto
  // temporal
  PlagiarismDetector temp(text1_, text2_, min_match_length_);
  return temp.analyze().similarity_percentage;
}
