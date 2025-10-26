//
// Created by mva on 10/25/25.
//

#include "BoyerMoore.h"
#include <algorithm>

std::vector<int> BoyerMoore::bad_character_table(const std::string& pattern) {
    std::vector<int> table(256, -1);
    for (int i = 0; i < static_cast<int>(pattern.size()); ++i) {
        table[static_cast<unsigned char>(pattern[i])] = i;
    }
    return table;
}

std::vector<int> BoyerMoore::search(const std::string& text, const std::string& pattern) {
    std::vector<int> result;
    int n = text.size();
    int m = pattern.size();

    if (m == 0 || m > n) return result;

    std::vector<int> bad_char = bad_character_table(pattern);

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j]) {
            --j;
        }

        if (j < 0) {
            result.push_back(s);
            s += (s + m < n) ? m - bad_char[static_cast<unsigned char>(text[s + m])] : 1;
        } else {
            s += std::max(1, j - bad_char[static_cast<unsigned char>(text[s + j])]);
        }
    }

    return result;
}
