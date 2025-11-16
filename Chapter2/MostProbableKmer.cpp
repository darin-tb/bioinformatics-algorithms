#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <fstream>
// Please do not remove package declarations because these are used by the autograder.

// Insert your ProfileMostProbableKmer function here, along with any subroutines you need.
// We represent the profile matrix as an array of length 4 in which
// each element is a dynamically resizable vector represnting the row.

struct  {
    std::string Text;
    int k;
    std::array<std::vector<double>, 4> Profile;
} input_params;

std::string ProfileMostProbableKmer(std::string Text, int k, std::array<std::vector<double>, 4> Profile){
    double maximum_score = 0.0;
    std::string best_string = "";
    for (int i = 0; i < Text.length() - k + 1; i++) {
        std::string substring = Text.substr(i, k);
        double score = 1.0;
        for (int j = 0; j < substring.length(); j++) {
            if (substring[j] == 'A') {
                score *= Profile[0].at(j);       
            }
            else if (substring[j] == 'C') {
                score *= Profile[1].at(j);             
            }
            else if (substring[j] == 'G') {
                score *= Profile[2].at(j);             
            }
            else if (substring[j] == 'T') {
                score *= Profile[3].at(j);                
            }
        }
        if (score >= maximum_score) {
            maximum_score = score;
            best_string = substring;
        }
    }
    return best_string;
}

int main() {

    std::array<std::vector<double>, 4> Profile;
    


    std::string line;
    std::ifstream input;
    input.open("ProfileMostProbableKmer/inputs/input_1.txt");
    int index = 0;

    while(getline(input, line)) {
        if (index == 0) {
            std::cout << line << std::endl;
            input_params.Text = line;
        }
        else if (index == 1) {
            std::cout << line << std::endl;
            input_params.k = stoi(line);
        }
        else {
            std::stringstream buffer(line);
            std::string val;
            std::vector<double> values;
            while(getline(buffer, val, ' ')) {
                values.push_back(stof(val));
            }
            Profile[index - 2] = values;
        }
        index++;
    }
    input_params.Profile = Profile;
    input.close();
    std::string best_string = ProfileMostProbableKmer(input_params.Text, input_params.k, input_params.Profile);
    std::cout << best_string;
    return 0;
}