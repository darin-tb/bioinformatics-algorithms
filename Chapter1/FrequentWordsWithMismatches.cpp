#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <unordered_map>

int MaxMap(std::unordered_map<std::string, int> freqMap) {
    int max = 0;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        if (it->second > max) {
            max = it->second;
        }
    }
    return max;
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

std::vector<std::string> FrequentWordsWithMismatches(std::string Text, int k, int d){
    std::vector<std::string> patterns;
    std::unordered_map<std::string, int> freqMap;
    int n = Text.length();
    for (int i = 0; i < n - k + 1; i++) {
        std::string pattern = Text.substr(i, k);
        std::vector<std::string> neighborhood = Neighbors(pattern, d);
        for (int j = 0; j < neighborhood.size(); j++) {
            std::string neighbor = neighborhood[j];
            if (freqMap.find(neighbor) == freqMap.end()) {
                freqMap[neighbor] = 1;
            } else {
                freqMap[neighbor]++;
            }
        }
        std::cout << freqMap.size() << "\n";
    }
    int m = MaxMap(freqMap);
    for (std::unordered_map<std::string, int>::iterator it = freqMap.begin(); it != freqMap.end(); it++) {
        if (it->second == m) {
            patterns.push_back(it->first);
        }
    }
    return patterns;
}

int main() {
    std::vector<std::string> patterns = FrequentWordsWithMismatches("CACAGTAGGCGCCGGCACACACAGCCCCGGGCCCCGGGCCGCCCCGGGCCGGCGGCCGCCGGCGCCGGCACACCGGCACAGCCGTACCGGCACAGTAGTACCGGCCGGCCGGCACACCGGCACACCGGGTACACACCGGGGCGCACACACAGGCGGGCGCCGGGCCCCGGGCCGTACCGGGCCGCCGGCGGCCCACAGGCGCCGGCACAGTACCGGCACACACAGTAGCCCACACACAGGCGGGCGGTAGCCGGCGCACACACACACAGTAGGCGCACAGCCGCCCACACACACCGGCCGGCCGGCACAGGCGGGCGGGCGCACACACACCGGCACAGTAGTAGGCGGCCGGCGCACAGCC", 10, 2);
    for (std::vector<std::string>::iterator it = patterns.begin(); it != patterns.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}