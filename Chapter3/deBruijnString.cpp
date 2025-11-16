#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

// construct the deBruijn graph from a string
// the first k-1 characters in the given string maps to the last k-1 characters in the same string
// Please do not remove package declarations because these are used by the autograder.

using k_mer_edgelist_t = std::unordered_map<std::string, std::vector<std::string>>;

// Insert your DeBruijnString function here, along with any subroutines you need
k_mer_edgelist_t DeBruijnString(std::string Text, int k){
    k_mer_edgelist_t edgelist;

    for (int i = 0; i < Text.length() - k + 1; i++) {
        std::string kmer = Text.substr(i,k);
        edgelist[kmer.substr(0,k-1)].push_back(kmer.substr(1,k-1));
    }
    return edgelist;
}

int main() {

    std::string Text = "TAATGCCATGGGATGTT";
    int k = 3;
    k_mer_edgelist_t edgelist = DeBruijnString(Text, k);
    for (auto it = edgelist.begin(); it != edgelist.end(); it++) {
        std::cout << it->first << ": ";
        for (auto val : it->second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}