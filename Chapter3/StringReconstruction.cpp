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

//1. For StringReconstruction, first build up a DeBruijn graph from a list of kmers (not an overlap graph - that would require a hamiltonian path)
//2. Next, compute the eulerian path over this list of kmers, visits each node once starting from anywhere, if the DeBruijn graph is already balanced, then compute the Eulerian cycle for the graph
//3. The path gives a vector of strings (DeBruijn Kmers) - string these together to form the genome path for the given DeBruijn graph
using namespace std;

// Please do not remove package declarations because these are used by the autograder.
using adjacency_list_t = unordered_map<string, vector<string>>;



adjacency_list_t DeBruijnKmers(std::vector<std::string> k_mers){
    adjacency_list_t edgelist;
    for (int i = 0; i < k_mers.size(); i++) {
        edgelist[k_mers[i].substr(0,k_mers[i].size()-1)].push_back(k_mers[i].substr(1,k_mers[i].size()-1));
    }
    return edgelist;
}



int EdgeCount(adjacency_list_t G) {
    int count = 0;
    for (auto it1 = G.begin(); it1 != G.end(); it1++) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            count++;
        }
    }
    return count;
}

vector<string> BuildCycle(string start_node, adjacency_list_t &visited_edges, adjacency_list_t &unvisited_edges) {
    vector<string> cycle;
    cycle.push_back(start_node);
    string current_node = start_node;
    while (true) {
        int index = rand() % unvisited_edges[current_node].size();
        string target = unvisited_edges[current_node][index];
        cycle.push_back(target);
        visited_edges[current_node].push_back(target);
        unvisited_edges[current_node].erase(find(unvisited_edges[current_node].begin(), unvisited_edges[current_node].end(), target));
        current_node = target;
        if (start_node == target)
            return cycle;
        
    }
}


//Eulerian Cycle function for already balanced graphs
vector<string> EulerianCycle(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<string> final_cycle;
    int index = rand() % n_nodes;
    int i = 0;
    string current_node;
    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    for (auto it = G.begin(); it != G.end(); it++) {
        if (i == index) {
            current_node = it->first;
        }
        visited_edges[it->first] = vector<string> {};
        unvisited_edges[it->first] = it->second;
        i++;
    }
    
    
    

    
    vector<string> cycle = BuildCycle(current_node, visited_edges, unvisited_edges);
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) {
        string start_node;
        int index;
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();
                break;
            }
        }


        vector<string> sub_cycle = BuildCycle(start_node, visited_edges, unvisited_edges);
        vector<string> current_cycle;
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
vector<string> EulerianPath(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<string> final_cycle;

    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    unordered_map<string, vector<int>> degrees;
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
    string last_node;
    string current_node;
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
    
    
    vector<string> cycle = BuildCycle(current_node, visited_edges, unvisited_edges);
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) {
        string start_node;
        int index;
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();
                break;
            }
        }


        vector<string> sub_cycle = BuildCycle(start_node, visited_edges, unvisited_edges);
        vector<string> current_cycle;
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
    vector<string> path;
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
// Insert your StringReconstruction function here, along with any subroutines you need
std::string StringReconstruction(std::vector<std::string> Patterns, int k){
    adjacency_list_t db = DeBruijnKmers(Patterns);
    vector<string> path = EulerianPath(db);
    string genome = GenomePath(path);
    return genome;
}