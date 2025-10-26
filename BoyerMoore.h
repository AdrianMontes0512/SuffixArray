//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_BOYERMOORE_H
#define UNTITLED_BOYERMOORE_H


#pragma once
#include <string>
#include <vector>

class BoyerMoore {
public:
    static std::vector<int> search(const std::string& text, const std::string& pattern);

private:
    static std::vector<int> bad_character_table(const std::string& pattern);
};


#endif //UNTITLED_BOYERMOORE_H