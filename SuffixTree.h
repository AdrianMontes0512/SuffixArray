//
// Created by mva on 10/25/25.
//

#ifndef UNTITLED_SUFFIXTREE_H
#define UNTITLED_SUFFIXTREE_H


#pragma once
#include <string>
#include <map>
#include <memory>

class SuffixTree {
public:
    explicit SuffixTree(const std::string& s);
    bool search(const std::string& pattern) const;

private:
    struct Node {
        std::map<char, std::unique_ptr<Node>> children;
        int start;
        int* end;
        Node* suffix_link;
    };

    std::string text_;
    std::unique_ptr<Node> root_;
    void build();
};



#endif //UNTITLED_SUFFIXTREE_H