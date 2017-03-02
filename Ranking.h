#ifndef RANKING_P_H_
#define RANKING_P_H_

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>

using namespace std;

void shuffle(vector<int>* vec);


typedef struct RankedNode {
    int node;
    double score;
    int randomPriority; //priority used during tie breaks.

    RankedNode() {
        node = 0;
        score = 0.0f;
        randomPriority = rand();
    }
    RankedNode(int n, double s) {
        node = n;
        score = s;
        randomPriority = rand();
    }

} RankedNode;


bool compareRankedNodes (const RankedNode i, const RankedNode j);
void create_ranking(const unordered_map<int, double>& scores, vector<RankedNode>* ranking);

class compareRankedNodesClass
{
public:

  bool operator() (const RankedNode& lhs, const RankedNode&rhs) const
  {
    return compareRankedNodes(lhs,rhs);
  }
};



class Ranking {
public:

    Ranking(const vector<int>& permutation);
    virtual ~Ranking();

    // True if item i is before j
    inline bool is_before(const int i, const int j) {
        return elem_pos_[i] < elem_pos_[j];
    }

    inline int at(const int pos) {
        return permutation_[pos];
    }

    inline int position_of(const int elem) {
            return elem_pos_[elem];
    }

    inline vector<int> to_vector(){
        return permutation_;
    }

    inline int num_elements(){
        return num_elements_;
    }

private:
    unordered_map<int, int> elem_pos_;
    vector<int> permutation_;
    int num_elements_;
};

#endif /* RANKING_P_H_ */
