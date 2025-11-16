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

vector<int> BuildCycle(int start_node, int last_node, adjacency_list_t &visited_edges, adjacency_list_t &unvisited_edges) {
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


// Insert your EulerianPath function here, along with any subroutines you need
vector<int> EulerianPath(adjacency_list_t G){
    // srand(time(NULL));
    int n_nodes = G.size();
    vector<int> final_cycle;

    adjacency_list_t visited_edges;
    adjacency_list_t unvisited_edges;
    unordered_map<int, vector<int>> degrees;
    for (auto it = G.begin(); it != G.end(); it++) {
        visited_edges[it->first] = vector<int> {};
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
    int last_node;
    int current_node;
    for (auto it = degrees.begin(); it != degrees.end(); it++) {
        if (it->second[0] > it->second[1]) {
            last_node = it->first;

        }  
        if (it->second[1] > it->second[0]) {
            current_node = it->first;
        }   
    }
    cout << "Degrees" << endl;
    for (auto it1 = degrees.begin(); it1 != degrees.end(); it1++) {
        cout << it1->first << ": ";
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            cout << *it2 << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << current_node << " " << last_node << endl;
    // int current_node = rand() % n_nodes;
    

    unvisited_edges[last_node].push_back(current_node);
    
    
    vector<int> cycle = BuildCycle(current_node, last_node, visited_edges, unvisited_edges); // build cycle randomly walks through the graph at each node until a cycle is reached
    
    int n = 0;
    while(EdgeCount(unvisited_edges) != 0) { // locates the first node that still contains unvisited edges, and uses that as a starting node to build another cycle using the currently unvisited edges.
        int start_node;
        int index;
        for (auto it = cycle.begin(); it != cycle.end(); it++) {
            if (unvisited_edges[*it].size() != 0) {
                start_node = *it;
                auto locate = find(cycle.begin(), cycle.end(), start_node);
                index = locate - cycle.begin();

                cout << index << " " << start_node << endl;
                break;
            }
        }


        vector<int> sub_cycle = BuildCycle(start_node, last_node, visited_edges, unvisited_edges);
        vector<int> current_cycle;
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


    }
    vector<int> path;
    int index = 0;
    for (auto it = cycle.begin(); it != cycle.end(); it++) { // eulerian path is equal to the eulerian cycle when you add an edge to a not fully balanced digraph
        if (*it == last_node) {
            index = it - cycle.begin();
        }
    }
    for (int i = index + 1; i < cycle.size(); i++) { // basically just take the location of the last node + 1, then iterate through the cycle to find the path from start node to last node
        path.push_back(cycle[i]);
    }
    for (int i = 1; i < index + 1; i++) {
        path.push_back(cycle[i]);
    }
    return path;
}

int main() {
    adjacency_list_t G;
    G[0] = vector<int> {1};
    G[1] = vector<int> {2,0};
    G[2] = vector<int> {3,5};
    G[3] = vector<int> {4};
    G[4] = vector<int> {2};
    G[5] = vector<int> {6};
    G[6] = vector<int> {2};
    vector<int> path = EulerianPath(G);
    cout << "Current path" << endl;
    for (auto it = path.begin(); it != path.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

