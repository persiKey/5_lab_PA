#include <time.h>
#include <random>
#include <iostream>

#include "Graph.h"
#include "BeesAlgorithm.h"

int main()
{

	int onlook;
	int employ;
	std::cout << "Amount of onlookers: "; std::cin >> onlook;
	std::cout << "Amount of employers: "; std::cin >> employ;

	BeesAlgorithm ba(onlook, employ);
	
	ba.Solve(1,299);
	ba.PrintBest();

	return 0;
}
