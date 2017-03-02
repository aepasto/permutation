#include "Ranking.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cmath>

void shuffle(vector<int>* vec){
    for (int i = 0; i< vec->size(); i++){
        int rnd = rand() % vec->size();
        if (rnd != i){
            int temp = vec->at(i);
            vec->at(i) = vec->at(rnd);
            vec->at(rnd) = temp;
        }
    }
}

//The higher the better with random tie breaking
bool compareRankedNodes(const RankedNode i, const RankedNode j) {
    if (i.score != j.score)
        return i.score > j.score;
    else
        return i.randomPriority > j.randomPriority; //random break of ties. it is consistent and stable.
}

void create_ranking(const unordered_map<int, double>& scores,
    vector<RankedNode>* ranking) {
    ranking->clear();
    for (const auto& elem : scores) {
        RankedNode ranking_elem;
        ranking_elem.node = elem.first;
        ranking_elem.score = elem.second;
        ranking->push_back(ranking_elem);
    }


    sort(ranking->begin(), ranking->end(), compareRankedNodes);
}


Ranking::~Ranking() {
}

Ranking::Ranking(const vector<int>& permutation) :
        permutation_(permutation), num_elements_(permutation.size()) {
    assert(permutation.size() > 0);
    for (size_t i = 0; i < permutation.size(); i++) {
        elem_pos_[permutation[i]] = i;
    }
}

