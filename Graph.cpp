#include "Graph.h"
#include "main.h"

#include <iostream>
#include <random>
#include <time.h>

#include <fstream>

TaskGraph::TaskGraph(int size) : _size(size)
{
	adj_matrix = new int* [_size];
	for (size_t i = 0; i < _size; i++)
	{
		adj_matrix[i] = new int[_size] {0};
	}

	int taken;
	int taken_y;
	int pos_value;
	int bound;
	for (int i = 0; i < _size - 1; i++)
	{
		taken = 0;
		for (int k = 0; k < i; k++)
		{
			taken += adj_matrix[i][k];
		}
		bound = rand() % (VERT_DEGREE_UPPER_BOUND - VERT_DEGREE_LOWER_BOUND) + VERT_DEGREE_LOWER_BOUND - taken;
		for (int k = 0;	k < bound ; k++)
		{
			taken_y = 0;
			pos_value = rand() % (_size - i - 1) + i + 1;

			for (int m = 0; m < i -1; m++)
			{
				taken_y += adj_matrix[pos_value][m];
			}
			if (taken_y >= VERT_DEGREE_UPPER_BOUND)
			{
				--k;
				continue;
			}
			adj_matrix[i][pos_value] = 1;
			adj_matrix[pos_value][i] = 1;
		}
	}

	int value;
	for (int i = 0; i < _size; i++)
	{
		for (int k = 0; k < i; k++)
		{
			if (adj_matrix[i][k])
			{
				value = rand() % (LENGHT_UPPER_BOUND - LENGHT_LOWER_BOUND + 1) + LENGHT_LOWER_BOUND;
				adj_matrix[i][k] = adj_matrix[k][i] = value;
			}
		}
	}

}

TaskGraph::~TaskGraph()
{
	for (size_t i = 0; i < _size; i++)
	{
		delete[] adj_matrix[i];
	}
	delete[] adj_matrix;
}

void TaskGraph::PrintMatrix()
{
	for (size_t i = 0; i < _size; i++)
	{
		for (size_t k = 0; k < _size; k++)
		{
			std::cout << adj_matrix[i][k] << 
				((k < _size - 1) ? ", " : " ");
		}
		std::cout << '\n';
	}
}

void TaskGraph::PrintMatrixToFile()
{
	std::ofstream file("matrix.txt");
	for (size_t i = 0; i < _size; i++)
	{
		for (size_t k = 0; k < _size; k++)
		{
			file << adj_matrix[i][k] <<
				((k < _size - 1) ? ", " : " ");
		}
		file << '\n';
	}
}
