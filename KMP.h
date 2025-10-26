//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_KMP_H
#define UNTITLED_KMP_H


#pragma once
#include <string>
#include <vector>

class KMP {
public:
    static std::vector<int> search(const std::string& text, const std::string& pattern);
    static std::vector<int> compute_lps(const std::string& pattern);
};



#endif //UNTITLED_KMP_H