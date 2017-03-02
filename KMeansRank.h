#ifndef K_MEANS_RANK_H_
#define K_MEANS_RANK_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Ranking.h"
#include "RankAggregation.h"
#include "RankAggregationAvg.h"
#include "RankDistance.h"

using namespace std;

// Quality measures
// Assign each elment i in cluster j to the majority label of cluster j.
// Purity is fraction of items correctly assigned (between 0 and 1).
// Assume classes and clustering between 0 and classes-1
double purity(vector<int>& ground_truth, vector<int>& assignment,
    int g_t_classes, int clusters,
    vector<int>* majority_class);


// Return the average over all the centers of the precision of the first pos_k position
// Where if the majority class of a cluster is class A each position in the center is a HIT if it
// is from class A and a MISS otherwise.
double avg_precision_at_k(unordered_map<int, int>& gt_elem_class, vector<int>& majority_class_cluster,
    vector<Ranking>& centers, int pos_k) ;

// Compute a k-means partion of the rankings.

class KMeansRank {
public:

    KMeansRank();
    virtual ~KMeansRank();

    // Output will contain at position output[i] the class of ranking i (in 0...K-1)
    // if true use tau distance otherwise it uses ap distance
    // gt_classes are used to compute purity only
    // node_to_class assign each element of the ranking to a class
    void k_means_clustering(vector<string>& gt_classes, unordered_map<int,string>& node_to_class, bool use_tau_distance,
        bool use_comparison_aggregation, int k, vector<Ranking>* rankings,
        int iterations, vector<int>* output);

private:

    void init_random();
    void assign_to_centers();
    void compute_centers();

    vector<Ranking>* rankings_;
    int k_;
    vector<Ranking> centers_;
    vector<int> assignment_;
    bool use_tau_distance_;
    bool use_comparison_aggregation_;
    double tot_distance_;

};

#endif /* K_MEANS_RANK_H_ */
