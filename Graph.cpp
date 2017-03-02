#include "Graph.h"
#include<iostream>
#include<cassert>
#include<algorithm>

Graph::Graph() :
        num_nodes_(0), num_edges_(0) {
}

Graph::~Graph() {
}

void Graph::clear() {
    nodes_.clear();
    node_map_.clear();
    num_nodes_ = 0;
    num_edges_ = 0;
}

bool Graph::add_edge(const int source, const int destination) {
    assert(source >= 0);
    assert(destination >= 0);

    if (node_map_.find(source) != node_map_.end()) {
        const auto & vec = node_map_.at(source);
        if (std::find(vec.begin(), vec.end(), destination) != vec.end()) {
            return false;
        }
    }

    ++num_edges_;

    if (nodes_.find(source) == nodes_.end()) {
        nodes_.insert(source);
        ++num_nodes_;
    }
    if (nodes_.find(destination) == nodes_.end()) {
        nodes_.insert(destination);
        ++num_nodes_;
    }

    node_map_[source].push_back(destination);

    return true;
}

int Graph::num_edges() const {
    return num_edges_;
}

int Graph::num_nodes() const {
    return num_nodes_;
}

void Graph::out_neighbors(const int source, vector<int>* vec) const {
    vec->clear();
    if (node_map_.find(source) != node_map_.end()) {
        const vector<int>& neighbors = node_map_.find(source)->second;
        vec->assign(neighbors.begin(), neighbors.end());
    }
}

void Graph::nodes(vector<int>* vec) const {
    vec->clear();
    vec->assign(nodes_.begin(), nodes_.end());
}

int Graph::out_degree(const int source) const {
    if (node_map_.find(source) == node_map_.end()) {
        return 0;
    }
    return node_map_.find(source)->second.size();
}
