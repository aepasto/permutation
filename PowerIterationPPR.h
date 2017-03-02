#ifndef ACL_H
#define ACL_H

#include <vector>
#include <queue>
#include <cstdlib>
#include <unordered_map>

#include "Graph.h"
#include "Ranking.h"

#include <unordered_map>
#include <unordered_set>


class PowerIterationPPR {
private:
	Graph* graph = NULL;

	void powerIterationPPR(int seed, double alpha, int iterations, unordered_map<int, double>* p);

public:
	PowerIterationPPR(Graph* graph);
	virtual ~PowerIterationPPR();
	void run(int seed, double alpha, int iterations,
			unordered_map<int, double>* score);
};

#endif
