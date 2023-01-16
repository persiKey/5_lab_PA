#include "BeesAlgorithm.h"
#include "main.h"

#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <string>
#include <assert.h>

BeesAlgorithm::BeesAlgorithm(int o_count, int e_count) : onlook_count(o_count), empl_count(e_count)
{

	task = new TaskGraph(VERTEX_AMOUNT);
	srand(time(NULL));
	paths = vector<path>(empl_count);
	paths_mark = vector<path_err>(empl_count);

	for (size_t i = 0; i < empl_count; i++)
	{
		paths_mark[i].first = &paths[i];
		paths_mark[i].second = 0;
	}

}

BeesAlgorithm::~BeesAlgorithm()
{
	delete task;
}



x_value BeesAlgorithm::RouletteWheelChoice(vector<std::pair<x_value, f_value>>& values)
{
	float sum = 0;
	for (auto& v : values)
		sum += v.second;

	float x_choice = rand() / float(RAND_MAX);
	float x = 0;

	for (size_t i = 0; i < values.size(); i++)
	{
		x += values[i].second / sum;

		if (x >= x_choice)
			return values[i].first;
	}
	return values.back().first;
}

void BeesAlgorithm::CalcRouteLenght(path& v)
{
	v.second = 0;
	for (size_t i = 0;i < v.first.size()-1; i++)
	{
		v.second += task->adj_matrix[v.first[i]][v.first[i + 1]];
	}
}

vector<int> BeesAlgorithm::LocalSearch(int start_vert, int end_vert)
{
	vector<int> res = { start_vert };
	vector < std::pair<x_value, f_value> > x_f;
	int cur_vert = start_vert;
	int i = 0;
	do
	{
		x_f.clear();
		for (int i = 0; i < task->_size;++i)
		{
			if (task->adj_matrix[cur_vert][i])
			{
				assert(cur_vert != i);
				x_f.push_back({ i,task->adj_matrix[cur_vert][i] });
			}
		}
		
		cur_vert = RouletteWheelChoice(x_f);
		
		res.push_back(cur_vert);
		++i;
		if (i > FAIL_COUNT)
			return vector<int>({ -1 });
		
		
	} while (cur_vert != end_vert);

	for (size_t i = 0; i < res.size()-1; i++)
	{
		assert(res[i] != res[i + 1]);
	}

	return res;
}


void BeesAlgorithm::Solve(int start_vert, int end_vert)
{
	path init_path;
	do
	{
		init_path.first = LocalSearch(start_vert, end_vert);
	} while (init_path.first[0] == -1);
	CalcRouteLenght(init_path);

	for (size_t i = 0; i < empl_count; i++)
	{
		paths[i] = init_path;
	}
	best_ind = 0;
	vector < std::pair<x_value, f_value> > f_x;

	std::ofstream file(std::to_string(onlook_count) + "_" + std::to_string(empl_count) + ".txt");
	for (size_t c = 0; c < NUMBER_OF_ITERATION; c++)
	{
		std::cout << "Best length = " << paths[best_ind].second << '\n';
		file << c << ", " << paths[best_ind].second << '\n';
		for (int i = 0; i < empl_count; i++)
		{
			int pivot = rand() % paths[i].first.size();
			vector<int> old_part_path =  {paths[i].first.begin(), paths[i].first.begin() + pivot};
			vector<int> new_part_path;
			path new_path;
			new_part_path = LocalSearch(
				paths[i].first[pivot], paths[i].first.back());
			if (new_part_path[0] == -1)
				continue;

			std::move(old_part_path.begin(), old_part_path.end(), std::back_inserter(new_path.first));
			std::move(new_part_path.begin(), new_part_path.end(), std::back_inserter(new_path.first));

			CalcRouteLenght(new_path);
			if (new_path.second < paths[i].second)
				paths[i] = new_path;

			if (paths_mark[i].second > FAIL_SEARCH_COUNT && best_ind != i)
			{
				do
				{
					new_path.first = LocalSearch(start_vert, end_vert);
				} while (new_path.first[0] == -1);
				CalcRouteLenght(new_path);

				paths[i] = new_path;
				paths_mark[i].second = 0;
			}
		}

		best_ind = std::min_element(paths.begin(), paths.end(),
			[](const path &a, const path &b) { return  (a.second < b.second);}) - paths.begin();
		

		f_x.clear();
		for (size_t i = 0; i < empl_count; i++)
		{
			f_x.push_back({ i, float(1)/paths[i].second });
		}

		for (size_t i = 0, pth; i < onlook_count; i++)
		{
			pth = RouletteWheelChoice(f_x);

			int pivot = rand() % paths[pth].first.size();
			vector<int> old_part_path = { paths[pth].first.begin(), paths[pth].first.begin() + pivot };
			vector<int> new_part_path;
			path new_path;
			new_part_path = LocalSearch(
				paths[pth].first[pivot], paths[pth].first.back());
			if (new_part_path[0] == -1)
			{
				paths_mark[pth].second++;
				continue;
			}
				
			std::move(old_part_path.begin(), old_part_path.end(), std::back_inserter(new_path.first));
			std::move(new_part_path.begin(), new_part_path.end(), std::back_inserter(new_path.first));

			CalcRouteLenght(new_path);
			if (new_path.second < paths[pth].second)
			{
				paths[pth] = new_path;
				paths_mark[pth].second = 0;
			}
		}
	}

}

void BeesAlgorithm::PrintBest()
{

		for (auto& v : paths[best_ind].first)
		{
			std::cout << v << '-';
		}
	
}
