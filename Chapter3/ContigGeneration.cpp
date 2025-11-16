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

using namespace std;

using adjacency_list_t = unordered_map<string, vector<string>>;
using degree_map = unordered_map<string, vector<int>>;

template <typename T>
void PrintEdgelist(const unordered_map<string, T>& edgelist){
    for (auto it = edgelist.begin(); it != edgelist.end(); it++) {
        cout << it->first << ": ";
        for (auto val : it->second) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void PrintVector(const vector<string> vec) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}


int CountNodes(adjacency_list_t edgelist) {
    int n = 0;
    for (auto it = edgelist.begin(); it != edgelist.end(); it++) {
        n++;
    }
    n++;
    return n;
}


//build deBruijn graph from kmers
// for each kmer, kmer[0,k-1] maps to kmer[1,k]
adjacency_list_t DeBruijnKmers(const vector<string>& k_mers){
    adjacency_list_t edgelist;
    for (int i = 0; i < k_mers.size(); i++) {
        edgelist[k_mers[i].substr(0,k_mers[i].size()-1)].push_back(k_mers[i].substr(1,k_mers[i].size()-1));
    }
    return edgelist;
}

degree_map BuildDegreeMap(const adjacency_list_t& G) {
    unordered_map<string, vector<int>> degrees;
    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    for (auto it = G.begin(); it != G.end(); it++) {
        visited_edges[it->first] = vector<string> {};
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
    return degrees;
}

vector<string> NonBranchingNodes(const adjacency_list_t& edgelist, degree_map& degrees) {
    vector<string> non_branching_nodes;
    for (auto it = degrees.begin(); it != degrees.end(); it++) {
        string v = it->first;
        int v_indegree = it->second[0];
        int v_outdegree = it->second[1];
        if (v_indegree == 1 && v_outdegree == 1) {
            non_branching_nodes.push_back(v);
        }
    }
    return non_branching_nodes;
}

string FindParentNode(const adjacency_list_t& edgelist, string child_node) {
    for (auto it1 = edgelist.begin(); it1 != edgelist.end(); it1++) {
        vector<string> out_nodes = it1->second;
        for (auto it2 = out_nodes.begin(); it2 != out_nodes.end(); it2++) {
            if (*it2 == child_node) {
                return it1->first; 
            }
        }
    };
    return "";
}

string FindChildNode(const adjacency_list_t& edgelist, string parent_node) {
    for (auto it1 = edgelist.begin(); it1 != edgelist.end(); it1++) {
        if (it1->first == parent_node) {
            return it1->second[0];
        }
    }
    return "";
}

bool ValueInVector(const string s, const vector<string> vec) {
    for (auto it = vec.begin(); it != vec.end(); it++) {
        if (*it == s) {
            return true;
        }
    }
    return false;
}


// find all maximal non branching paths, represents all contigs found in DeBruijn graph
vector<vector<string>> MaximalNonBranchingPaths(const adjacency_list_t& edgelist, degree_map& degrees) {
    vector<vector<string>> paths;
    vector<string> non_branching_nodes = NonBranchingNodes(edgelist, degrees);
    vector<string> seen_nodes;


    
    for (auto it = non_branching_nodes.begin(); it != non_branching_nodes.end(); it++) {

        vector<string> path;
        string node = *it;
        string start_node = node;

        if (ValueInVector(node, seen_nodes)) {
            continue;
        }

        while(!ValueInVector(node, seen_nodes)) {
            path.push_back(node);
            if (!ValueInVector(node, non_branching_nodes)) {
                break;
            }
            seen_nodes.push_back(node);
            node = FindParentNode(edgelist, node);
        }
        
        reverse(path.begin(), path.end());
        
        node = FindChildNode(edgelist, start_node);
        if (ValueInVector(node, seen_nodes)) {
            path.push_back(node);
            node = FindChildNode(edgelist, start_node);
        }

        while(!ValueInVector(node, seen_nodes)) {
            path.push_back(node);
            if (!ValueInVector(node, non_branching_nodes)) {
                break;
            }
            
            seen_nodes.push_back(node);
            node = FindChildNode(edgelist, node);
        }
        paths.push_back(path);
    }

    for (auto it1 = edgelist.begin(); it1 != edgelist.end(); it1++) {
        string start_node = it1->first;
        vector<string> end_nodes = it1->second;
        if (ValueInVector(start_node, seen_nodes)) {
            continue;
        }
        for (auto it2 = end_nodes.begin(); it2 != end_nodes.end(); it2++) {
            string end_node = *it2;
            if (ValueInVector(end_node, seen_nodes)) {
                continue;
            } else {
                vector<string> contig = {start_node, end_node};
                paths.push_back(contig);
            }
        }
    }



    return paths;
}


// reconstruct string from path in debruijn graph
string GenomePath(vector<string> path){
    string genome = path[0];
    for (int i = 1; i < path.size(); i++) {
        genome = genome.substr(0, i) + path[i];
    }
    return genome;
}

vector<string> ContigGeneration(vector<string> Patterns) {
    adjacency_list_t edgelist = DeBruijnKmers(Patterns);
    degree_map degrees = BuildDegreeMap(edgelist);

    PrintEdgelist(edgelist);
    PrintEdgelist(degrees);

    vector<vector<string>> non_branching_paths = MaximalNonBranchingPaths(edgelist, degrees);
    vector<string> contigs;
    for (auto it = non_branching_paths.begin(); it != non_branching_paths.end(); it++) {
        string genome = GenomePath(*it);
        cout << genome << endl;
        contigs.push_back(genome);
    }
    return contigs;
}

int main() {
    vector<string> k_mers;
    // k_mers.push_back("ATG");
    // k_mers.push_back("ATG");
    // k_mers.push_back("TGT");
    // k_mers.push_back("TGG");
    // k_mers.push_back("CAT");
    // k_mers.push_back("GGA");
    // k_mers.push_back("GAT");
    // k_mers.push_back("AGA");

    // k_mers.push_back("AG");
    // k_mers.push_back("GT");
    // k_mers.push_back("GC");
    // k_mers.push_back("TA");

    // k_mers.push_back("GTT");
    // k_mers.push_back("TTA");
    // k_mers.push_back("TAC");
    // k_mers.push_back("TTT");

    k_mers.push_back("GAGA");
    k_mers.push_back("AGAG");
    k_mers.push_back("AACG");
    k_mers.push_back("ACGT");
    k_mers.push_back("ACGG");



    ContigGeneration(k_mers);

} 