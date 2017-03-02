


#ifndef RANK_ALG_P_H_
#define RANK_ALG_P_H_

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Ranking.h"


using namespace std;


// Compute a ranking from a set of rankings over the same items in the following way.
// It uses merge sort to sort items starting from u.a.r permutation
// and each time i is compared with j. i is placed before j iff it appears before j in >50% of the
// Rankings in input (in case of ties it uses random tie breaking).


class RankAggregation {
public:

    RankAggregation();
    virtual ~RankAggregation();

    void rank_aggregation(vector<Ranking>* rankings, vector<int>* output);

private:
    vector<Ranking>* rankings_;

    // Using comparison based on rankings.
    void merge_sort(vector<int>* output);
    void merge_sort(int begin, int end, vector<int>* output) ;

    // Return true if left item is before right item.
    bool is_before(const int left, const int right);
};


#endif /* RANK_ALG_P_H_ */
