//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_SUFFIXARRAY_H
#define UNTITLED_SUFFIXARRAY_H


#pragma once
#include <string>
#include <vector>

class SuffixArray {
public:
    explicit SuffixArray(const std::string& s);

    const std::vector<int>& sa() const;
    int n() const;
    bool search(const std::string& pattern) const;

    static std::vector<int> build_sa(const std::string& s);

private:
    std::string s_;
    std::vector<int> sa_;
};




#endif //UNTITLED_SUFFIXARRAY_H