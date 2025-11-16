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

// Insert your KmerComposition function here, along with any subroutines you need
std::vector<std::string> KmerComposition(std::string Text, int k){  
    std::vector<std::string> kmers; 
    for (int i = 0; i < Text.length() - k + 1; i++) {
        kmers.push_back(Text.substr(i, k));
    }
    return kmers;
}

int main() {
    std::vector<std::string> kmers =  KmerComposition("CAATCCAAC", 5);
    for (auto it = kmers.begin(); it != kmers.end(); it++) {
        std::cout << *it << " ";
    }
}