#pragma once
#include "Graph.h"
#include <vector>
#include <utility>

using std::vector;
using std::pair;

typedef int x_value;
typedef float f_value;
typedef int path_length;

typedef std::pair<vector<int>, path_length> path;
typedef pair<path*, int> path_err;

class BeesAlgorithm
{
private:
	int onlook_count;
	int empl_count;
	TaskGraph* task;
	vector<path_err> paths_mark;
	vector<path> paths;
	int best_ind;
	void CalcRouteLenght(path &v);
	vector<int> LocalSearch(int start_vert, int end_vert );
	x_value RouletteWheelChoice(vector < std::pair<x_value, f_value> > & values);
public:
	BeesAlgorithm(int o_count, int e_count);
	~BeesAlgorithm();
	void Solve(int start_vert, int end_vert);
	void PrintBest();
};

