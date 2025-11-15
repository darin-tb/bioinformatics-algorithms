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

int main() {
    int hammingDistance = HammingDistance("GATAGCAGCTTCTGAACTGGTTACCTGCCGTGAGTAAATTAAAATTTTATTGACTTAGGTCACTAAATACT", "AATAGCAGCTTCTCAACTGGTTACCTCGTATGAGTAAATTAGGTCATTATTGACTCAGGTCACTAACGTCT");
    std::cout << hammingDistance; 
    return 0;
}