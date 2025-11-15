#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
// Please do not remove package declarations because these are used by the autograder.
// Please do not remove package declarations because these are used by the autograder.
std::map <std::string, int> FrequencyTable(std::string Text, int k) {
    std::map <std::string, int> freqMap;
    int n = Text.length();
    for (int i = 0; i < n - k + 1; i++) {
        std::string pattern = Text.substr(i, k);
        if (freqMap.find(pattern) == freqMap.end()) {
            freqMap[pattern] = 1;
        } else {
            freqMap[pattern] += 1;
        }
    }
    // std::map <std::string, int>::iterator map_it = freqMap.begin();
    // for (; map_it != freqMap.end(); map_it++) {
    //     std::cout << map_it->first << " " << map_it->second << "\n";
    // }
    return freqMap;
}

bool InVector(std::vector<std::string> kmers, std::string pattern) {
    for (std::vector<std::string>::iterator it = kmers.begin(); it != kmers.end(); it++) {
        if (pattern == *it) {
            return true;
        }
    }
    return false;
}

// Insert your FindClumps function here, along with any subroutines you need
std::vector <std::string> FindClumps(std::string Genome, int k, int L, int t){
    std::vector<std::string> L_t_kmers;
    for (int i = 0; i < Genome.length() - L + 1; i++) {
        std::string window = Genome.substr();
        std::map<std::string, int> freqMap = FrequencyTable(window, k);
        for (std::map<std::string, int>::iterator it = freqMap.begin(); it != freqMap.end(); it++) {
            if (it->second >= t) {
                L_t_kmers.push_back(it->first);
            }
        }
    }
    std::vector <std::string> unique_kmers;
    for (std::vector<std::string>::iterator it = L_t_kmers.begin(); it != L_t_kmers.end(); it++) {
        if (!InVector(unique_kmers, *it)) {
            unique_kmers.push_back(*it);
        }
    }
    return unique_kmers;
}

int main() {
    std::vector<std::string> clumps =  FindClumps("CGGACTCGACAGATGTGAAGAACGACAATGTGAAGACTCGACACGACAGAGTGAAGAGAAGAGGAAACATTGTAA", 5, 50, 4);
    for (std::vector<std::string>::iterator it = clumps.begin(); it != clumps.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}