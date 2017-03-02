#include "Graph.h"
#include "GraphUtils.h"
#include "PowerIterationPPR.h"
#include "RandomVisit.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

void output_ranking(int id, string cat, vector<RankedNode>& ranking, unordered_set<int>& to_rank, fstream& output) {
    output << id << "\t" << cat << "\t";
    for (const auto& node : ranking) {
        if (to_rank.find(node.node) !=to_rank.end()){
            output << node.node << ",";
        }
    }
    output << endl;
}

int main(int argc, char** argv) {

    srand(87); // Randomness used to break ties in ranking

    if (argc < 8) {
        cerr
                << "Use: wiki_graph_file; seed_list_file; nodes_to_rank_file; use_random_vist (1 for random visit, 0 for ppr); alpha; iteration_ppr_method; output_file;";
        return 1;
    }

    string graph_file = argv[1];
    string seed_list_file = argv[2];
    string nodes_to_rank_file = argv[3];
    bool use_random_visit = atoi(argv[4]) == 1;
    double alpha = atof(argv[5]);
    double iterations = atoi(argv[6]);
    string output_file = argv[7];

    fstream output(output_file.c_str(), ios::out);

    Graph graph;

    import_graph(graph_file.c_str(), &graph);
    PowerIterationPPR ppr(&graph);
    RandomVisit rv (&graph);

    vector<pair<int, string> > seeds_list;
    import_seed_list(seed_list_file, &seeds_list);

    vector<pair<int, string> > nodes_to_rank_pairs;
    import_seed_list(nodes_to_rank_file, &nodes_to_rank_pairs);

    unordered_set<int> nodes_to_rank;
    for (const auto & node_pair : nodes_to_rank_pairs){
        nodes_to_rank.insert(node_pair.first);
    }

    unordered_map<int, double> scores;

    int n_rank = 0;
    for (const auto& center : seeds_list) {

        unordered_map<int, double> scores;
        cout << "Rank " << n_rank++ << endl;
        if (use_random_visit){
            rv.run(center.first,alpha, &scores);
        } else {
            ppr.run(center.first, alpha, iterations, &scores);
        }
        vector<RankedNode> ranking;
        create_ranking(scores, &ranking);
        output_ranking(center.first, center.second, ranking, nodes_to_rank, output);
    }

    return 0;
}
