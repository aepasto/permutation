#include "RankDistance.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cmath>

RankDistance::~RankDistance() {
}

RankDistance::RankDistance() {
    ground_truth_ = NULL;
    other_ = NULL;
}

// Cost for swapping element that has position right_pos in the ground-truth ranking
double tau_weight_fn(const int right_pos) {
    return 1.0;
}

// Cost for swapping element that has position right_pos in the ground-truth ranking
double ap_weight_fn(const int pos) {
    if (pos <= 0) {
        return 0; // No role as no inversion with before first elements
    } else {
        return static_cast<double>(1.0) / static_cast<double>(pos);
    }
}

double RankDistance::tau_distance(const vector<int>& ground_truth,
    const vector<int>& other) {
    ground_truth_ = new Ranking(ground_truth);
    other_ = new Ranking(other);

    vector<int> vec = other;
    double res = merge_sort(0, vec.size() - 1, &vec, tau_weight_fn);

    delete ground_truth_;
    delete other_;

    return res;
}

double RankDistance::ap_distance(const vector<int>& ground_truth,
    const vector<int>& other) {
    ground_truth_ = new Ranking(ground_truth);
    other_ = new Ranking(other);

    // Start from other and then sort to get to ground_truth
    vector<int> vec = other;
    double res = merge_sort(0, vec.size() - 1, &vec, ap_weight_fn);

    delete ground_truth_;
    delete other_;

    return res * static_cast<double>(vec.size()) / 2.0;
}

// If left is before right in GROUND_TRUTH_ ranking
bool RankDistance::is_before(const int left, const int right) {
    assert(ground_truth_);
    return ground_truth_->is_before(left, right);
}

// Sort from begin (included) to end (included)
double RankDistance::merge_sort(int begin, int end, vector<int>* output,
    double (*weight_fn)(const int)) {
    if (begin >= end) {
        return 0; // already sorted - it is single element
    }

    int middle = ((end - begin + 1) / 2) + begin;

    // Recurse

    double res = 0;
    res += merge_sort(begin, middle - 1, output, weight_fn);
    res += merge_sort(middle, end, output, weight_fn);

    // Merge phase
    vector<int> new_vec;

    int pos_left = begin;
    int pos_right = middle;

    // Total cost if all left-side array is inverted with resp. to an item on right-side array.
    double cost_tot = 0.0;
    for (int i = begin; i <= middle - 1; i++) {
        int pos_gt = ground_truth_->position_of(output->at(i));
        cost_tot += weight_fn(pos_gt);
    }


    while (pos_left <= middle - 1 || pos_right <= end) {
        if (pos_left <= middle - 1 && pos_right <= end) {
            if (is_before(output->at(pos_left), output->at(pos_right))) {
                new_vec.push_back(output->at(pos_left));

                // Reduced cost on left
                int pos_gt = ground_truth_->position_of(output->at(pos_left));
                cost_tot -= weight_fn(pos_gt);

                pos_left++;

            } else {
                // INVERSION !
                new_vec.push_back(output->at(pos_right));

//                cout << "Inversion of " << output->at(pos_left) << " WITH "
//                       << output->at(pos_right) << " of cost " << cost_tot
//                       << endl;
                res += cost_tot;

                pos_right++;

            }

        } else if (pos_left <= middle - 1) {
            new_vec.push_back(output->at(pos_left));
            int pos_gt = ground_truth_->position_of(output->at(pos_left));
            cost_tot -= weight_fn(pos_gt);

            pos_left++;
        } else {
            new_vec.push_back(output->at(pos_right));
            pos_right++;
        }
    }

    for (size_t i = 0; i < new_vec.size(); i++) {
        output->at(begin + i) = new_vec[i];
    }

    return res;
}
