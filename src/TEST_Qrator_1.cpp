/*
 * TEST_Qrator_1.cpp
 *
 *  Created on: 05.04.2016
 *      Author: Dmitriy Khaustov (khaustov.dm@gmail.com)
 */

#include <stddef.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

typedef std::vector<std::vector<bool>> AdjacencyMatrix;
typedef std::vector<unsigned int> VisitsList;
typedef std::stack<unsigned int> Path;

int calls = 0;
int recursion = 0;
int maxRecursion = 0;
clock_t start;

bool depthFirstSearch(const AdjacencyMatrix &am, VisitsList &colors, int currentVertex = 0, Path *path = NULL)
{
	++calls;
	maxRecursion = std::max(maxRecursion, ++recursion);

	if (path)
	{
		path->push(currentVertex);
	}
	colors[currentVertex] = 1;
	for (unsigned int nextVertex = 0; nextVertex < am[currentVertex].size(); ++nextVertex)
	{
		if (am[currentVertex][nextVertex] && !am[nextVertex][currentVertex])
		{
			if (colors[nextVertex] == 0)
			{
				if (depthFirstSearch(am, colors, nextVertex, path))
				{
					--recursion;
					return true;
				}
			}
			else if (colors[nextVertex] == 1)
			{
				if (path)
				{
					std::cout << "Found cycle: " << nextVertex;

					while (!path->empty())
					{
						std::cout << " <- " << path->top();
						if (path->top() == nextVertex)
						{
							path->pop();
							break;
						}
						path->pop();
					}

					std::cout << std::endl;
				}

				--recursion;
				return true;
			}
		}
	}
	colors[currentVertex] = 2;
	if (path)
	{
		path->pop();
	}

	--recursion;
	return false;
}

bool isAcyclic(const AdjacencyMatrix &am)
{
	++calls;
	maxRecursion = std::max(maxRecursion, ++recursion);

	VisitsList colors;
	Path path;

	colors.assign(am.size(), false);

	for (size_t i = 0; i < colors.size(); ++i)
	{
		if (colors[i])
		{
			continue;
		}
		if (depthFirstSearch(am, colors, i))
//		if (depthFirstSearch(am, colors, i, &path))
		{
//			std::cout << "Graph has a cycle" << std::endl;

			--recursion;
			return false;
		}
	}

	--recursion;
	return true;
}

bool orientEdges(AdjacencyMatrix &am, std::stack<std::pair<unsigned int, unsigned int>> &undirectedEdges)
{
	++calls;
	maxRecursion = std::max(maxRecursion, ++recursion);

	if (undirectedEdges.empty())
	{
		--recursion;
		return isAcyclic(am);
	}

	std::pair<unsigned int, unsigned int> edge = undirectedEdges.top();
	undirectedEdges.pop();

	int c = random() % 2;

	if (c)
	{
		am[edge.first][edge.second] = true;
		am[edge.second][edge.first] = false;

		if (!isAcyclic(am))
		{
			--recursion;
			return false;
		}
		if (orientEdges(am, undirectedEdges))
		{
			--recursion;
			return true;
		}
	}

	am[edge.first][edge.second] = false;
	am[edge.second][edge.first] = true;
	if (!isAcyclic(am))
	{
		--recursion;
		return false;
	}
	if (orientEdges(am, undirectedEdges))
	{
		--recursion;
		return true;
	}

	if (!c)
	{
		am[edge.first][edge.second] = true;
		am[edge.second][edge.first] = false;
		if (!isAcyclic(am))
		{
			--recursion;
			return false;
		}
		if (orientEdges(am, undirectedEdges))
		{
			--recursion;
			return true;
		}
	}

	am[edge.first][edge.second] = true;
	am[edge.second][edge.first] = true;
	undirectedEdges.push(edge);

	--recursion;
	return false;
}


bool solve(const AdjacencyMatrix &in, AdjacencyMatrix &out)
{
	++calls;
	maxRecursion = std::max(maxRecursion, ++recursion);

	if (!isAcyclic(in))
	{
//		std::cout << "Solution impossible: graph already has cycle" << std::endl;
		--recursion;
		return false;
	}

	// Collect undirected edges
	std::stack<std::pair<unsigned int, unsigned int>> undirectedEdges;
	for (size_t i = 0; i < out.size(); i++)
	{
		for (size_t j = i + 1; j < out[i].size(); j++)
		{
			if (in[i][j] && in[j][i])
			{
				undirectedEdges.push(std::make_pair(i, j));
			}
		}
	}

	out = in;

	start = clock();
	// Try orient undirected edges
	if (!orientEdges(out, undirectedEdges))
	{
		std::cout << ((float)(clock() - start)/CLOCKS_PER_SEC) << ": Solution not found (" << calls << "/" << maxRecursion << ")"<< std::endl;
		--recursion;
		return false;
	}

	std::cout << ((float)(clock() - start)/CLOCKS_PER_SEC) << ": ";
	--recursion;
	return true;
}

int main1(int argc, const char *argv[])
{
	int R = 2714;
	const size_t N = 100;
//	AdjacencyMatrix in = {
//		{0,1,0,0,0},
//		{1,0,0,1,0},
//		{0,0,0,1,0},
//		{0,0,0,0,1},
//		{0,1,0,1,0}
//	};

	AdjacencyMatrix in;
	AdjacencyMatrix out;

	std::vector<bool> row;

	again:
	srandom(++R);

	row.assign(N, false);
	in.assign(N, row);


	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = i + 1; j < N; j++)
		{
			if (((float)(random() % 1000) / 1000) > 0.04)
			{
				continue;
			}
			else if ((random() % 3) == 0)
			{
				in[j][i] = in[i][j] = 1;
			}
			else if ((random() % 2) > 0)
			{
				in[j][i] = 1;
			}
			else
			{
				in[i][j] = 1;
			}
		}
	}
	for (size_t i = 0; i < N; i++)
	{
		in[i][i] = false;
	}

	row.assign(N, false);
	out.assign(N, row);

	if (!solve(in, out))
	{
		calls = 0;
		recursion = 0;
		maxRecursion = 0;
		goto again;
		return 0;
	}

	std::cout << "Source matrix:" << std::endl;
	for (size_t i = 0; i < in.size(); i++)
	{
		for (size_t j = 0; j < in[i].size(); j++)
		{
			if (j) std::cout << ",";
			std::cout << (in[i][j] ? "1" : "0");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	int edges = 0;
	std::cout << "Solution found:" << std::endl;
	for (size_t i = 0; i < out.size(); i++)
	{
		for (size_t j = 0; j < out[i].size(); j++)
		{
			if (j) std::cout << ",";
			std::cout << (out[i][j] ? "1" : "0");
			if (out[i][j]) edges++;
		}
		std::cout << std::endl;
	}

	std::cout << "edges: " << edges << std::endl;
	std::cout << "calls: " << calls << std::endl;
	std::cout << "maxDepth: " << maxRecursion << std::endl;
	std::cout << "R: " << R << std::endl;

	return 0;
}

int main(int argc, const char *argv[])
{
	int R = 0;
	const size_t N = 100;

	AdjacencyMatrix in;
	AdjacencyMatrix out;

	std::vector<bool> row;
	row.assign(N, false);
	in.assign(N, row);

	again:
	srandom(++R);
	in.assign(N, row);

	int deep = 0;

	again2:

	size_t i = random() % in.size();
	size_t j = random() % in.size();

	if (i == j) goto again2;
	if (in[i][j]) goto again2;

	in[i][j] = 1;
	if (!isAcyclic(in))
	{
		in[i][j] = 0;
		goto again2;
	}

	out.assign(N, row);

	if (!solve(in, out))
	{
		calls = 0;
		recursion = 0;
		maxRecursion = 0;
//		goto again;
		return 0;
	}

	std::cout << "Source matrix:" << std::endl;
	for (size_t i = 0; i < in.size(); i++)
	{
		for (size_t j = 0; j < in[i].size(); j++)
		{
			if (j) std::cout << ",";
			std::cout << (in[i][j] ? "1" : "0");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Solution found:" << std::endl;
	for (size_t i = 0; i < out.size(); i++)
	{
		for (size_t j = 0; j < out[i].size(); j++)
		{
			if (j) std::cout << ",";
			std::cout << (out[i][j] ? "1" : "0");
		}
		std::cout << std::endl;
	}

	std::cout << "calls: " << calls << std::endl;
	std::cout << "maxDepth: " << maxRecursion << std::endl;
	std::cout << "R: " << R << std::endl;

	std::cout << "points: " << ++deep << std::endl;
	goto again2;

	return 0;
}
