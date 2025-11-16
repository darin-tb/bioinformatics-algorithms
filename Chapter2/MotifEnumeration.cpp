#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

std::vector<std::vector<std::string>> BuildKmerMap(std::vector<std::string> Dna, int k) {
    std::vector<std::vector<std::string>> KmerMap;
    for (int i = 0; i < Dna.size(); i++) {
        std::vector<std::string> Kmers;
        std::string pattern = Dna.at(i);
        for (int j = 0; j < pattern.length() - k + 1; j++) {
            std::string kmer = pattern.substr(j, k);
            Kmers.push_back(kmer);
        }
        KmerMap.push_back(Kmers);
    }
    return KmerMap;
}

bool InVector(std::vector<std::string> vec, std::string s) {
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++) {
        if (*it == s) {
            return true;
        }
    }
    return false;
}

int CountMismatches(std::string s1, std::string s2) {
    int mismatches = 0;
    for (int i = 0; i < s1.length(); i++) {
        if (s1[i] != s2[i]) {
            mismatches++;
        }
    }
    return mismatches;
}

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
        return std::vector<std::string>{Pattern};
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


// Insert your MotifEnumeration function here, along with any subroutines you need
std::vector<std::string> MotifEnumeration(std::vector<std::string> Dna, int k, int d){
    std::vector<std::string> Patterns;
    std::vector<std::vector<std::string>> KmerMap = BuildKmerMap(Dna, k);
    for (int i = 0; i < KmerMap.size(); i++) {
        for(int j = 0; j < KmerMap[i].size(); j++) {
            std::string kmer = KmerMap.at(i).at(j);
            std::vector<std::string> neighbors = Neighbors(kmer, d);
            for (std::vector<std::string>::iterator neighbors_it = neighbors.begin(); neighbors_it != neighbors.end(); neighbors_it++) {
                std::string neighbor = *neighbors_it;
                bool all_found = true;
                for (int l = 0; l < KmerMap.size(); l++) {
                    if (i == l) {
                        continue;
                    }
                    bool found = false;
                    for (int m = 0; m < KmerMap[l].size(); m++) {
                        std::string query_kmer = KmerMap.at(l).at(m);
                        int mismatches = CountMismatches(neighbor, query_kmer);
                        if (mismatches <= d) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        all_found = false;
                        break;
                    }
                }
                if (all_found && !InVector(Patterns, neighbor)) {
                    Patterns.push_back(neighbor);
                }
            }
        }

    }
    return Patterns;
}
int main() {
    std::vector<std::string> Dna;
    Dna.push_back("ATTTGGC");
    Dna.push_back("TGCCTTA");
    Dna.push_back("CGGTATC");
    Dna.push_back("GAAAATT");
    int k = 3;
    int d = 1;

    std::vector<std::string> Patterns = MotifEnumeration(Dna, k, d);
    for (auto it = Patterns.begin(); it != Patterns.end(); it++) {
        std::cout << *it << " ";
    }
}