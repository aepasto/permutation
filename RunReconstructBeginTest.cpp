#include "Graph.h"
#include "GraphUtils.h"
#include "RankDistance.h"
#include "RankAggregation.h"
#include "RankAggregationAvg.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

struct Result {
    double prec_at_10;
    double inversion_at_10;
    double inversion_all;
};

// GT is 1234...100
// return precision @10, correc pairs top 10
Result print_precision(int samples, vector<int> ranking) {
    int frac_top10_at10 = 0;
    for (int i = 0; i < 20; i++) {
        if (ranking[i] <= 10) {
            frac_top10_at10 += 1;
        }
    }
    int inversion_top_ten = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (ranking[i] > ranking[j]) {
                inversion_top_ten += 1;
            }
        }
    }
    int inversion_all = 0;
    for (int i = 0; i < 100; i++) {
        for (int j = i + 1; j < 100; j++) {
            if (ranking[i] > ranking[j]) {
                inversion_all += 1;
            }
        }
    }

    Result result;
    result.prec_at_10 = (double) (frac_top10_at10) / 10.0;
    result.inversion_at_10 = 1.0 - (double) (2 * inversion_top_ten) / (10 * 9);
    result.inversion_all = 1.0 - (double) (2 * inversion_all) / (100 * 99);
    return result;
}

int main(int argc, char** argv) {

    srand(time(0));

    if (argc < 3) {
        cerr
                << "Use: rankings_file; 0 for comparison; 1 for average based (recursive); 2 for simple avg based (only one sort); 3 for domination based ";
        return 1;
    }

    string rankings_file = argv[1];

    bool use_comparision_based = atoi(argv[2]) == 0;
    bool use_average_based = atoi(argv[2]) == 1;
    bool use_simple_average_based = atoi(argv[2]) == 2;
    bool use_domination_based = atoi(argv[2]) == 3;

    vector<Ranking> rankings;
    vector<string> classes_ranks;
    import_rankings(rankings_file, &rankings, &classes_ranks);

    vector<int> order;
    for (int i = 0; i < rankings.size(); i++) {
        order.push_back(i);
    }
    cout << "Samples,Prec@10,CorrPairs@10,CorrPairsAll\n";

    for (int i = 1; i < 31; i++) {
        int samples = i * 10;

        double avg_prec10 = 0;
        double avg_inver10 = 0;
        double avg_invers_all = 0;
        int tests = 300;
        for (int t = 0; t < tests; t++) {
            shuffle(&order);
            vector<int> res;
            vector<Ranking> sub_sample;
            for (int j = 0; j < samples; j++) {
                sub_sample.push_back(rankings[order[j]]);
            }
            if (use_average_based) {
                RankAggregationAvg ra;
                ra.rank_aggregation(&sub_sample, &res);

            } else if (use_comparision_based) {
                RankAggregation ra;
                ra.rank_aggregation(&sub_sample, &res);

            } else if (use_simple_average_based) {
                RankAggregationAvgSimple ra;
                ra.rank_aggregation(&sub_sample, &res);

            } else if (use_domination_based) {
                RankAggregationDomination ra;
                ra.rank_aggregation(&sub_sample, &res);

            }

            auto p = print_precision(samples, res);
            avg_prec10 += p.prec_at_10;
            avg_inver10 += p.inversion_at_10;
            avg_invers_all += p.inversion_all;
        }
        avg_prec10 /= tests;
        avg_inver10 /= tests;
        avg_invers_all /= tests;
        cout << samples << "," << avg_prec10 << "," << avg_inver10 << ","
                << avg_invers_all << endl;
    }

    return 0;
}
