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



using namespace std;

//1. For StringReconstruction, first build up a DeBruijn graph from a list of kmers (not an overlap graph - that would require a hamiltonian path)
//2. Next, compute the eulerian path over this list of kmers, visits each node once starting from anywhere, if the DeBruijn graph is already balanced, then compute the Eulerian cycle for the graph
//3. The path gives a vector of strings (DeBruijn Kmers) - string these together to form the genome path for the given DeBruijn graph

// Please do not remove package declarations because these are used by the autograder.

// unordered map does not inherently work on pairs, so we create a hashing function to map pairs to other data structures
struct hash_pair {
    template <class T1, class T2>
    size_t operator () (const pair<T1,T2> &p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};


using read_pair_t = pair<string, string>;
using adjacency_list_t = unordered_map<read_pair_t, vector<read_pair_t>, hash_pair>;
using degree_map = unordered_map<read_pair_t, vector<int>, hash_pair>;


// count number of edges in graph (for looping until all edges are visited)
int EdgeCount(adjacency_list_t G) {
    int count = 0;
    for (auto it1 = G.begin(); it1 != G.end(); it1++) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            count++;
        }
    }
    return count;
}




vector<read_pair_t> BuildCycle(read_pair_t start_node, adjacency_list_t &visited_edges, adjacency_list_t &unvisited_edges) {
    vector<read_pair_t> cycle;
    cycle.push_back(start_node);
    read_pair_t current_node = start_node;
    while (true) {
        int index = rand() % unvisited_edges[current_node].size(); // visit random neighbor
        read_pair_t target = unvisited_edges[current_node][index]; 
        cycle.push_back(target);
        visited_edges[current_node].push_back(target); // mark target node as visited
        unvisited_edges[current_node].erase(find(unvisited_edges[current_node].begin(), unvisited_edges[current_node].end(), target));
        current_node = target;
        if (start_node == target)
            return cycle;
        
    }
}


//Eulerian Cycle function for already balanced graphs
vector<read_pair_t> EulerianCycle(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<read_pair_t> final_cycle;
    int index = rand() % n_nodes;
    int i = 0;
    read_pair_t current_node;
    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    for (auto it = G.begin(); it != G.end(); it++) {
        if (i == index) {
            current_node = it->first;
        }
        visited_edges[it->first] = vector<read_pair_t> {};
        unvisited_edges[it->first] = it->second;
        i++;
    }
    
    
    vector<read_pair_t> cycle = BuildCycle(current_node, visited_edges, unvisited_edges);
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) {
        read_pair_t start_node;
        int index;
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();
                break;
            }
        }


        vector<read_pair_t> sub_cycle = BuildCycle(start_node, visited_edges, unvisited_edges);
        vector<read_pair_t> current_cycle;
        for (int i = 0; i < index; i++) {
            current_cycle.push_back(cycle[i]);
        } 
        for (int i = 0; i < sub_cycle.size() - 1; i++) {
            current_cycle.push_back(sub_cycle[i]);
        }
        for (int i = index; i < cycle.size(); i++) {
            current_cycle.push_back(cycle[i]);
        }
        cycle.clear();
        for (auto it = current_cycle.begin(); it != current_cycle.end(); it++) 
            cycle.push_back(*it);
    }
    return cycle;
}
// Insert your EulerianPath function here, along with any subroutines you need
vector<read_pair_t> EulerianPath(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<read_pair_t> final_cycle;

    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    degree_map degrees;
    for (auto it = G.begin(); it != G.end(); it++) {
        visited_edges[it->first] = vector<read_pair_t> {};
        unvisited_edges[it->first] = it->second;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) { 
            if (degrees.find(*it2) == degrees.end()) {
                degrees[*it2] = vector<int> {0,0}; //indegree first, outdegree second
            }
            degrees[*it2][0]++; 
        }
        if (degrees.find(it->first) == degrees.end()) {
            degrees[it->first] = vector<int> {0,0}; //indegree first, outdegree second
        }
        degrees[it->first][1] += it->second.size();
    }
    read_pair_t last_node;
    read_pair_t current_node;
    bool unbalanced = false;
    for (auto it = degrees.begin(); it != degrees.end(); it++) {
        if (it->second[0] > it->second[1]) {
            last_node = it->first;
            unbalanced = true;
        }  
        if (it->second[1] > it->second[0]) {
            current_node = it->first;
            unbalanced = true;
        }   
    }
    if (!unbalanced) {
        return EulerianCycle(G);
    }
    

    unvisited_edges[last_node].push_back(current_node);
    
    
    vector<read_pair_t> cycle = BuildCycle(current_node, visited_edges, unvisited_edges);
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) {
        read_pair_t start_node;
        int index;
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();
                break;
            }
        }


        vector<read_pair_t> sub_cycle = BuildCycle(start_node, visited_edges, unvisited_edges);
        vector<read_pair_t> current_cycle;
        for (int i = 0; i < index; i++) {
            current_cycle.push_back(cycle[i]);
        } 
        for (int i = 0; i < sub_cycle.size() - 1; i++) {
            current_cycle.push_back(sub_cycle[i]);
        }
        for (int i = index; i < cycle.size(); i++) {
            current_cycle.push_back(cycle[i]);
        }
        cycle.clear();
        for (auto it = current_cycle.begin(); it != current_cycle.end(); it++) 
            cycle.push_back(*it);


    }
    vector<read_pair_t> path;
    int index = 0;
    for (auto it = cycle.begin(); it != cycle.end(); it++) {
        if (*it == last_node) {
            index = it - cycle.begin();
        }
    }
    for (int i = index + 1; i < cycle.size(); i++) {
        path.push_back(cycle[i]);
    }
    for (int i = 1; i < index + 1; i++) {
        path.push_back(cycle[i]);
    }
    return path;
}

string GenomePath(vector<string> path){
    string genome = path[0];
    for (int i = 1; i < path.size(); i++) {
        genome = genome.substr(0, i) + path[i];
    }
    return genome;
}

adjacency_list_t DeBruijnKmers(vector<read_pair_t> k_mers){
    adjacency_list_t edgelist;

    for (int i = 0; i < k_mers.size(); i++) {
        read_pair_t read_pair_prefix;
        read_pair_t read_pair_suffix;
        read_pair_prefix.first = k_mers[i].first.substr(0,k_mers[i].first.size() - 1);
        read_pair_prefix.second = k_mers[i].second.substr(0,k_mers[i].second.size()-1);
        read_pair_suffix.first = k_mers[i].first.substr(1,k_mers[i].first.size()-1);
        read_pair_suffix.second = k_mers[i].second.substr(1,k_mers[i].second.size()-1);

        edgelist[read_pair_prefix].push_back(read_pair_suffix);
    }
    return edgelist;
}


vector<vector<char>> PairedComposition(vector<pair<string, string>> pair_vector, int output_len, int d) {
    vector<vector<char>> paired_composition;

    for (int i = 0; i < pair_vector.size(); i++) {
        vector<char> kmer_char_vec;
        size_t kmer_index = 0;
        size_t processed_state = 0; // processed state should be 0 or 1 or 2, 1 = processed prefix, 2 = processed suffix
        
        for (int j = 0; j < output_len - d; j++) {
            string kmer_to_process;
            if (j < i)
                kmer_char_vec.push_back('\0');
            else {
                if (processed_state == 0) {
                    kmer_to_process = pair_vector[i].first; 
                } else if (processed_state == 1) {
                    kmer_to_process = pair_vector[i].second;
                } else if (processed_state == 2) {
                    kmer_char_vec.push_back('\0');
                    continue;
                }
                kmer_char_vec.push_back(kmer_to_process[kmer_index]);
                kmer_index++;
                if (kmer_index == kmer_to_process.length()){
                    if (processed_state == 0) {
                        for (int k = 0; k < d; k++)
                            kmer_char_vec.push_back('\0');
                        kmer_index = 0;
                        processed_state = 1; 
                    } else if (processed_state == 1) {
                        processed_state = 2;
                    }
                }
            }
        }
        paired_composition.push_back(kmer_char_vec);
    }

    return paired_composition;
}

// Insert your GappedGenomePath function here, along with any subroutines you need
string GappedGenomePath(vector<read_pair_t> GappedPatterns, int k, int d){
    
    int output_len = k + d + k + GappedPatterns.size() - 1;
    vector<vector<char>> paired_composition = PairedComposition(GappedPatterns, output_len, d);

    string reconstructed_string = "";

    for (int i = 0; i < output_len; i++) {
        char temp_char = '\0';
        for (int j = 0; j < GappedPatterns.size(); j++) {
            if (paired_composition[j][i] != temp_char) {
                if (paired_composition[j][i] != '\0')
                    temp_char = paired_composition[j][i];
            }
        } 
        reconstructed_string += temp_char;
    }

    return reconstructed_string;
}

// Insert your StringReconstructionReadPairs function here, along with any subroutines you need
string StringReconstructionReadPairs(vector<read_pair_t> PairedReads, int k, int d){
    adjacency_list_t db = DeBruijnKmers(PairedReads);
    vector<read_pair_t> path = EulerianPath(db);

    for (int i = 0; i < PairedReads.size(); i++) {
        path[i].first += path[i+1].first[path[i+1].first.length() - 1];
        path[i].second += path[i+1].second[path[i+1].second.length() - 1];
    }

    vector<read_pair_t> reconstructed_path;
    for (int i = 0; i < path.size() - 1; i++) {
        reconstructed_path.push_back(path[i]);
    }


    for (int i = 0; i < reconstructed_path.size(); i++) {
        cout << reconstructed_path[i].first << " | " << reconstructed_path[i].second << endl;
    }
    cout << endl;
    string reconstruction = GappedGenomePath(reconstructed_path, k, d);
    for (int i = 0; i < reconstructed_path.size(); i++) {
        if (reconstruction.find(reconstructed_path[i].first) == std::string::npos || reconstruction.find(reconstructed_path[i].second) == std::string::npos)
            return StringReconstructionReadPairs(PairedReads, k, d);
    }
    return reconstruction;
}


int main() {

    vector<read_pair_t> read_pairs;
    read_pair_t pair1, pair2, pair3, pair4, pair5, pair6;
    // pair1.first = "ACAC";
    // pair1.second = "CTCT";

    // pair2.first = "ACAT";
    // pair2.second = "CTCA";

    // pair3.first = "CACA";
    // pair3.second = "TCTC";
   
    // pair4.first = "GACA";
    // pair4.second = "TCTC";
    pair1.first = "GTTT";
    pair1.second = "ATTT";

    pair2.first = "TTTA";
    pair2.second = "TTTG";

    pair3.first = "TTAC";
    pair3.second = "TTGT";
   
    pair4.first = "TACG";
    pair4.second = "TGTA";
    
    pair5.first = "ACGT";
    pair5.second = "GTAT";
    
    pair6.first = "CGTT";
    pair6.second = "TATT";


    read_pairs.push_back(pair1);
    read_pairs.push_back(pair2);
    read_pairs.push_back(pair3);
    read_pairs.push_back(pair4);
    read_pairs.push_back(pair5);
    read_pairs.push_back(pair6);
    

    string reconstruction = StringReconstructionReadPairs(read_pairs, 4, 2);

    cout << reconstruction << endl;

    return 0;
}