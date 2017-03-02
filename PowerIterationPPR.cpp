#include <iostream>
#include <cassert>
#include <cmath>
#include <queue>
#include <vector>
#include <algorithm>

#include "PowerIterationPPR.h"

using namespace std;

PowerIterationPPR::PowerIterationPPR(Graph* graph) {
    this->graph = graph;
}

void PowerIterationPPR::run(int seed, double alpha, int iterations,
    unordered_map<int, double>* scores) {

    assert(iterations >= 1);

    scores->clear();

    unordered_map<int, double> rProb[2];

    rProb[0][seed] = 1.0;

    int array = 0;

    for (int i = 0; i < iterations; i++) {
        cerr << "iter : " << i << endl;
        int new_array = 1 - array;
        rProb[new_array].clear();
        for (const auto& node_score : rProb[array]) {
            int degreeU = graph->out_degree(node_score.first);

            // If dangling node it always jump back
            if (degreeU == 0) {
                rProb[new_array][seed] += 1.0 * rProb[array][node_score.first];
            } else {
                vector<int> neighbors;
                graph->out_neighbors(node_score.first, &neighbors);
                rProb[new_array][seed] += alpha
                        * rProb[array][node_score.first];
                for (const auto& neighbor : neighbors) {
                    rProb[new_array][neighbor] += (1.0 - alpha)
                            / static_cast<double>(degreeU)
                            * rProb[array][node_score.first];
                }
            }
        }

        array = new_array;
    }

    // Ensure all nodes in output
    vector<int> nodes;
    graph->nodes(&nodes);
    for (const auto& n : nodes) {
        scores->insert(make_pair(n, 0.0));
    }
    for (const auto& node_prob : rProb[array]) {
        scores->at(node_prob.first) = node_prob.second;
    }
}

PowerIterationPPR::~PowerIterationPPR() {
}
