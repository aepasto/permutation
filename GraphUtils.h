/*
 * GraphScheduler.h
 *
 *  Created on: Oct 22, 2014
 *      Author: aepasto
 */

#ifndef GRAPHUTILS_H_
#define GRAPHUTILS_H_

#include <cstring>

#include "Graph.h"
#include "Ranking.h"

using namespace std;

void import_graph(const string& file_name, Graph* graph);

// Output vec of pairs of: page_id_int, category_string,

// Input: file with
// Line i : "page name string" <tab> "page category string" <tab> "page id integer"
void import_seed_list(const string& file_name, vector<pair<int,string> >* vec);

// NOT used any more using same format as function before
// Input: file with
// Line i : "page id integer" <tab> "category string --- not used ---"
//void import_nodes_to_rank(const string& file_name, unordered_set<int>* to_rank);


// Input: file with
// Line i : "page id integer" <tab> "category string" <tab> "comma separated list of ids (interger) in order of ranking)"
void import_rankings(const string& file_name, vector<Ranking>* ranks,
    vector<string>* classess_of_ranks);


#endif /* GRAPHUTILS_H_ */
