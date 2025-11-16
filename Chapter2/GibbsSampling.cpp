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
#include <map>
using namespace std;

struct r {
    int random_index;
    double p;
};

int Random(vector<double> probabilities) {
    double prob_sum = 0.0;
    int X = 100;
    vector<int> dist;

    for (int i = 0; i < probabilities.size(); i++) {
        prob_sum += probabilities.at(i);
    }
    if (prob_sum != 1.0) {
        for (int i = 0; i < probabilities.size(); i++) {
            probabilities.at(i) = probabilities.at(i) / prob_sum;
        }
    }
    for (int i = 0; i < probabilities.size(); i++) {
        for (double j = 0.0; j < probabilities.at(i) * X; j = j + 1) {
            dist.push_back(i);
        }
    }
    // for (auto it = dist.begin(); it != dist.end(); it++) {
    //     cout << *it << " ";
    // }
    int random_num = rand() % X;

    return dist.at(random_num);
}


// Please do not remove package declarations because these are used by the autograder.

array<vector<double>, 4> BuildProfile(vector<string> Kmers) {
    array<vector<double>, 4> Profile;
    vector<vector<double>> profile_probs;
    
    for (int i = 0; i < Kmers.at(0).length(); i++) {
        vector<double> intermediate_probs;
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
        vector<double> transposed_probs;
        for (int j = 0; j < profile_probs.size(); j++) {    
            transposed_probs.push_back(profile_probs.at(j).at(i));
        }
        Profile[i] = transposed_probs;
    }
    return Profile;
}


string ProfileMostProbableKmer(string Text, int k, array<vector<double>, 4> Profile){
    double maximum_score = 0.0;
    string best_string = Text.substr(0,k);
    for (int i = 0; i < Text.length() - k + 1; i++) {
        string substring = Text.substr(i, k);
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

int Score(vector<string> Motifs) {
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
                cout << "Error: max_index not found";
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

vector<string> RandomInitializations(vector<string> Dna, int k, int t, int N) {
    // vector<string> TrueBestMotifs;

    
    vector<string> Motifs;
    vector<string> BestMotifs; 

    for (int i = 0; i < Dna.size(); i++) { // randomly select motifs to use for each string in Dna
        int random_index = rand() % (Dna.at(i).length() - k + 1);
        Motifs.push_back(Dna.at(i).substr(random_index,k));
    }
    for (int i = 0; i < Motifs.size(); i++) { // store each randomly selected into motif into the list of best motifs
        BestMotifs.push_back(Motifs.at(i));
    }

    for (int i = 0; i < N; i++) { // iterate N times
        int random_leave_out_index = rand() % (Dna.size()); // randomly choose one of the Dna strings to be left out
        vector<string> one_out_motifs;
        string random_leave_out_dna = Dna.at(random_leave_out_index);
        for (int j = 0; j < Motifs.size(); j++) { // append all other motifs to separate motif vector
            if (j == random_leave_out_index)
                continue;
            one_out_motifs.push_back(Motifs.at(j));
        }
        array<vector<double>, 4> Profile = BuildProfile(one_out_motifs); // build a profile based off of each motif that was not excluded

        vector<string> temp_kmers; // using the profile, compute the probability of observing each of the kmers within the excluded Dna string
        vector<double> kmer_probs;
        for (int j = 0; j < random_leave_out_dna.length() - k + 1; j++) {
            string temp_kmer = random_leave_out_dna.substr(j, k);
            double kmer_prob = 1.0;
            for (int k = 0; k < temp_kmer.length(); k++) {
                if (temp_kmer[k] == 'A') 
                    kmer_prob *= Profile[0].at(k);
                else if (temp_kmer[k] == 'C')
                    kmer_prob *= Profile[1].at(k);
                else if (temp_kmer[k] == 'G')
                    kmer_prob *= Profile[2].at(k);
                else if (temp_kmer[k] == 'T')
                    kmer_prob *= Profile[3].at(k);
            }
            temp_kmers.push_back(temp_kmer);
            kmer_probs.push_back(kmer_prob);
        }

        int random_kmer_index = Random(kmer_probs);
        Motifs.at(random_leave_out_index) = temp_kmers.at(random_kmer_index);

        if (Score(Motifs) < Score(BestMotifs)) {
            BestMotifs.clear();
            for (int j = 0; j < Motifs.size(); j++) {
                BestMotifs.push_back(Motifs.at(j));
            }
        }
    }
    return BestMotifs;
}

vector<string> GibbsSampling(vector<string> Dna, int k, int t, int N){
    vector<string> TrueBestMotifs;
    vector<string> BestMotifs;
    for (int j = 0; j < Dna.size(); j++) {
        int random_index = rand() % (Dna.at(j).length() - k + 1);
        // cout << random_index << endl;
        TrueBestMotifs.push_back(Dna.at(j).substr(random_index,k));
    }
    for (int i = 0; i < 1000; i++) {
        BestMotifs = RandomInitializations(Dna, k, t, N);
        if (Score(BestMotifs) < Score(TrueBestMotifs)) {
            TrueBestMotifs.clear();
            for (int i = 0; i < BestMotifs.size(); i++) {
                TrueBestMotifs.push_back(BestMotifs.at(i));
            }
        }
    }
    return TrueBestMotifs;
}
int main() {

    vector<string> Dna;
    Dna.push_back("CGCCCCTCTCGGGGGTGTTCAGTAAACGGCCA");
    Dna.push_back("GGGCGAGGTATGTGTAAGTGCCAAGGTGCCAG");
    Dna.push_back("TAGTACCGAGACCGAAAGAAGTATACAGGCGT");
    Dna.push_back("TAGATCAAGTTTCAGGTGCACGTCGGTGAACCAA");
    Dna.push_back("AATCCACCAGCTCCACGTGCAATGTTGGCCTA");
    int k = 8;
    int t = 5;
    int N = 100;

    srand(time(NULL));
    vector<string> TrueBestMotifs = GibbsSampling(Dna, k, t, N);

    for (auto it = TrueBestMotifs.begin(); it != TrueBestMotifs.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;



    // vector<double> probabilities;
    // map<int, int> values;
    // probabilities.push_back(0.1);
    // probabilities.push_back(0.2);
    // probabilities.push_back(0.3);
    // probabilities.push_back(0.05);
    // probabilities.push_back(0.1);
    // probabilities.push_back(0.30);
    // for (int i = 0; i < 1000000; i++) {
    //     values[Random(probabilities)]++;
    // }
    // for (auto it = values.begin(); it != values.end(); it++) {
    //     cout << it->first << " " << it->second << endl;
    // }
}