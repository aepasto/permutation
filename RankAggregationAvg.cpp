#include "RankAggregationAvg.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cmath>

RankAggregationAvg::~RankAggregationAvg() {
}

RankAggregationAvg::RankAggregationAvg() {
}
// output the last item excluding the excluded.
int RankAggregationAvg::last_item(vector<Ranking>* rankings,
    unordered_set<int>& excluded) {

    int lenght = rankings->at(0).num_elements();

    unordered_map<int, int> item_pos;
    for (int i = 0; i < rankings->size(); i++) {
        assert(rankings->at(i).num_elements() == lenght);
        for (int j = 0; j < lenght; j++) {
            if (excluded.find(rankings->at(i).at(j)) == excluded.end()) {
                item_pos[rankings->at(i).at(j)] += j + 1;
            }
        }
    }

    RankedNode last_item_rank;
    last_item_rank.node = -1;
    last_item_rank.score = -1;

    for (const auto& item_pos_pair : item_pos) {
        RankedNode other;
        other.node = item_pos_pair.first;
        other.score = item_pos_pair.second;

        if (compareRankedNodes(other, last_item_rank)) {
            last_item_rank = other;
        }
    }
    return last_item_rank.node;
}

void RankAggregationAvg::rank_aggregation(vector<Ranking>* rankings,
    vector<int>* output) {

    int lenght = rankings->at(0).num_elements();

    unordered_set<int> excluded;
    output->clear();
    output->resize(lenght);

    for (size_t i = 0; i < lenght; i++) {
        int last = last_item(rankings, excluded);
        excluded.insert(last);
        output->at(lenght - 1 - i) = last;
    }
}

// ********************

RankAggregationAvgSimple::~RankAggregationAvgSimple() {
}

RankAggregationAvgSimple::RankAggregationAvgSimple() {
}



void RankAggregationAvgSimple::rank_aggregation(vector<Ranking>* rankings,
    vector<int>* output) {

    int lenght = rankings->at(0).num_elements();

    output->clear();

    unordered_map<int, int> item_pos;
    for (int i = 0; i < rankings->size(); i++) {
        assert(rankings->at(i).num_elements() == lenght);
        for (int j = 0; j < lenght; j++) {
            item_pos[rankings->at(i).at(j)] += j + 1;
        }
    }
    vector<RankedNode> vec;
    for (const auto& item_pos_pair : item_pos) {
        RankedNode other;
        other.node = item_pos_pair.first;
        other.score = -item_pos_pair.second;
        vec.push_back(other);
    }

    sort(vec.begin(), vec.end(), compareRankedNodes);

    for (size_t i = 0; i < lenght; i++) {
        output->push_back(vec[i].node);
    }
}

// ********************

RankAggregationDomination::~RankAggregationDomination() {
}

RankAggregationDomination::RankAggregationDomination() {
}

void RankAggregationDomination::rank_aggregation(vector<Ranking>* rankings,
    vector<int>* output) {

    int lenght = rankings->at(0).num_elements();

    output->clear();

    unordered_map<pair<int, int>, int, pairhash> item_dominated;
    for (int i = 0; i < rankings->size(); i++) {
        assert(rankings->at(i).num_elements() == lenght);
        for (int j = 0; j < lenght; j++) {
            for (int k = j + 1; k < lenght; k++) {
                item_dominated[make_pair(rankings->at(i).at(j),
                        rankings->at(i).at(k))] += 1;
            }
        }
    }
    //cout << "dominated " << endl;


    unordered_map<int, int> item_dominated_num;

    for (const auto& item_pair_dom : item_dominated) {
        if (item_pair_dom.second >= rankings->size()/2){
            item_dominated_num[item_pair_dom.first.first]+=1;
        }
    }

    //cout << "dominated count" << endl;

    vector<RankedNode> vec;

    for (const auto& item_dom : item_dominated_num) {
        RankedNode other;
        other.node = item_dom.first;
        other.score = item_dom.second;
        vec.push_back(other);
    }
    //cout << "sorting" << endl;

    sort(vec.begin(), vec.end(), compareRankedNodes);

    for (size_t i = 0; i < lenght; i++) {
        output->push_back(vec[i].node);
    }
}

