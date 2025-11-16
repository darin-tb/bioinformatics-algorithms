#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
#include <ranges>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <stdlib.h>
#include <time.h>
#include <map>
using namespace std;
// Please do not remove package declarations because these are used by the autograder.

using adjacency_list_t = unordered_map<int, vector<int>>;


int EdgeCount(adjacency_list_t G) {
    int count = 0;
    for (auto it1 = G.begin(); it1 != G.end(); it1++) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            count++;
        }
    }
    return count;
}

vector<int> BuildCycle(int start_node, adjacency_list_t &visited_edges, adjacency_list_t &unvisited_edges) {
    vector<int> cycle;
    cycle.push_back(start_node);
    int current_node = start_node;
    while (true) {
        int index = rand() % unvisited_edges[current_node].size();
        int target = unvisited_edges[current_node][index];
        cycle.push_back(target);
        visited_edges[current_node].push_back(target);
        unvisited_edges[current_node].erase(find(unvisited_edges[current_node].begin(), unvisited_edges[current_node].end(), target));
        current_node = target;
        if (start_node == target)
            return cycle;
        
    }
}


// Insert your EulerianCycle function here, along with any subroutines you need
vector<int> EulerianCycle(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<int> final_cycle;

    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    for (auto it = G.begin(); it != G.end(); it++) {
        visited_edges[it->first] = vector<int> {};
        unvisited_edges[it->first] = it->second;
    }
    
    int current_node = rand() % n_nodes;
    

    
    vector<int> cycle = BuildCycle(current_node, visited_edges, unvisited_edges);
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) {
        int start_node;
        int index;
        // determines node with unvisited edges within cycle, and designates as starting node
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();

                cout << index << " " << start_node << endl;
                break;
            }
        }

        // builds up new cycle starting at node with unvisited edges found in previous cycle
        vector<int> sub_cycle = BuildCycle(start_node, visited_edges, unvisited_edges);
        vector<int> current_cycle;

        // then insert new cycle into previous cycle, repeat until no unvisited edges are left
        for (int i = 0; i < index; i++) {
            current_cycle.push_back(cycle[i]);
        } 
        for (int i = 0; i < sub_cycle.size() - 1; i++) {
            current_cycle.push_back(sub_cycle[i]);
        }
        for (int i = index; i < cycle.size(); i++) {
            current_cycle.push_back(cycle[i]);
        }
        for (auto it = current_cycle.begin(); it != current_cycle.end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
        cycle.clear();
        for (auto it = current_cycle.begin(); it != current_cycle.end(); it++) 
            cycle.push_back(*it);


        cout << "----------------------------------------------------" << endl;
        cout << "Visited Edges" << endl;
        for (auto it1 = visited_edges.begin(); it1 != visited_edges.end(); it1++) {
            cout << it1->first << ": ";
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                cout << *it2 << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "Unvisited Edges" << endl;
        for (auto it1 = unvisited_edges.begin(); it1 != unvisited_edges.end(); it1++) {
            cout << it1->first << ": ";
            for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
                cout << *it2 << " ";
            }
            cout << endl;
        }
        cout << endl;
        

        cout << "Current Cycle" << endl;
        for (auto it = current_cycle.begin(); it != current_cycle.end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
        // cin.ignore();
    }
    return cycle;
}

int main() {
    adjacency_list_t G;
    G[0] = vector<int> {3};
    G[1] = vector<int> {0};
    G[2] = vector<int> {1,6};
    G[3] = vector<int> {2};
    G[4] = vector<int> {2};
    G[5] = vector<int> {4};
    G[6] = vector<int> {5,8};
    G[7] = vector<int> {9};
    G[8] = vector<int> {7};
    G[9] = vector<int> {6};
    EulerianCycle(G);
}

