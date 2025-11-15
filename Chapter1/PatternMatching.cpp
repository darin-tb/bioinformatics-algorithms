#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <fstream>

std::vector<int> PatternMatching(std::string Pattern, std::string Genome){
    std::vector<int> indices;
    for(int i = 0; i < Genome.length() - Pattern.length() + 1; i++) {
        std::string substring = Genome.substr(i, Pattern.length());
        // std::cout << substring << "\n";
        if (substring == Pattern) {
            indices.push_back(i);
        }
    }
    return indices;
}

int main() {
    std::string genome;
    std::ifstream vcholerae("Vibrio_cholerae.txt");
    vcholerae >> genome;
    std::vector<int> v = PatternMatching("CTTGATCAT", genome);
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        std::cout << *it << " ";
    }
    vcholerae.close();
    return 0;
}