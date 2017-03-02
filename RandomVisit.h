#ifndef RANDOM_VISIT_H
#define RANDOM_VISIT_H

#include <vector>
#include <queue>
#include <cstdlib>
#include <unordered_map>

#include "Graph.h"
#include "Ranking.h"

#include <unordered_map>
#include <unordered_set>

using namespace std;


class RandomVisit {
private:
	Graph* graph = NULL;


public:
	RandomVisit(Graph* graph);
	virtual ~RandomVisit();
	void run(int seed, double alpha, unordered_map<int, double>* score);
};

#endif
