#include "Graph.h"
#include "GraphUtils.h"
#include "RankDistance.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

int main(int argc, char** argv) {

    srand(time(0));

    if (argc < 2) {
        cerr << "Use: rankings_file;";
        return 1;
    }

    string rankings_file = argv[1];

    vector<Ranking> rankings;
    vector<string> classes_ranks;
    import_rankings(rankings_file, &rankings, &classes_ranks);

    // G.t.Classes as integer
    unordered_map<string, int> class_str_int;
    vector<string> classes_order;
    int n_class = 0;
    for (const auto & class_elem : classes_ranks) {
        if (class_str_int.find(class_elem) == class_str_int.end()) {
            class_str_int[class_elem] = n_class++;
            classes_order.push_back(class_elem);
        }
    }

    assert(n_class < 100);

    RankDistance rd;

    double avg_distance_ap[100][100] = { 0 };
    int avg_distance_count_ap[100][100] = { 0 };
    double avg_distance_tau[100][100] = { 0 };
    int avg_distance_count_tau[100][100] = { 0 };

    for (int i = 0; i < rankings.size(); i += 2) { // To sample
        for (int j = 0; j < rankings.size(); j += 2) {
            if ( i == j) continue;
            cout << i << endl;
            int c1 = class_str_int[classes_ranks[i]];
            int c2 = class_str_int[classes_ranks[j]];

            avg_distance_ap[c1][c2] += rd.ap_distance(rankings[i].to_vector(),
                    rankings[j].to_vector());
            avg_distance_tau[c1][c2] += rd.tau_distance(rankings[i].to_vector(),
                    rankings[j].to_vector());
            avg_distance_count_tau[c1][c2] += 1;
            avg_distance_count_ap[c1][c2] += 1;
        }
    }

    for (int c1 = 0; c1 < n_class; c1++) {
        for (int c2 = 0; c2 < n_class; c2++) {
            cout << classes_order[c1] << " " << classes_order[c2] << " "
                    << (avg_distance_ap[c1][c2]
                            / avg_distance_count_ap[c1][c2]) << " "
                    << (avg_distance_tau[c1][c2]
                            / avg_distance_count_tau[c1][c2]) << "\n";
        }
    }
    return 0;
}
