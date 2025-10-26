//
// Created by mva on 10/25/25.
//

#include "SuffixTree.h"

SuffixTree::SuffixTree(const std::string& s) : text_(s) {
    build();
}

void SuffixTree::build() {
}

bool SuffixTree::search(const std::string& pattern) const {
    return text_.find(pattern) != std::string::npos;
}
