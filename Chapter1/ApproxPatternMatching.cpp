#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <fstream>


int HammingDistance(std::string p, std::string q){
    int hammingDistance = 0;
    for (std::string::iterator it1 = p.begin(), it2 = q.begin(); it1 != p.end() || it2 != q.end(); it1++, it2++) {
        if (*it1 != *it2) {
            hammingDistance++;
        }
    }
    return hammingDistance;
}
std::vector<int> ApproximatePatternMatching(std::string Pattern, std::string Text, int d){
    std::vector<int> indices;
    for(int i = 0; i < Text.length() - Pattern.length() + 1; i++) {
        std::string substring = Text.substr(i, Pattern.length());
        if (HammingDistance(substring, Pattern) <= d) {
            indices.push_back(i);
        }
    }
    return indices;
}

int main() {
    std::string genome;
    std::ifstream vcholerae("Vibrio_cholerae.txt");
    vcholerae >> genome;
    int d = 3;
    std::vector<int> v = ApproximatePatternMatching("ATTCTGGA", "CGCCCGAATCCAGAACGCATTCCCATATTTCGGGACCACTGGCCTCCACGGTACGGACGTCAATCAAAT", d);
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    vcholerae.close();
    return 0;
}