/*
 * UDynGraph.h
 *
 *  Created on: Oct 14, 2014
 *      Author: aepasto
 */

#ifndef GRAPH_DS_H_
#define GRAPH_DS_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Stores Directed Graph

class Graph {
public:
    // Returns true if the edge is added.
    bool add_edge(const int u, const int v);

    void out_neighbors(const int u, vector<int>* vec) const;

    int out_degree(const int u) const;

    void clear();

    void nodes(vector<int>* vec) const;

    int num_nodes() const;
    int num_edges() const;

    Graph();
    virtual ~Graph();
private:
    unordered_map<int, vector<int> > node_map_;
    unordered_set<int> nodes_;

    int num_nodes_;
    int num_edges_;
};

#endif /* GRAPH_DS_H_ */
