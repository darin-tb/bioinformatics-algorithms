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


int DistanceBetweenPatternAndStrings(std::string Pattern, std::vector<std::string> Dna) {
    int k = Pattern.length();
    int distance = 0;
    for (int i = 0; i < Dna.size(); i++) {
        int hamming_distance = INT_MAX;
        for (int j = 0; j < Dna.at(i).size() - k + 1; j++) {
            std::string substring = Dna.at(i).substr(j, k);
            if (HammingDistance(Pattern, substring) < hamming_distance) {
                hamming_distance = HammingDistance(Pattern, substring);
            }
        }
        distance += hamming_distance;
    }
    return distance;
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

std::string MedianString(std::vector<std::string> Dna, int k){
    int overall_distance = INT_MAX;
    std::string median_string = "";

    std::string template_string = "";
    for (int i = 0; i < k; i++) {
        template_string += "A";
    }

    std::vector<std::string> all_strings = Neighbors(template_string, k);

    for (int i = 0; i < all_strings.size(); i++) {
        std::string pattern = all_strings.at(i);
        int distance = DistanceBetweenPatternAndStrings(pattern, Dna);
        if (distance <= overall_distance) {
            overall_distance = distance;
            median_string = pattern;
        }
    }
    return median_string;
}

// int main() {
//     std::vector<std::string> Dna;
//     Dna.push_back("AAATTGACGCAT");
//     Dna.push_back("GACGACCACGTT");
//     Dna.push_back("CGTCAGCGCCTG");
//     Dna.push_back("GCTGAGCACCGG");   
//     Dna.push_back("AGTACGGGACAG");
//     std::cout << MedianString(Dna, 3);
//     // std::vector<std::string> neighbors = Neighbors("AAA", 3);
//     // for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
//     //     std::cout << *it << " ";
//     // }
//     return 0;
// }