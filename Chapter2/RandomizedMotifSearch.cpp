#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <stdlib.h>
#include <time.h> 
#include <iostream>

// Please do not remove package declarations because these are used by the autograder.

std::vector<std::string> InitializeBestMotifs(std::vector<std::string> Dna, int k) {
    std::vector<std::string> InitialBestMotifs;
    for (int i = 0; i < Dna.size(); i++) {
        InitialBestMotifs.push_back(Dna.at(i).substr(0,k));
    }
    return InitialBestMotifs;
}

std::array<std::vector<double>, 4> BuildProfile(std::vector<std::string> Kmers) {
    std::array<std::vector<double>, 4> Profile;
    std::vector<std::vector<double>> profile_probs;
    
    for (int i = 0; i < Kmers.at(0).length(); i++) {
        std::vector<double> intermediate_probs;
        double A_count = 1;
        double C_count = 1;
        double G_count = 1;
        double T_count = 1;
        for (int j = 0; j < Kmers.size(); j++) {
            if (Kmers.at(j)[i] == 'A') 
                A_count++;
            else if (Kmers.at(j)[i] == 'C')
                C_count++;            
            else if (Kmers.at(j)[i] == 'G')
                G_count++;
            else if (Kmers.at(j)[i] == 'T')
                T_count++;
        }
        double sum = A_count + C_count + G_count + T_count;
        intermediate_probs.push_back(A_count / sum);
        intermediate_probs.push_back(C_count / sum);           
        intermediate_probs.push_back(G_count / sum);
        intermediate_probs.push_back(T_count / sum);
        profile_probs.push_back(intermediate_probs);
    }
    for (int i = 0; i < profile_probs.at(0).size(); i++) {
        std::vector<double> transposed_probs;
        for (int j = 0; j < profile_probs.size(); j++) {    
            transposed_probs.push_back(profile_probs.at(j).at(i));
        }
        Profile[i] = transposed_probs;
    }
    return Profile;
}


std::string ProfileMostProbableKmer(std::string Text, int k, std::array<std::vector<double>, 4> Profile){
    double maximum_score = 0.0;
    std::string best_string = Text.substr(0,k);
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
        if (score > maximum_score) {
            maximum_score = score;
            best_string = substring;
        }
    }
    return best_string;
}

int Score(std::vector<std::string> Motifs) {
    int score = 0;
    char consensus = '\0';
    for (int i = 0; i < Motifs.at(0).size(); i++) {
        int A_count = 1;
        int C_count = 1;
        int G_count = 1;
        int T_count = 1;
        for (int j = 0; j < Motifs.size(); j++) {
            if (Motifs.at(j)[i] == 'A') 
                A_count++;
            else if (Motifs.at(j)[i] == 'C')
                C_count++;            
            else if (Motifs.at(j)[i] == 'G')
                G_count++;
            else if (Motifs.at(j)[i] == 'T')
                T_count++;
        }
        int v[4] = {A_count, C_count, G_count, T_count};
        int max_index = 0;
        int max_value = v[0];
        for (int i = 0; i < sizeof(v) / sizeof(int); i++) {
            if (v[i] > max_value) {
                max_index = i;
                max_value = v[i];
            }
        }
        switch (max_index){
            case 0:
                consensus = 'A';
                break;
            case 1:
                consensus = 'C';
                break;
            case 2:
                consensus = 'G';
                break;
            case 3:
                consensus = 'T';
                break;
            default:
                std::cout << "Error: max_index not found";
                exit(0);
        }
        for (int l = 0; l < Motifs.size(); l++) {
            if (Motifs.at(l)[i] != consensus) { 
                score++;
            }
        }
    }
    return score;
}

std::vector<std::string> RandomizedMotifSearch(std::vector<std::string> Dna, int k, int t){
// std::vector<std::string> TrueBestMotifs;

    // srand(time(NULL));
    std::vector<std::string> Motifs;
    std::vector<std::string> BestMotifs; 

    for (int j = 0; j < Dna.size(); j++) {
        int random_index = rand() % (Dna.at(j).length() - k + 1);
        // std::cout << random_index << std::endl;
        Motifs.push_back(Dna.at(j).substr(random_index,k));
    }
    for (int l = 0; l < Motifs.size(); l++) {
        BestMotifs.push_back(Motifs.at(l));
        // TrueBestMotifs.push_back(Motifs.at(l));
    }

    while (true) {
        std::array<std::vector<double>, 4> Profile = BuildProfile(Motifs);
        Motifs.clear();
        for (int m = 0; m < Dna.size(); m++) {
            Motifs.push_back(ProfileMostProbableKmer(Dna.at(m), k, Profile));
        }
        if (Score(Motifs) < Score(BestMotifs)) {
            BestMotifs.clear();
            for (int n = 0; n < Motifs.size(); n++) {
                BestMotifs.push_back(Motifs.at(n));
            }
        } else {
            return BestMotifs;
        }
    }
    // if (Score(BestMotifs) < Score(TrueBestMotifs)) {
    //     TrueBestMotifs.clear();
    //      for (int o = 0; o < BestMotifs.size(); o++) {
    //         TrueBestMotifs.push_back(BestMotifs.at(o));
    //     }
    // }
}

int main() {
    std::vector<std::string> Dna;
    Dna.push_back("CGCCCCTCTCGGGGGTGTTCAGTAAACGGCCA");
    Dna.push_back("GGGCGAGGTATGTGTAAGTGCCAAGGTGCCAG");
    Dna.push_back("TAGTACCGAGACCGAAAGAAGTATACAGGCGT");
    Dna.push_back("TAGATCAAGTTTCAGGTGCACGTCGGTGAACCAA");
    Dna.push_back("TCCACCAGCTCCACGTGCAATGTTGGCCTA");
    int k = 8;
    int t = 5;

    srand(time(NULL));
    std::vector<std::string> BestMotifs;
    for (int i = 0; i < 1000; i++) {
        BestMotifs = RandomizedMotifSearch(Dna, k, t);
    }


    for (auto it = BestMotifs.begin(); it != BestMotifs.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::array<std::vector<double>, 4> Profile = BuildProfile(BestMotifs);
    for (int i = 0; i < Profile.size(); i++) {
        for (int j = 0; j < Profile[i].size(); j++) {
            std::cout << Profile[i].at(j) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}