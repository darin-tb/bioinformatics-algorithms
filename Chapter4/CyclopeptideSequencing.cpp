#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include "MassDict.h"


using count_dict = map<int, int>;

count_dict BuildCountDict(vector<int> spectrum) {
    count_dict spectrum_counts;
    for (auto it = spectrum.begin(); it != spectrum.end(); it++) {
        if (spectrum_counts.count(*it) == 0) {
            spectrum_counts[*it] = 1;
        } else {
            spectrum_counts[*it]++;
        }
    }
    return spectrum_counts;
}

bool CheckConsistency(count_dict spectrum_counts_1, count_dict spectrum_counts_2) {
    for (auto it = spectrum_counts_1.begin(); it != spectrum_counts_1.end(); it++) {
        int spectrum = it->first;
        int count = it->second;
        // if (spectrum_counts_2.count(spectrum) == 0) {
        //     return false;
        // }
        if (count > spectrum_counts_2[spectrum]) {
            return false;
        } 
    }
    return true;
}


template <typename T>
void PrintVector(const vector<T> vec) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

template <typename T, typename U>
void PrintMap(const map<T, U> m) {
    for (auto it = m.begin(); it != m.end(); it++) {
        cout << it->first << " " << it->second << "; ";
    }
    cout << endl;
}

void PrintPeptideMasses(const vector<string> peptides, mass_dict amino_acid_mass) {
    for (int i = 0; i < peptides.size(); i++) {
        string peptide = peptides[i];
        for (int j = 0; j < peptide.size(); j++) {
            char c = peptide[j];
            string s{c};
            if (j != peptide.size() - 1) {
                cout << amino_acid_mass[s] << "-";
            } else {
                cout << amino_acid_mass[s] << " ";
            }
            
        }
        cout << " ";
    }
    cout << endl;
}

template <typename T>
bool VectorEqual(vector<T> v1, vector<T> v2) {
    if (v1.size() != v2.size()) {
        return false;
    } else {
        for (int i = 0; i < v1.size(); i++) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }
}

template <typename T>
bool ValueInVector(T val, vector<T> v) {
    typename vector<T>::iterator it = find(v.begin(), v.end(), val);
    if (it == v.end()) {
        return false;
    } else {
        return true;
    }
}

template <typename T>
bool ValuesInVector(vector<T> vals, vector<T> v) {
    bool all_found = false;
    for (int i = 0; i < vals.size(); i++) {
        T val = vals[i];
        typename vector<T>::iterator it = find(v.begin(), v.end(), val);
        if (it == v.end()) {
            return false;
        } else {
            all_found = true;
        }
    }
    return all_found;
}

vector<int> CyclicSpectrum(string peptide, vector<string> alphabet, mass_dict amino_acid_mass) {
    vector<int> prefix_mass = {0};
    for (int i = 1; i < peptide.size()+1; i++) {
        char c = peptide[i - 1];
        string s{c};
        int new_mass = prefix_mass[i-1] + amino_acid_mass.at(s);
        prefix_mass.push_back(new_mass);
    }
    int peptide_mass = prefix_mass[peptide.size()];
    vector<int> cyclic_spectrum = {0};
    for (int i = 0; i < peptide.size(); i++) {
        for (int j = i+1; j < peptide.size()+1; j++) {
            int new_spectrum = prefix_mass[j] - prefix_mass[i];
            cyclic_spectrum.push_back(new_spectrum);
            if (i > 0 && j < peptide.size()) {
                cyclic_spectrum.push_back(peptide_mass - new_spectrum);
            }
        }
    }
    sort(cyclic_spectrum.begin(), cyclic_spectrum.end());
    return cyclic_spectrum;
}


vector<int> LinearSpectrum(string peptide, vector<string> alphabet, mass_dict amino_acid_mass) {
    vector<int> prefix_mass = {0};
    for (int i = 1; i < peptide.size()+1; i++) {
        char c = peptide[i - 1];
        string s{c};
        int new_mass = prefix_mass[i-1] + amino_acid_mass.at(s);
        prefix_mass.push_back(new_mass);
    }
    vector<int> linear_spectrum = {0};
    for (int i = 0; i < peptide.size(); i++) {
        for (int j = i+1; j < peptide.size()+1; j++) {
            int new_spectrum = prefix_mass[j] - prefix_mass[i];
            linear_spectrum.push_back(new_spectrum);
        }
    }
    sort(linear_spectrum.begin(), linear_spectrum.end());
    return linear_spectrum;
}

int Mass(string peptide, mass_dict amino_acid_mass) {
    int mass = 0;
    for (int i = 0; i < peptide.size(); i++) {
        char c = peptide[i];
        string s{c};
        mass += amino_acid_mass[s];
    }
    return mass;
}

int ParentMass(vector<int> spectrum) {
    return spectrum[spectrum.size() - 1];
}

vector<string> Expand(vector<string> peptides, const vector<string> alphabet) {
    vector<string> expanded_peptides;
    for (int i = 0; i < peptides.size(); i++) {
        string curr_peptide = peptides[i];
        for (int j = 0; j < alphabet.size(); j++) {
            string new_peptide = curr_peptide + alphabet[j];
            expanded_peptides.push_back(new_peptide);
        }
    }
    return expanded_peptides;
}

vector<string> CyclopeptideSequencing(vector<int> spectrum, const vector<string> alphabet, const mass_dict amino_acid_mass) {
    vector<string> candidate_peptides = {""};
    vector<string> final_peptides;
    count_dict spectrum_count_dict = BuildCountDict(spectrum);
    int iteration = 0;
    while (candidate_peptides.size() > 0) {
        
        candidate_peptides = Expand(candidate_peptides, alphabet);
        for (auto it = candidate_peptides.begin(); it != candidate_peptides.end(); it++) {
            string peptide = *it;
            
            vector<int> peptide_linear_spectrum = LinearSpectrum(peptide, alphabet, amino_acid_mass);
            count_dict peptide_count_dict = BuildCountDict(peptide_linear_spectrum);
            
            vector<int> peptide_cyclic_spectrum = CyclicSpectrum(peptide, alphabet, amino_acid_mass);
            
            if (Mass(peptide, amino_acid_mass) == ParentMass(spectrum)) {
                if (VectorEqual(peptide_cyclic_spectrum, spectrum) && !ValueInVector(peptide, final_peptides)) {
                    final_peptides.push_back(peptide);
                }
                candidate_peptides.erase(it);
                it--;
            } else if (!CheckConsistency(peptide_count_dict, spectrum_count_dict)) {
                candidate_peptides.erase(it);
                it--;
            }
        }
        PrintVector(candidate_peptides);
        vector<int> masses;
        for (int i = 0; i < candidate_peptides.size(); i++) {
            masses.push_back(Mass(candidate_peptides[i], amino_acid_mass));
        }
        PrintVector(masses);
        // if (iteration == 1) {
        //     return final_peptides;
        // }
        // iteration++;
    }
    return final_peptides;
}




int main() {
    
    vector<int> spectrum;
    stringstream ss("0 87 87 97 97 101 103 113 114 114 128 184 186 198 200 201 201 211 214 217 231 283 297 298 298 301 304 311 314 315 345 384 385 398 398 411 411 412 417 418 432 495 497 498 499 512 512 514 515 531 546 584 599 602 609 612 615 618 625 628 643 681 696 712 713 715 715 728 729 730 732 795 809 810 815 816 816 829 829 842 843 882 912 913 916 923 926 929 929 930 944 996 1010 1013 1016 1026 1026 1027 1029 1041 1043 1099 1113 1113 1114 1124 1126 1130 1130 1140 1140 1227");
    
    string word;
    while(getline(ss, word, ' ')) {
        spectrum.push_back(stoi(word));
    }
    
    // vector<int> spectrum = {0, 113, 128, 186, 241, 299, 314, 427};
    PrintVector(spectrum);
    vector<string> alphabet;
    mass_dict amino_acid_mass = MassDict("integer_mass_table_unique.txt");
    vector<int> linear_spectrum = LinearSpectrum("PP", alphabet, amino_acid_mass);
    PrintVector(linear_spectrum);
    count_dict pp_count_dict = BuildCountDict(linear_spectrum);
    PrintMap(pp_count_dict);
    
    for (auto it = amino_acid_mass.begin(); it != amino_acid_mass.end(); it++) {
        string str = it->first;
        alphabet.push_back(str);
    }

    PrintMassDict(amino_acid_mass);

    vector<string> peptides = CyclopeptideSequencing(spectrum, alphabet, amino_acid_mass);
    PrintPeptideMasses(peptides, amino_acid_mass);
}
