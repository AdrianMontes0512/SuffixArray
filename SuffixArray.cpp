//
// Created by mva on 10/25/25.
//

#include "SuffixArray.h"
#include <algorithm>

SuffixArray::SuffixArray(const std::string& s) : s_(s) {
    sa_ = build_sa(s_);
}

const std::vector<int>& SuffixArray::sa() const {
    return sa_;
}

int SuffixArray::n() const {
    return static_cast<int>(s_.size());
}

bool SuffixArray::search(const std::string& pattern) const {
    int m = static_cast<int>(pattern.size());
    int left = 0, right = n() - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        int pos = sa_[mid];

        int cmp = 0;
        for (int i = 0; i < m && pos + i < n(); ++i) {
            if (pattern[i] < s_[pos + i]) {
                cmp = -1;
                break;
            } else if (pattern[i] > s_[pos + i]) {
                cmp = 1;
                break;
            }
        }

        if (cmp == 0 && pos + m <= n()) {
            return true;
        }

        if (cmp < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return false;
}

std::vector<int> SuffixArray::build_sa(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return {};
    if (n == 1) return {0};

    std::vector<int> sa(n), rank(n), tmp_sa(n), tmp_rank(n);

    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank[i] = static_cast<unsigned char>(s[i]);
    }

    int max_val = 256;
    std::vector<int> cnt(std::max(max_val, n));

    for (int i = 0; i < n; ++i) cnt[rank[i]]++;
    for (int i = 1; i < max_val; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; --i) sa[--cnt[rank[i]]] = i;

    for (int k = 1; k < n; k *= 2) {
        std::fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; ++i) {
            int idx = (sa[i] + k < n) ? rank[sa[i] + k] : 0;
            cnt[idx]++;
        }
        for (int i = 1; i < std::max(max_val, n); ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) {
            int idx = (sa[i] + k < n) ? rank[sa[i] + k] : 0;
            tmp_sa[--cnt[idx]] = sa[i];
        }

        std::fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; ++i) cnt[rank[tmp_sa[i]]]++;
        for (int i = 1; i < std::max(max_val, n); ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) sa[--cnt[rank[tmp_sa[i]]]] = tmp_sa[i];

        tmp_rank[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            int p1 = sa[i - 1], p2 = sa[i];
            bool same = (rank[p1] == rank[p2]);
            if (p1 + k < n && p2 + k < n) {
                same = same && (rank[p1 + k] == rank[p2 + k]);
            } else {
                same = same && (p1 + k >= n && p2 + k >= n);
            }
            tmp_rank[sa[i]] = tmp_rank[sa[i - 1]] + (same ? 0 : 1);
        }
        rank = tmp_rank;
        max_val = rank[sa[n - 1]] + 1;

        if (max_val >= n) break;
    }

    return sa;
}

