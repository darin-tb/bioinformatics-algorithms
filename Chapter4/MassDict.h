#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

using mass_dict = unordered_map<string, int>;

mass_dict MassDict(string filename) {
    mass_dict table;
    string line;
    ifstream istream(filename);
    if (istream.is_open()) {
        while(getline(istream, line)) {
            vector<string> words;
            string word;
            stringstream ss(line);
            while (ss >> word) {
                words.push_back(word);
            }
            table[words[0]] = stoi(words[1]);
        }
    }
    istream.close();
    return table;
}

void PrintMassDict(mass_dict table) {
    for (auto it = table.begin(); it != table.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
}


