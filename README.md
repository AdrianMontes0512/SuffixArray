# Suffix Array - Proyecto de Algoritmos y Estructuras de Datos

## Descripción

Este proyecto implementa la estructura de datos Suffix Array y demuestra su aplicación práctica en la detección de plagio entre documentos de texto.

El Suffix Array es un arreglo de índices que representa todos los sufijos de una cadena ordenados lexicográficamente. Esta estructura permite realizar búsquedas de patrones de manera eficiente.

## Problema que Resuelve

El Suffix Array resuelve problemas de búsqueda de patrones en textos de forma eficiente:

- Búsqueda de múltiples patrones en el mismo texto
- Detección de subcadenas comunes entre documentos
- Identificación de fragmentos duplicados o plagiados
- Análisis de similitud textual

## Complejidades

| Operación | Complejidad | Descripción |
|-----------|-------------|-------------|
| Construcción | O(n log n) | Usando algoritmo de duplicación |
| Búsqueda | O(m log n) | m = longitud del patrón, n = longitud del texto |
| Espacio | O(n) | Arreglo de enteros del tamaño del texto |

## Ventajas vs Otros Algoritmos

- Comparado con KMP: Mejor para múltiples búsquedas (el costo de construcción se amortiza)
- Comparado con Boyer-Moore: Búsquedas más rápidas una vez construido
- Comparado con Suffix Tree: Usa menos memoria, O(n) vs O(n log n)

## Contenido del Repositorio

```
SuffixArray/
├── SuffixArray.h/cpp           - Implementación del Suffix Array
├── PlagiarismDetector.h/cpp    - Aplicación creativa: detector de plagio
├── demo_plagiarism.cpp         - Programa de demostración interactivo
├── Benchmark.h/cpp             - Comparación con KMP y Boyer-Moore
├── main.cpp                    - Programa de benchmarking
├── KMP.h/cpp                   - Algoritmo KMP para comparación
├── BoyerMoore.h/cpp            - Algoritmo Boyer-Moore para comparación
└── CMakeLists.txt              - Configuración de compilación
```

## Instalación y Compilación

### Requisitos

- C++20 o superior
- CMake 3.10 o superior
- Compilador compatible (GCC, Clang)

### Clonar el Repositorio

```bash
git clone https://github.com/TU_USUARIO/SuffixArray.git
cd SuffixArray
```

### Compilar con CMake

```bash
mkdir build
cd build
cmake ..
make
```

Esto generará dos ejecutables:
- `benchmark_main` - Programa de benchmarking
- `demo_plagiarism` - Demostración del detector de plagio

### Compilar Directamente con g++

Si no tienes CMake instalado:

```bash
# Compilar el programa de benchmarks
g++ -std=c++20 -o benchmark_main main.cpp SuffixArray.cpp Benchmark.cpp KMP.cpp BoyerMoore.cpp

# Compilar la demo de plagio
g++ -std=c++20 -o demo_plagiarism demo_plagiarism.cpp PlagiarismDetector.cpp SuffixArray.cpp
```

## Ejecución

### Demo de Detección de Plagio (Aplicación Creativa)

```bash
./build/demo_plagiarism
```

Este programa muestra:
- 5 casos de prueba con diferentes niveles de plagio
- Fragmentos comunes encontrados entre textos
- Porcentaje de similitud calculado
- Modo interactivo para probar con textos propios

### Programa de Benchmarking

```bash
./build/benchmark_main
```

Compara el desempeño de Suffix Array, KMP y Boyer-Moore con textos de diferentes tamaños (1K, 10K, 100K, 1M caracteres).

## Aplicación Creativa: Detector de Plagio

La aplicación desarrollada detecta fragmentos copiados entre dos documentos usando Suffix Array.

### Funcionamiento

1. Combina ambos textos con un separador único
2. Construye el Suffix Array del texto combinado
3. Calcula el LCP Array (Longest Common Prefix) usando el algoritmo de Kasai
4. Identifica fragmentos compartidos entre los documentos
5. Calcula métricas de similitud

### Ventajas de Usar Suffix Array

- Eficiencia: O(n log n) comparado con O(n²) de métodos básicos
- Precisión: Encuentra todas las coincidencias, incluso pequeñas
- Flexible: Longitud mínima de coincidencia configurable
- Escalable: Funciona eficientemente con documentos grandes

### Ejemplo de Uso

```cpp
#include "PlagiarismDetector.h"

string text1 = "El Suffix Array es una estructura muy potente...";
string text2 = "El Suffix Array es una estructura muy eficiente...";

// Crear detector con longitud mínima de 10 caracteres
PlagiarismDetector detector(text1, text2, 10);

// Analizar
auto report = detector.analyze();

cout << "Similitud: " << report.similarity_percentage << "%" << endl;
cout << "Fragmentos encontrados: " << report.matches.size() << endl;
```

## Algoritmo de Construcción

El Suffix Array se construye usando el algoritmo de duplicación (prefix doubling):

1. Ordenar sufijos por el primer carácter
2. En cada iteración k, ordenar por prefijos de longitud 2^k
3. Usar counting sort O(n) en lugar de comparison sort para eficiencia
4. Continuar hasta que todos los sufijos estén ordenados

Optimizaciones implementadas:
- Counting sort para cada iteración
- Terminación temprana cuando todos los sufijos son distinguibles
- Reutilización de rankings previos

## Casos de Prueba

El programa de demostración incluye varios casos:

1. Plagio parcial obvio - Fragmentos directamente copiados
2. Textos académicos similares - Frases compartidas con diferente redacción
3. Detección de código duplicado - Fragmentos de código idénticos
4. Textos completamente diferentes - Verificación de ausencia de falsos positivos
5. Frases repetitivas - Patrones comunes en ambos textos

## Resultados de Benchmark

Ejemplo de salida del programa de benchmarking:

```
=== Test con n = 100000 ===
      Algorithm     Input Size Construction (ms)    Search (ms)    Memory (KB)
------------------------------------------------------------------------------
   Suffix Array         100000           128.646          0.002        390.625
            KMP         100000             0.001          6.299          0.012
    Boyer-Moore         100000             0.002          4.381          1.000
```

Interpretación:
- Para una sola búsqueda: KMP y Boyer-Moore son más rápidos
- Para múltiples búsquedas: Suffix Array amortiza su costo de construcción
- Suffix Array usa más memoria pero ofrece búsquedas muy rápidas

## Aplicaciones Prácticas

1. Detección de plagio académico
2. Búsqueda de secuencias en bioinformática
3. Compresión de datos (BWT)
4. Motores de búsqueda y autocompletado

## Estructura del Código

Todo el código está comentado en español con explicaciones del algoritmo paso a paso. Los comentarios incluyen:

- Explicación del funcionamiento de cada función
- Complejidad temporal de las operaciones
- Ejemplos ilustrativos donde es útil

## Autores

Proyecto desarrollado para el curso de Algoritmos y Estructuras de Datos.

## Referencias

- Manber & Myers (1993) - "Suffix Arrays: A New Method for On-Line String Searches"
- Algoritmo de Kasai para construcción del LCP Array