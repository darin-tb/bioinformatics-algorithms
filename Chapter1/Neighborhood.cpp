#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>


std::string Suffix(std::string Pattern) {
    return Pattern.substr(1, Pattern.length());
}

int HammingDistance(std::string p, std::string q){
    int hammingDistance = 0;
    for (std::string::iterator it1 = p.begin(), it2 = q.begin(); it1 != p.end() || it2 != q.end(); it1++, it2++) {
        if (*it1 != *it2) {
            hammingDistance++;
        }
    }
    return hammingDistance;
}

std::vector<std::string> Neighbors(std::string Pattern, int d){

    std::vector<std::string> nucleotides = {"A","C","G","T"};
    if (d == 0) {
        std::vector<std::string> pattern;
        pattern.push_back(Pattern);
        return pattern;
    }
    if (Pattern.length() == 1) {
        return nucleotides;
    }
    std::vector<std::string> Neighborhood;
    std::vector<std::string> SuffixNeighbors = Neighbors(Suffix(Pattern), d);
    for (std::vector<std::string>::iterator it1 = SuffixNeighbors.begin(); it1 != SuffixNeighbors.end(); it1++) {
        if (HammingDistance(Suffix(Pattern), *it1) < d) {
            for (std::vector<std::string>::iterator it2 = nucleotides.begin(); it2 != nucleotides.end(); it2++) {
                Neighborhood.push_back(*it2 + *it1);
            }
         } else {
            Neighborhood.push_back(Pattern[0] + *it1);
         }
    }
    return Neighborhood;
}

int main() {
    std::vector<std::string> neighborhood = Neighbors("ACG", 1);
    for (auto it = neighborhood.begin(); it != neighborhood.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}