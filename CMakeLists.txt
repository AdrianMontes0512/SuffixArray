cmake_minimum_required(VERSION 4.0)
project(SuffixArray)

set(CMAKE_CXX_STANDARD 20)

# Ejecutable principal de benchmarks
add_executable(benchmark_main main.cpp
        SuffixArray.cpp
        SuffixArray.h
        SuffixTree.cpp
        SuffixTree.h
        KMP.cpp
        KMP.h
        BoyerMoore.cpp
        BoyerMoore.h
        Benchmark.cpp
        Benchmark.h)

# Ejecutable para demostracion de deteccion de plagio
add_executable(demo_plagiarism demo_plagiarism.cpp
        PlagiarismDetector.cpp
        PlagiarismDetector.h
        SuffixArray.cpp
        SuffixArray.h)

# Demo Interactivo TUI (Terminal User Interface)
add_executable(demo_tui demo_tui.cpp
        InteractiveMenu.cpp
        InteractiveMenu.h
        PlagiarismDetector.cpp
        PlagiarismDetector.h
        SuffixArray.cpp
        SuffixArray.h)
