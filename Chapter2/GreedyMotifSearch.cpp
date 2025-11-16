#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <cassert>
#include <numeric>

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
        double A_count = 0;
        double C_count = 0;
        double G_count = 0;
        double T_count = 0;
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
int Score(std::vector<std::string> Motifs, std::array<std::vector<double>,4> Profile) {
    int score = 0;
    char consensus = '\0';
    for (int i = 0; i < Profile[0].size(); i++) {
        int max_index = 0;
        double max_prob = 0;
        for (int j = 0; j < Profile.size(); j++) {
            if (Profile.at(j).at(i) > max_prob) {
                max_index = j;
                max_prob = Profile[j].at(i);
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
// Insert your GreedyMotifSearch function here, along with any subroutines you need
std::vector<std::string> GreedyMotifSearch(std::vector<std::string> Dna, int k, int t){
    std::vector<std::string> BestMotifs = InitializeBestMotifs(Dna, k);
    for (int i = 0; i < Dna.at(0).size() - k + 1; i++) {
        std::vector<std::string> Motifs;
        std::string starting_kmer = Dna.at(0).substr(i, k);
        Motifs.push_back(starting_kmer);
        std::array<std::vector<double>, 4> Profile;
        for (int j = 1; j < t; j++) {
            Profile = BuildProfile(Motifs);
            std::string best_kmer = ProfileMostProbableKmer(Dna.at(j), k, Profile);
            Motifs.push_back(best_kmer);
        }
        std::vector<std::string> TempMotifs;
        for (int i = 0; i < BestMotifs.size() - 1; i++) {
            TempMotifs.push_back(BestMotifs.at(i));
        }
        std::array<std::vector<double>, 4> Profile2 = BuildProfile(TempMotifs);
        if (Score(Motifs, Profile) < Score(BestMotifs, Profile2)) {
            BestMotifs = Motifs;
        }
        // for (int i = 0; i < BestMotifs.size(); i++) {
        //     std::cout << BestMotifs.at(i) << " ";
        // }
    }

    return BestMotifs;
}

int main() {
    std::vector<std::string> Dna;
    Dna.push_back("GGCGTTCAGGCA");
    Dna.push_back("AAGAATCAGTCA");
    Dna.push_back("CAAGGAGTTCGC");
    Dna.push_back("CACGTCAATCAC");
    Dna.push_back("CAATAATATTCG");
    int k = 3;
    int t = 5;
    // Dna.push_back("GCCCAA");
    // Dna.push_back("GGCCTG");
    // Dna.push_back("AACCTA");
    // Dna.push_back("TTCCTT");
    // int k = 3;
    // int t = 4;

    std::vector<std::string> BestMotifs = GreedyMotifSearch(Dna, k, t);
    for (auto it = BestMotifs.begin(); it != BestMotifs.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::array<std::vector<double>, 4> Profile = BuildProfile(BestMotifs);
    for (int i = 0; i < Profile.size(); i++) {
        for (int j = 0; j < Profile.at(i).size(); j++) {
            std::cout << Profile[i].at(j) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}