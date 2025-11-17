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

using namespace std;



void PrintVector(const vector<int> vec) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
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

int main() {
    string peptide = "NCGGDCNIRGPDW";
    vector<string> alphabet;
    mass_dict amino_acid_mass = MassDict("integer_mass_table.txt");

    for (auto it = amino_acid_mass.begin(); it != amino_acid_mass.end(); it++) {
        string str = it->first;
        const char *cstr = str.c_str();
        alphabet.push_back(cstr);
    }

    PrintMassDict(amino_acid_mass);

    vector<int> cyclic_spectrum = CyclicSpectrum(peptide, alphabet, amino_acid_mass);
    PrintVector(cyclic_spectrum);
    cout << cyclic_spectrum.size() << endl;
}