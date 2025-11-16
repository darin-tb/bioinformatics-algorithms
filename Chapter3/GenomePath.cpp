#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;
// Please do not remove package declarations because these are used by the autograder.

// Insert your GenomePath function here, along with any subroutines you need
string GenomePath(vector<string> path){
    string genome = path[0];
    for (int i = 1; i < path.size(); i++) {
        genome = genome.substr(0, i) + path[i];
    }
    return genome;
}

int main() {
    vector<string> path;
    path.push_back("ACCGA");
    path.push_back("CCGAA");
    path.push_back("CGAAG");
    path.push_back("GAAGC");
    path.push_back("AAGCT"); 
    string genome = GenomePath(path);
    cout << genome << endl;
}