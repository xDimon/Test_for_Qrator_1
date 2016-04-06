/*
 * TEST_Qrator_1.cpp
 *
 *  Created on: 05.04.2016
 *      Author: Dmitriy Khaustov (khaustov.dm@gmail.com)
 */

#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <math.h>

typedef std::vector< std::vector<bool> > AdjacencyMatrix;
typedef std::vector<unsigned int> VisitsList;
typedef std::pair<unsigned int, unsigned int> Edge;

bool depthFirstSearch(const AdjacencyMatrix &am, VisitsList &visits, unsigned int currentVertex)
{
	visits[currentVertex] = 1;
	for (unsigned int nextVertex = 0; nextVertex < am[currentVertex].size(); ++nextVertex)
	{
		if (am[currentVertex][nextVertex] && !am[nextVertex][currentVertex])
		{
			if (visits[nextVertex] == 0)
			{
				if (depthFirstSearch(am, visits, nextVertex))
				{
					return true;
				}
			}
			else if (visits[nextVertex] == 1)
			{
				// Cycle detected
				return true;
			}
		}
	}
	visits[currentVertex] = 2;

	return false;
}

bool isAcyclic(const AdjacencyMatrix &am)
{
	VisitsList visits;

	visits.assign(am.size(), 0);

	for (unsigned int i = 0; i < visits.size(); ++i)
	{
		if (visits[i])
		{
			continue;
		}
		if (depthFirstSearch(am, visits, i))
		{
			// Graph has a cycle
			return false;
		}
	}

	return true;
}

bool checkCycleByVertex(const AdjacencyMatrix &am, unsigned int beginVertex, unsigned int endVertex)
{
	VisitsList visits(am.size(), 0);
	visits[beginVertex] = 1;

	return depthFirstSearch(am, visits, endVertex);
	return true;
}

bool orientEdges(AdjacencyMatrix &am, std::stack<Edge> &undirectedEdges)
{
	if (undirectedEdges.empty())
	{
		return true;
	}

	Edge edge = undirectedEdges.top();
	undirectedEdges.pop();

	// Try orient from lower to upper
	am[edge.first][edge.second] = true;
	am[edge.second][edge.first] = false;

	if (checkCycleByVertex(am, edge.first, edge.second))
	{
		if (orientEdges(am, undirectedEdges))
		{
			return true;
		}
	}

	// Try orient from upper to lower
	am[edge.first][edge.second] = false;
	am[edge.second][edge.first] = true;

	if (checkCycleByVertex(am, edge.second, edge.first))
	{
		if (orientEdges(am, undirectedEdges))
		{
			return true;
		}
	}

	// Repair prev state
	am[edge.first][edge.second] = true;
	am[edge.second][edge.first] = true;

	undirectedEdges.push(edge);

	return false;
}

bool solve(const AdjacencyMatrix &in, AdjacencyMatrix &out)
{
	// Check for initially cyclic
	if (!isAcyclic(in))
	{
		std::cout << "Solution impossible: graph already has cycle" << std::endl;
		return false;
	}

	out = in;

	// Collect undirected edges
	std::stack<Edge> undirectedEdges;
	for (unsigned int i = 0; i < out.size(); i++)
	{
		for (unsigned int j = i + 1; j < out[i].size(); j++)
		{
			if (out[i][j] && out[j][i])
			{
				undirectedEdges.push(std::make_pair(i, j));
			}
		}
	}

	// Try orient undirected edges
	if (!orientEdges(out, undirectedEdges))
	{
		std::cout << "Solution not found" << std::endl;
		return false;
	}

	return true;
}

void fillMatrix(AdjacencyMatrix &m, const int *ar, unsigned int arSize)
{
	unsigned int n = sqrt(arSize/sizeof(int));

	std::vector<bool> row(n, false);

	m.assign(n, row);

	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			m[i][j] = ar[i*n+j] != 0;
		}
	}
}

void printMatrix(const AdjacencyMatrix &m)
{
	for (unsigned int i = 0; i < m.size(); i++)
	{
		for (unsigned int j = 0; j < m[i].size(); j++)
		{
			std::cout << (m[i][j] ? "1 " : "0 ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, const char *argv[])
{
	int example_cyclic[] = {
		0,0,0,0,1,
		1,0,0,0,0,
		0,1,0,0,0,
		0,0,1,0,0,
		0,0,0,1,0
	};

	int example_withoutSolution[] = {
		0,1,0,1,1,
		0,0,1,0,0,
		0,0,0,0,1,
		0,0,1,0,0,
		1,0,1,0,0
	};

	int example_withSolution[] = {
		0,0,0,0,1,
		1,0,0,0,0,
		0,1,0,0,0,
		0,0,1,0,0,
		1,0,0,1,0
	};

	AdjacencyMatrix in;
	AdjacencyMatrix out;

	std::cout << "===================================== Initially cyclic graph ======================================" << std::endl;

	fillMatrix(in, example_cyclic, sizeof(example_cyclic));

	std::cout << "Source matrix:" << std::endl;
	printMatrix(in);

	if (solve(in, out))
	{
		std::cout << "Solution found:" << std::endl;
		printMatrix(out);
	}
	std::cout << std::endl;


	std::cout << "===================================== Graph without solution ======================================" << std::endl;

	fillMatrix(in, example_withoutSolution, sizeof(example_withoutSolution));

	std::cout << "Source matrix:" << std::endl;
	printMatrix(in);

	if (solve(in, out))
	{
		std::cout << "Solution found:" << std::endl;
		printMatrix(out);
	}
	std::cout << std::endl;



	std::cout << "======================================= Graph with solution =======================================" << std::endl;

	fillMatrix(in, example_withSolution, sizeof(example_withSolution));

	std::cout << "Source matrix:" << std::endl;
	printMatrix(in);

	if (solve(in, out))
	{
		std::cout << "Solution found:" << std::endl;
		printMatrix(out);
	}
	std::cout << std::endl;

	std::cout << "===================================================================================================" << std::endl;
	std::cout << "Done" << std::endl;
	return 0;
}
