#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <map>
#include <set>
#include <stack>
// Please do not remove package declarations because these are used by the autograder.

using namespace std;
using k_mer_edgelist_t = unordered_map<string, vector<string>>;


// overlap graph - string 1 maps to string 2 if suffix of string1 is equal to prefix of string 2
// Insert your OverlapGraph function here, along with any subroutines you need
k_mer_edgelist_t OverlapGraph(vector<string> Patterns){
    k_mer_edgelist_t edgelist;
    for (int i = 0; i < Patterns.size(); i++) {
        for (int j = 0; j < Patterns.size(); j++) {
            if (i==j)
                continue;
            string suffix = Patterns[i].substr(1, Patterns[i].size());
            string prefix = Patterns[j].substr(0, Patterns[j].size() - 1);
            if (suffix == prefix) {
                if (edgelist.find(Patterns[i]) == edgelist.end()) {
                    edgelist[Patterns[i]] = vector<string> {};
                } 
                if (find(edgelist[Patterns[i]].begin(), edgelist[Patterns[i]].end(), Patterns[j]) == edgelist[Patterns[i]].end())
                edgelist[Patterns[i]].push_back(Patterns[j]);
            }
        }
    }

    return edgelist;
}

// Function to perform DFS
vector<string> DFS(unordered_map<string, vector<string>>& graph, string start) {
    stack<string> s;
    vector<string> visited;

    // Push the start node onto the stack
    s.push(start);

    while (!s.empty()) {
        // Pop a vertex from the stack
        string current = s.top();
        s.pop();

        // If the current node has not been visited yet
        if (find(visited.begin(), visited.end(), current) == visited.end()) {
            // Mark it as visited
            visited.push_back(current);

            // Process the current node
            // cout << current << " ";

            // Push all the adjacent vertices onto the stack
            for (auto neighbor : graph[current]) {
                if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                    s.push(neighbor);
                }
            }
        }
    }
    return visited;
}


string GenomePath(vector<string> path){
    string genome = path[0];
    for (int i = 1; i < path.size(); i++) {
        genome = genome.substr(0, i) + path[i];
        cout << genome << endl;
    }
    return genome;
}

bool HasAll(vector<string> vec1, vector<string> vec2) {
    for (auto value : vec1) {
        if (find(vec2.begin(), vec2.end(), value) == vec2.end())
            return false;
    }
    return true;
}

void EulerianPath(k_mer_edgelist_t graph, string current, vector<string> &kmer_comp, vector<string> Patterns) {
    if (kmer_comp.size() == 0)
        kmer_comp.push_back(current);
    if (HasAll(Patterns, kmer_comp)) {
        return;
    } 
    for (auto it = graph.begin(); it != graph.end(); it++) {
        for (auto neighbor : it->second) {
            if (neighbor == current) {
                it->second.erase(find(it->second.begin(), it->second.end(), current));
            }
        }
    }

    for (auto it = graph.begin(); it != graph.end(); it++) {
        cout << it->first << ": ";
        for (auto neighbor : it->second) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
    


    for (auto neighbor : graph[current]) {
        k_mer_edgelist_t temp_graph = graph;
        temp_graph[current].erase(find(temp_graph[current].begin(), temp_graph[current].end(), neighbor));
        vector<string> temp_visited = DFS(graph, current);
        bool single_connected_component = HasAll(Patterns, temp_visited);
        if (single_connected_component) {
            kmer_comp.push_back(neighbor);
            return EulerianPath(temp_graph, neighbor, kmer_comp, Patterns);
        } else {
            continue;
        }
    }

    // for (auto it = graph.begin(); it != graph.end(); it++) {
    //     if ()
    // }
}


int main() {
    vector<string> Patterns;
    Patterns.push_back("000");
    Patterns.push_back("001");
    Patterns.push_back("010");
    Patterns.push_back("011");
    Patterns.push_back("100");
    Patterns.push_back("101");
    Patterns.push_back("110");
    Patterns.push_back("111");
    // Patterns.push_back("1000");
    // Patterns.push_back("1001");
    // Patterns.push_back("1010");
    // Patterns.push_back("1011");
    // Patterns.push_back("1100");
    // Patterns.push_back("1101");
    // Patterns.push_back("1110");
    // Patterns.push_back("1111");
    k_mer_edgelist_t edgelist = OverlapGraph(Patterns);
    vector<string> kmer_comp;
    EulerianPath(edgelist, Patterns[0], kmer_comp, Patterns);
    
    for (auto it = kmer_comp.begin(); it != kmer_comp.end(); it++) {
        cout << *it << " ";
    }

    cout << endl;

    string genome = GenomePath(kmer_comp);
    cout << genome << endl;



    // for (auto it1 = edgelist.begin(); it1 != edgelist.end(); it1++) {
    //     cout << it1->first << ": ";
    //     for (auto it2 = it1->second.begin(); it2!= it1->second.end(); it2++) {
    //         cout << *it2 << " ";
    //     }
    //     cout << endl;
    // }
}