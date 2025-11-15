#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>



std::vector<int> MinimumSkew(std::string Genome){
    std::vector<int> skewArray;
    skewArray.push_back(0);
    int skew = 0;
    for (std::string::iterator it = Genome.begin(); it != Genome.end(); it++) {

        if (*it == 'C') {
            skew--;
            skewArray.push_back(skew);
        } else if (*it == 'G') {
            skew++; 
            skewArray.push_back(skew);
        } else {
            skewArray.push_back(skew);
        }
    } 

    std::vector<int> min_indices;
    std::vector<int>::iterator min_it = std::min_element(skewArray.begin(), skewArray.end());
    if (min_it != skewArray.end()) {
        for (std::vector<int>::iterator it = skewArray.begin(); it != skewArray.end(); it++) {
            if (*it == *min_it) {
                min_indices.push_back(std::distance(skewArray.begin(), it));
            }
        }
    }

    return min_indices;
}

int main() {
    std::vector<int> skewArray = MinimumSkew("TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT");
    for (auto it = skewArray.begin(); it != skewArray.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}