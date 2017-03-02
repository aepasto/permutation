#include "Graph.h"
#include "GraphUtils.h"
#include "KMeansRank.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

void output_assign(string cat, int partition, fstream& output) {
    output << cat << "\t" << partition << "\t";
    output << endl;
}

int main(int argc, char** argv) {


    if (argc < 8) {
        cerr
                << "Use: rankings_file; node_class_list_file; " <<
                "(1 for tua_distance, 0 for ap_distance); k_classes_num;" <<
                "iteration_k_means; (1 for comparsion aggregation, 0 for avg pos aggregation)"
                << " seed_of_random_generator_used ";
        return 1;
    }

    string rankings_file = argv[1];
    string node_class_list_file = argv[2];
    bool use_tau_distance = atoi(argv[3]) == 1;
    int k_classes = atoi(argv[4]);
    int iterations = atoi(argv[5]);
    bool use_comparison_aggregation = atoi(argv[6]) == 1;
    //string output_file = argv[7];
    int seed_random_generator = atoi(argv[7]);

    srand(seed_random_generator);


    vector<pair<int, string> > nodes_to_class_pairs;
    import_seed_list(node_class_list_file, &nodes_to_class_pairs);
    unordered_map<int, string> nodes_to_class;
    for (const auto & nc : nodes_to_class_pairs) {
        nodes_to_class[nc.first] = nc.second;
    }

    //fstream output(output_file.c_str(), ios::out);

    vector<Ranking> rankings;
    vector<string> classes_ranks;
    import_rankings(rankings_file, &rankings, &classes_ranks);


    vector<int> assignment;
    KMeansRank kmeans;
    kmeans.k_means_clustering(classes_ranks, nodes_to_class, use_tau_distance,
            use_comparison_aggregation, k_classes, &rankings, iterations,
            &assignment);

    //for (size_t i = 0; i < rankings.size(); i++) {
    //    output_assign(classes_ranks[i], assignment[i], output);
    //}

    return 0;
}
