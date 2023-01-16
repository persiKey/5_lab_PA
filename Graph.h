#pragma once
struct TaskGraph
{
	int** adj_matrix;
	const size_t _size;
	TaskGraph(int size);
	~TaskGraph();
	void PrintMatrix();
	void PrintMatrixToFile();
};

