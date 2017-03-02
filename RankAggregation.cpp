#include "RankAggregation.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cmath>

RankAggregation::~RankAggregation() {
}

RankAggregation::RankAggregation() :
        rankings_(NULL) {

}

void RankAggregation::rank_aggregation(vector<Ranking>* rankings,
    vector<int>* output) {
    rankings_ = rankings;
    merge_sort(output);
}

bool RankAggregation::is_before(const int left, const int right) {
    assert(rankings_);

    int before = 0;
    int after = 0;

    for (size_t i = 0; i < rankings_->size(); i++) {
        if (rankings_->at(i).is_before(left, right)) {
            before++;
        } else {
            after++;
        }
    }

    if (before != after) {
        return before > after;
    } else { // random tie breaking.
        return rand() % 2 == 0;
    }
}


void RankAggregation::merge_sort(vector<int>* output) {
    assert(rankings_);
    assert(rankings_->size() >= 1);

    vector<int> perm;
    perm.resize(rankings_->at(0).num_elements());
    for (int i = 0; i < rankings_->at(0).num_elements(); i++) {
        perm[i] = rankings_->at(0).at(i);
    }

    shuffle(&perm);

    merge_sort(0, perm.size() - 1, &perm);

    *output = perm;
}

// Sort from begin (included) to end (included)
void RankAggregation::merge_sort(int begin, int end, vector<int>* output) {
    if (begin >= end){
        return; // already sorted - it is single element
    }

    int middle = ((end - begin + 1) / 2) + begin;

    // Recurse
    merge_sort(begin, middle - 1, output);
    merge_sort(middle, end, output);

    // Merge phase
    vector<int> new_vec;

    int pos_left = begin;
    int pos_right = middle;

    while (pos_left <= middle - 1 || pos_right <= end) {
        if (pos_left <= middle - 1 && pos_right <= end) {
            if (is_before(output->at(pos_left), output->at(pos_right))) {
                new_vec.push_back( output->at(pos_left));
                pos_left++;
            } else {
                new_vec.push_back(output->at(pos_right));
                pos_right++;
            }

        } else if (pos_left <= middle - 1) {
            new_vec.push_back(output->at(pos_left));
            pos_left++;
        } else {
            new_vec.push_back(output->at(pos_right));
            pos_right++;
        }
    }

    for (size_t i = 0; i < new_vec.size(); i++) {
        output->at(begin + i) = new_vec[i];
    }
}
