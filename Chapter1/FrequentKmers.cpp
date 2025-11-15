#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>

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

int MaxMap(std::map<std::string, int> freqMap) {
    int max = 0;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        if (it->second > max) {
            max = it->second;
        }
    }
    return max;
}

// Insert your FrequentWords function here, along with any subroutines you need
std::vector <std::string> FrequentWords(std::string Text, int k){
    std::map<std::string, int> freqMap = FrequencyTable(Text, k);
    int max = MaxMap(freqMap);
    std::vector <std::string> v;

    for (auto it = freqMap.begin(); it != freqMap.end(); it++){
        if (it->second == max) {
            v.push_back(it->first);
        }
    }

    return v;
}

int main() {
    std::vector<std::string> v = FrequentWords("ACGTTGCATGTCGCATGATGCATGAGAGCT", 4);
    for (auto it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    return 0;
}