#include "PlagiarismDetector.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// Programa de demostración del Detector de Plagio
//
// Este programa muestra cómo usar la clase PlagiarismDetector para
// encontrar fragmentos comunes entre dos textos de manera eficiente.

// Función auxiliar para imprimir una línea divisoria
void print_separator(char c = '=', int length = 80) {
  cout << string(length, c) << endl;
}

// Función para mostrar un fragmento de texto con contexto
void show_text_preview(const string &text, int max_length = 60) {
  if (text.length() <= max_length) {
    cout << "\"" << text << "\"";
  } else {
    cout << "\"" << text.substr(0, max_length) << "...\"";
  }
}

// Función principal de demostración
void demo_plagiarism_detection(const string &text1, const string &text2,
                               const string &title, int min_length = 10) {
  print_separator();
  cout << "DEMO: " << title << endl;
  print_separator();

  cout << "\nTexto 1 (longitud: " << text1.size() << " chars):" << endl;
  show_text_preview(text1, 100);
  cout << endl;

  cout << "\nTexto 2 (longitud: " << text2.size() << " chars):" << endl;
  show_text_preview(text2, 100);
  cout << endl;

  // Crear el detector y analizar
  cout << "\nAnalizando con longitud mínima de coincidencia: " << min_length
       << " caracteres..." << endl;
  PlagiarismDetector detector(text1, text2, min_length);
  PlagiarismDetector::Report report = detector.analyze();

  // Mostrar resultados
  print_separator('-');
  cout << "RESULTADOS DEL ANÁLISIS" << endl;
  print_separator('-');

  cout << fixed << setprecision(2);
  cout << "Porcentaje de similitud: " << report.similarity_percentage << "%"
       << endl;
  cout << "Total de caracteres coincidentes: " << report.total_matched_chars
       << endl;
  cout << "Fragmento más largo encontrado: " << report.longest_match
       << " caracteres" << endl;
  cout << "Número de fragmentos comunes: " << report.matches.size() << endl;

  if (!report.matches.empty()) {
    cout << "\nFragmentos comunes encontrados:" << endl;
    print_separator('-', 80);

    // Mostrar hasta los primeros 5 fragmentos más largos
    auto matches = report.matches;
    sort(
        matches.begin(), matches.end(),
        [](const PlagiarismDetector::Match &a,
           const PlagiarismDetector::Match &b) { return a.length > b.length; });

    int count = min(5, static_cast<int>(matches.size()));
    for (int i = 0; i < count; ++i) {
      const auto &match = matches[i];
      cout << "\n[" << (i + 1) << "] Longitud: " << match.length
           << " caracteres" << endl;
      cout << "    Posición en texto 1: " << match.pos1 << endl;
      cout << "    Posición en texto 2: " << match.pos2 << endl;
      cout << "    Fragmento: ";
      show_text_preview(match.text, 70);
      cout << endl;
    }

    if (matches.size() > 5) {
      cout << "\n... y " << (matches.size() - 5) << " fragmentos más." << endl;
    }
  } else {
    cout << "\nNo se encontraron fragmentos comunes con la longitud mínima "
            "especificada."
         << endl;
  }

  cout << endl;
}

int main() {
  cout << endl;
  print_separator('*');
  cout << "*" << string(78, ' ') << "*" << endl;
  cout << "*" << string(20, ' ') << "DETECTOR DE PLAGIO CON SUFFIX ARRAY"
       << string(23, ' ') << "*" << endl;
  cout << "*" << string(78, ' ') << "*" << endl;
  print_separator('*');
  cout << endl;

  // DEMO 1: Plagio obvio - texto completamente copiado
  {
    string original = "El Suffix Array es una estructura de datos muy potente "
                      "para búsquedas de patrones. "
                      "Permite encontrar todas las ocurrencias de un patrón en "
                      "tiempo logarítmico.";
    string plagiado = "El Suffix Array es una estructura de datos muy potente "
                      "para búsquedas de patrones. "
                      "Es una herramienta fundamental en bioinformática.";

    demo_plagiarism_detection(original, plagiado, "Plagio Parcial Obvio", 15);
  }

  // DEMO 2: Textos académicos similares
  {
    string texto1 =
        "La complejidad temporal del algoritmo de construcción del Suffix "
        "Array "
        "utilizando el método de duplicación es O(n log n), donde n representa "
        "la longitud de la cadena de entrada. Este algoritmo es eficiente y "
        "ampliamente utilizado en aplicaciones de procesamiento de texto.";

    string texto2 =
        "El método de duplicación para construir el Suffix Array tiene una "
        "complejidad temporal de O(n log n), donde n representa la longitud "
        "de la cadena de entrada. Se utiliza ampliamente en bioinformática "
        "para el análisis de secuencias de ADN.";

    demo_plagiarism_detection(texto1, texto2,
                              "Textos Académicos con Similitudes", 12);
  }

  // DEMO 3: Código similar (detección de plagio en programación)
  {
    string code1 = "for (int i = 0; i < n; i++) { "
                   "sum += array[i]; "
                   "if (sum > max_sum) { max_sum = sum; } }";

    string code2 = "int total = 0; "
                   "for (int i = 0; i < n; i++) { "
                   "sum += array[i]; "
                   "if (sum > max_sum) { max_sum = sum; } } "
                   "return max_sum;";

    demo_plagiarism_detection(code1, code2, "Detección de Código Copiado", 8);
  }

  // DEMO 4: Textos completamente diferentes
  {
    string texto1 =
        "El gato persiguió al ratón por toda la casa durante horas.";
    string texto2 = "Los algoritmos de ordenamiento son fundamentales en "
                    "ciencias de la computación.";

    demo_plagiarism_detection(texto1, texto2, "Textos Completamente Diferentes",
                              8);
  }

  // DEMO 5: Análisis de fragmentos repetidos en el mismo texto
  {
    string texto1 = "Los suffix arrays son útiles. Los suffix arrays permiten "
                    "búsquedas eficientes. "
                    "El uso de suffix arrays es común en bioinformática.";
    string texto2 = "Los suffix arrays son estructuras de datos. Los suffix "
                    "arrays se utilizan "
                    "en muchas aplicaciones.";

    demo_plagiarism_detection(texto1, texto2, "Frases Repetitivas", 10);
  }

  // Ejemplo interactivo (opcional)
  cout << endl;
  print_separator('*');
  cout << "¿Desea probar con sus propios textos? (s/n): ";
  char respuesta;
  cin >> respuesta;
  cin.ignore(); // Limpiar el buffer

  if (respuesta == 's' || respuesta == 'S') {
    string custom_text1, custom_text2;
    int min_len;

    cout << "\nIngrese el primer texto (una línea): " << endl;
    getline(cin, custom_text1);

    cout << "\nIngrese el segundo texto (una línea): " << endl;
    getline(cin, custom_text2);

    cout << "\nLongitud mínima de coincidencia (recomendado 8-15): ";
    cin >> min_len;

    demo_plagiarism_detection(custom_text1, custom_text2,
                              "Análisis Personalizado", min_len);
  }

  print_separator('*');
  cout << "\nGracias por usar el Detector de Plagio!" << endl;
  cout << "Esta aplicación demuestra el poder del Suffix Array para resolver"
       << endl;
  cout << "problemas reales de búsqueda de patrones y análisis de texto."
       << endl;
  print_separator('*');
  cout << endl;

  return 0;
}
