#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <vector>
#include <algorithm>

#include "RandomVisit.h"

using namespace std;

RandomVisit::RandomVisit(Graph* graph) {
    this->graph = graph;
}

void RandomVisit::run(int seed, double alpha, unordered_map<int, double>* scores) {

    scores->clear();

    // Ensure all nodes in output
    vector<int> nodes;
    graph->nodes(&nodes);
    for (const auto& n : nodes) {
        scores->insert(make_pair(n, 0.0));
    }


            //priority_queue<RankedNode, std::vector<RankedNode>, compareRankedNodesClass> pq;

    //RankedNode r;
    //r.node = seed;
    //pq.push(r);

    unordered_set<int> visited;
    //visited.insert(seed);

    int curr_node = seed;
   // int pos = 0;
    int max_lenght = 5*graph->num_nodes();

    int pos = graph->num_nodes()+1;

    for (int i = 0;  i < max_lenght; i++) {

        //RankedNode r = pq.top();
        //pq.pop();
        //scores->at(r.node) = pos--;
        if (visited.find(curr_node) == visited.end()){
            scores->at(curr_node) = pos--;
            visited.insert(curr_node);
        }
        double ran = ((double) rand() / (RAND_MAX));
        int out_deg = graph->out_degree(curr_node);

        if (ran < alpha || out_deg == 0){ // jumpback
            curr_node = seed;
        } else {
            vector<int> neighbors;
            graph->out_neighbors(curr_node, &neighbors);
            int ran = rand() % out_deg;
            curr_node = neighbors[ran];
        }
    }
}

RandomVisit::~RandomVisit() {
}
