


#ifndef RANK_ALGAvg_P_H_
#define RANK_ALGAvg_P_H_

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Ranking.h"


using namespace std;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};


// Recurisively find items lowest position (on average) remove it, repeat.
class RankAggregationAvg {
public:

    RankAggregationAvg();
    virtual ~RankAggregationAvg();

    int last_item(vector<Ranking>* rankings,
        unordered_set<int>& excluded);
    void rank_aggregation(vector<Ranking>* rankings, vector<int>* output);

};

// Sort by avg. position.
class RankAggregationAvgSimple {
public:

    RankAggregationAvgSimple();
    virtual ~RankAggregationAvgSimple();

    void rank_aggregation(vector<Ranking>* rankings, vector<int>* output);

};

// Sort by avg. position.
class RankAggregationDomination {
public:

    RankAggregationDomination();
    virtual ~RankAggregationDomination();

    void rank_aggregation(vector<Ranking>* rankings, vector<int>* output);

};

#endif /* RANK_ALGAvg_P_H_ */
