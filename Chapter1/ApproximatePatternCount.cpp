#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>

int HammingDistance(std::string p, std::string q){
    int hammingDistance = 0;
    for (std::string::iterator it1 = p.begin(), it2 = q.begin(); it1 != p.end() || it2 != q.end(); it1++, it2++) {
        if (*it1 != *it2) {
            hammingDistance++;
        }
    }
    return hammingDistance;
}

int ApproximatePatternCount(std::string Pattern, std::string Text, int d){
    int count = 0;
    for (int i = 0; i < Text.length() - Pattern.length() + 1; i++) {
        std::string substring = Text.substr(i, Pattern.length());
        if (HammingDistance(substring, Pattern) <= d) {
            count++;
        }
    }
    return count;
}

int main() {
    int count = ApproximatePatternCount("GAGG", "TTTAGAGCCTTCAGAGG", 2);
    std::cout << count;
    return 0;
}