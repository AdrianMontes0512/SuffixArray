cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
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
