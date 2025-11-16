#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

// Please do not remove package declarations because these are used by the autograder.

using k_mer_edgelist_t = std::unordered_map<std::string, std::vector<std::string>>;

//build deBruijn graph from kmers
// for each kmer, kmer[0,k-1] maps to kmer[1,k]

// Insert your DeBruijnKmers function here, along with any subroutines you need
k_mer_edgelist_t DeBruijnKmers(std::vector<std::string> k_mers){
    k_mer_edgelist_t edgelist;
    for (int i = 0; i < k_mers.size(); i++) {
        edgelist[k_mers[i].substr(0,k_mers[i].size()-1)].push_back(k_mers[i].substr(1,k_mers[i].size()-1));
    }
    return edgelist;
}

int main() {
    std::vector<std::string> k_mers;
          
    k_mers.push_back("GAGG");
    k_mers.push_back("CAGG");
    k_mers.push_back("GGGG");
    k_mers.push_back("GGGA");
    k_mers.push_back("CAGG");
    k_mers.push_back("AGGG");
    k_mers.push_back("GGAG");

    k_mer_edgelist_t edgelist = DeBruijnKmers(k_mers);
    for (auto it = edgelist.begin(); it != edgelist.end(); it++) {
        std::cout << it->first << ": ";
        for (auto val : it->second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}