//
// Created by mva on 10/25/25.
//

#include "KMP.h"

std::vector<int> KMP::compute_lps(const std::string& pattern) {
    int m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0;

    for (int i = 1; i < m; ) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}

std::vector<int> KMP::search(const std::string& text, const std::string& pattern) {
    std::vector<int> result;
    std::vector<int> lps = compute_lps(pattern);
    int n = text.size(), m = pattern.size();

    for (int i = 0, j = 0; i < n; ) {
        if (text[i] == pattern[j]) {
            ++i; ++j;
        }
        if (j == m) {
            result.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            j ? j = lps[j - 1] : ++i;
        }
    }
    return result;
}
