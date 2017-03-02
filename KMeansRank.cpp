#include "KMeansRank.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <climits>

// Quality measures
// Assign each elment i in cluster j to the majority label of cluster j.
// Purity is fraction of items correctly assigned (between 0 and 1).
// Assume classes and clustering between 0 and classes-1
double purity(vector<int>& ground_truth, vector<int>& assignment,
    int g_t_classes, int clusters, vector<int>* majority_class) {
    unordered_map<int, int>* count_gt_class_per_cluster = new unordered_map<int,
            int> [clusters];
    ;

    majority_class->clear();
    for (size_t i = 0; i < assignment.size(); i++) {
        count_gt_class_per_cluster[assignment[i]][ground_truth[i]] += 1;
    }
    for (int i = 0; i < clusters; i++) {
        int max_class = -1; // Do not change! used in the next function as a flag
        int max_value = -1;
        int total = 0;
        for (const auto& gt_class_count : count_gt_class_per_cluster[i]) {
            if (gt_class_count.second >= max_value) {
                max_class = gt_class_count.first;
                max_value = gt_class_count.second;
            }
            total += gt_class_count.second;
        }

        //cout << max_class << " - " << count_gt_class_per_cluster[i].size() << endl;
        //cout.flush();
        assert(
                count_gt_class_per_cluster[i].size() == 0
                        || (max_class >= 0 && max_class <= g_t_classes));
        majority_class->push_back(max_class);

        //cout << "MAJORITY: " << max_class << " TOTAL: " << total << " "
        //        << " INCLASS: " << max_value << endl;
    }
    delete[] count_gt_class_per_cluster;
    int correct = 0;
    for (size_t i = 0; i < assignment.size(); i++) {
        if (ground_truth[i] == majority_class->at(assignment[i])) {
            correct += 1;
        }
    }

    return static_cast<double>(correct) / static_cast<double>(assignment.size());
}

// Return the average over all the centers of the precision of the first pos_k position
// Where if the majority class of a cluster is class A each position in the center is a HIT if it
// is from class A and a MISS otherwise.
double avg_precision_at_k(unordered_map<int, int>& gt_elem_class,
    vector<int>& majority_class_cluster, vector<Ranking>& centers, int pos_k) {
    assert(pos_k > 0);
    int clusters = 0;
    int correct = 0;

    int correct_in_cluster = 0;
    for (int i = 0; i < centers.size(); i++) {
        if (majority_class_cluster[i] == -1) {
            // empty cluster
            continue;
        }
        correct_in_cluster = 0;
        clusters++;
        for (int j = 0; j < pos_k; j++) {
            if (majority_class_cluster[i] == gt_elem_class[centers[i].at(j)]) {
                correct++;
                correct_in_cluster++;
            }
        }
        //if (pos_k == 5) {
        //    cout << "Clust: " << majority_class_cluster[i] << " "
        //            << static_cast<double>(correct_in_cluster)
        //                    / static_cast<double>(pos_k) << endl;
        //}
    }

    return static_cast<double>(correct) / static_cast<double>(pos_k * clusters);
}

// Return the average over all the centers of the precision of the first pos_k position
// Where if the majority class of a cluster is class A each position in the center is a HIT if it
// is from class A and a MISS otherwise.
double avg_ROC(unordered_map<int, int>& gt_elem_class,
    vector<int>& majority_class_cluster, vector<Ranking>& centers) {

    int clusters = 0;
    double roc_sum = 0;

    int correct_in_cluster = 0;

    for (int i = 0; i < centers.size(); i++) {
        if (majority_class_cluster[i] == -1) {
            // empty cluster
            continue;
        }
        correct_in_cluster = 0;
        clusters++;

        int previousPositive = 0, previousNegative = 0;
        double avg = 0.0;
        for (int j = 0; j < centers[i].num_elements(); j++) {
            if (majority_class_cluster[i] == gt_elem_class[centers[i].at(j)]) {
                previousPositive++;
            } else {
                avg += previousPositive;
                previousNegative++;

            }
        }
        double roc = (avg / previousPositive) / previousNegative;
        roc_sum += roc;

        //cout << "Clust: " << majority_class_cluster[i] << " " << roc << endl;
    }

    return static_cast<double>(roc_sum) / static_cast<double>(clusters);
}

KMeansRank::~KMeansRank() {
}

KMeansRank::KMeansRank() :
        rankings_(NULL), k_(0), use_tau_distance_(true), tot_distance_(0.0) {

}

void KMeansRank::init_random() {

    assignment_.clear();
    centers_.clear();
    for (size_t i = 0; i < rankings_->size(); i++) {
        assignment_.push_back(rand() % k_);
    }
    for (int i = 0; i < k_; i++) {
        vector<int> empty = { 1 };
        Ranking r(empty);
        centers_.push_back(r);
    }
    compute_centers();

    // distance first step
    RankDistance rd;
    for (size_t i = 0; i < rankings_->size(); i++) {
        if (use_tau_distance_) {
            tot_distance_ += rd.tau_distance(
                    centers_[assignment_[i]].to_vector(),
                    rankings_->at(i).to_vector());
        } else {
            tot_distance_ += rd.ap_distance(
                    centers_[assignment_[i]].to_vector(),
                    rankings_->at(i).to_vector());
        }
    }

    /*vector<int> rand_perm;
     for (size_t i = 0; i < rankings_->size(); i++) {
     rand_perm.push_back(i);
     }
     shuffle(&rand_perm);
     centers_.clear();
     for (int i = 0; i < k_; i++) {
     Ranking r = rankings_->at(rand_perm[i]);
     centers_.push_back(r);
     }
     assign_to_centers();*/
}

void KMeansRank::compute_centers() {
    vector<Ranking>* rank_class = new vector<Ranking> [k_];
    for (size_t i = 0; i < rankings_->size(); i++) {
        assert(assignment_[i] >= 0);
        assert(assignment_[i] <= k_ - 1);
        rank_class[assignment_[i]].push_back(rankings_->at(i));
    }

    for (int i = 0; i < k_; i++) {
        if (rank_class[i].size() > 0) {
            vector<int> new_center;

            if (use_comparison_aggregation_) {
                RankAggregation ra;
                ra.rank_aggregation(&(rank_class[i]), &new_center);
            } else {
                RankAggregationAvg ra;
                ra.rank_aggregation(&(rank_class[i]), &new_center);
            }
            Ranking new_center_r(new_center);
            centers_[i] = new_center_r;

        }
    }
    delete[] rank_class;
}

void KMeansRank::assign_to_centers() {

    tot_distance_ = 0.0;
    assignment_.clear();
    vector<double> min_distance;
    vector<int> min_distance_class;

    for (size_t i = 0; i < rankings_->size(); i++) {
        min_distance.push_back(numeric_limits<double>::max());
        min_distance_class.push_back(0);
    }
    for (int cl = 0; cl < k_; cl++) {
        RankDistance rd;
        for (size_t i = 0; i < rankings_->size(); i++) {
            double distance = 0;
            if (use_tau_distance_) {
                distance = rd.tau_distance(centers_[cl].to_vector(),
                        rankings_->at(i).to_vector());
            } else {
                distance = rd.ap_distance(centers_[cl].to_vector(),
                        rankings_->at(i).to_vector());
            }
            if (distance < min_distance[i]) {
                min_distance_class[i] = cl;
                min_distance[i] = distance;
            }
        }

    }
    for (size_t i = 0; i < rankings_->size(); i++) {
        assignment_.push_back(min_distance_class[i]);
        tot_distance_ += min_distance[i];
    }

}

void KMeansRank::k_means_clustering(vector<string>& gt_classes_rank,
    unordered_map<int, string>& gt_elements_class, bool use_tau_distance,
    bool use_comparison_aggregation, int k, vector<Ranking>* rankings,
    int iterations, vector<int>* output) {

    use_tau_distance_ = use_tau_distance;
    k_ = k;
    rankings_ = rankings;
    assignment_.clear();
    use_comparison_aggregation_ = use_comparison_aggregation;

    // G.t.Classes as integer
    unordered_map<string, int> class_str_int;
    int n_class = 0;
    for (const auto & class_elem : gt_classes_rank) {
        if (class_str_int.find(class_elem) == class_str_int.end()) {
            //cout << "CLASS: " << class_elem << " = " << n_class << endl;
            class_str_int[class_elem] = n_class++;
        }
    }
    // Gt for centers
    vector<int> classes_ranks_int;
    for (const auto & class_elem : gt_classes_rank) {
        classes_ranks_int.push_back(class_str_int[class_elem]);
    }

    // Gt for elements of rankings
    unordered_map<int, int> classes_elements_int;

    for (const auto& gt_elem_class_p : gt_elements_class) {

        assert(
                class_str_int.find(gt_elem_class_p.second)
                        != class_str_int.end());
        classes_elements_int[gt_elem_class_p.first] =
                class_str_int[gt_elem_class_p.second];
    }

    init_random();

    vector<int> majority_class;

    cout << "iter, distance, purity, ROC, P@1, P@3, P@5, P@10, P@50" << endl;

    for (int i = 0; i < iterations; i++) {
        double pu = purity(classes_ranks_int, assignment_, n_class, k_,
                &majority_class);
        double precision_at_1 = avg_precision_at_k(classes_elements_int,
                majority_class, centers_, 1);
        double precision_at_3 = avg_precision_at_k(classes_elements_int,
                majority_class, centers_, 3);
        double precision_at_5 = avg_precision_at_k(classes_elements_int,
                majority_class, centers_, 5);
        double precision_at_10 = avg_precision_at_k(classes_elements_int,
                majority_class, centers_, 10);
        double precision_at_50 = avg_precision_at_k(classes_elements_int,
                majority_class, centers_, 50);
        double roc = avg_ROC(classes_elements_int, majority_class, centers_);

        cout << i << "," << tot_distance_ << "," << pu << "," << roc << ","
                << precision_at_1 << "," << precision_at_3 << ","
                << precision_at_5 << "," << precision_at_10 << ","
                << precision_at_50 << endl;
        compute_centers();
        assign_to_centers();
    }

    assert(assignment_.size() == rankings_->size());

    output->clear();
    output->assign(assignment_.begin(), assignment_.end());

    // FOR ANECTODAL EVIDENCE REMOVE
    for (int i= 0; i < k_; i++){
        cout << "CENTER: " << i << " RANKS: ";
        for (int j = 0 ; j < 5; j++ ){
            cout << centers_[i].at(j) << ", ";
        }
        cout << endl;
    }
}
