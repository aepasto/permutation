/*
 * GraphScheduler.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: aepasto
 */

#include "GraphUtils.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;

void import_graph(const string& file_name, Graph* graph) {
    graph->clear();
    ifstream file_stream(file_name.c_str(), ios_base::in);
    std::string delimiter = " ";
    string line;

    vector<int> tokens;

    assert(file_stream.is_open());

    while (getline(file_stream, line)) {
        //if (count % 10000000 == 0) {
        //    cout << "Imported: " << count / 1000000 << "M" << endl;
        //    cout.flush();
        //}
        //count++;

        tokens.clear();

        size_t pos = 0;
        std::string token;
        while (pos != line.npos) {
            pos = line.find(delimiter);
            token = line.substr(0, pos);
            int token_int = atoi(token.c_str());
            assert(token_int >= 0);
            tokens.push_back(token_int);
            line.erase(0, pos + delimiter.length());
        }
        if (line.size() == 0) { // last line
            break;
        }

        graph->add_edge(tokens[0], tokens[1]);

    }

    assert(graph->num_edges() > 0);
}

// Input: file with
// Line i : "page id integer" <tab> "category string" <tab> "comma separated list of ids (interger) in order of ranking)"
void import_rankings(const string& file_name, vector<Ranking>* ranks,
    vector<string>* classess_of_ranks) {

    classess_of_ranks->clear();
    ranks->clear();

    ifstream file_stream(file_name.c_str(), ios_base::in);
    std::string delimiter = "\t";
    string line;

    vector<string> tokens;
    assert(file_stream.is_open());

    string line_cp;

    while (getline(file_stream, line)) {
        line_cp = line;
        tokens.clear();
        size_t pos = 0;
        std::string token;
        while (pos != line.npos) {
            pos = line.find(delimiter);
            token = line.substr(0, pos);
            tokens.push_back(token);
            line.erase(0, pos + delimiter.length());
        }

        if (line.size() == 0 || tokens.size() != 3) {
            cerr << "ERR LINE" << line << endl;
            continue;
        }
        int id = atoi(tokens[0].c_str());
        string cat = tokens[1];
        string ranking_as_str = tokens[2];

        assert(id >= 0);
        classess_of_ranks->push_back(cat);

        // Parsing ranking (comma separated)
        vector<int> rank_int;
        pos = 0;
        while (pos != ranking_as_str.npos) {
            pos = ranking_as_str.find(",");
            token = ranking_as_str.substr(0, pos);
            if (token.size() > 0) {
                int elem = atoi(token.c_str());
                rank_int.push_back(elem);
            }
            ranking_as_str.erase(0, pos + delimiter.length());
        }
        assert(rank_int.size() > 0);
        Ranking rank(rank_int);
        ranks->push_back(rank);
    }

    assert(ranks->size() > 0);
}


void import_nodes_to_rank(const string& file_name,
unordered_set<int>* to_rank) {

to_rank->clear();
ifstream file_stream(file_name.c_str(), ios_base::in);
std::string delimiter = "\t";
string line;

vector<string> tokens;
assert(file_stream.is_open());

string line_cp;

while (getline(file_stream, line)) {
    line_cp = line;
    tokens.clear();
    size_t pos = 0;
    std::string token;
    while (pos != line.npos) {
        pos = line.find(delimiter);
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delimiter.length());
    }

    if (line.size() == 0 || tokens.size() != 2) {
        cerr << "ERR LINE" << line << endl;
        continue;
    }
    int id = atoi(tokens[0].c_str());
    assert(id >= 0);
    to_rank->insert(id);
}

assert(to_rank->size() > 0);
}

void import_seed_list(const string& file_name,
vector<pair<int, string> >* vec) {
vec->clear();
ifstream file_stream(file_name.c_str(), ios_base::in);
std::string delimiter = "\t";
string line;

vector<string> tokens;
assert(file_stream.is_open());

string line_cp;

while (getline(file_stream, line)) {
    line_cp = line;
    tokens.clear();
    size_t pos = 0;
    std::string token;
    while (pos != line.npos) {
        pos = line.find(delimiter);
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delimiter.length());
    }

    if (line.size() == 0 || tokens.size() != 3) {
        cerr << "ERR LINE" << line << endl;
        continue;
    }
    int id = atoi(tokens[2].c_str());
    assert(id >= 0);
    vec->push_back(make_pair(id, tokens[1]));
}

assert(vec->size() > 0);
}
