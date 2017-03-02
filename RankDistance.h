#ifndef RANK_DISTANCE_H_
#define RANK_DISTANCE_H_

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Ranking.h"

using namespace std;

// Compute various distances between rankings

double tau_weight_fn(const int right);
double ap_weight_fn(const int right);

class RankDistance {
public:

    RankDistance();
    virtual ~RankDistance();

    double tau_distance(const vector<int>& ground_truth,
        const vector<int>& other);

    double ap_distance(const vector<int>& ground_truth,
         const vector<int>& other);
private:
    Ranking* ground_truth_;
    Ranking* other_;

    // Using comparison based on rankings.
    double merge_sort(int begin, int end, vector<int>* output,
        double (*weight)(const int));

    // Return true if left item is before right item.
    bool is_before(const int left, const int right);
};

#endif /* RANK_DISTANCE_H_ */
