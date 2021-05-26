#include "pch.h"
#include "Graph.h"
#include <queue>
#include "CellComponent.h"

void Graph::CreateGraphFromGrid(const int& cols, const int& rows, const std::vector<std::shared_ptr<CellComponent>>& pCells)
{
	if (pCells.size() < 0)
		return;

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int index = c + (r * cols);
			m_Graph.push_back(std::vector<int>());

			int connectionIndex;
			int hexaC;
			if (r % 2 == 0)
				hexaC = c * 2;
			else
				hexaC = c * 2 + 1;

			int hexaToRealC;
			auto hexaConvert = [](int hexa)
			{
				if (hexa % 2 == 0)
					return hexa / 2;
				else
					return (hexa - 1) / 2;
			};

			//top left
			if (hexaC > 0 && r > 0)
			{
				hexaToRealC = hexaConvert(hexaC - 1);

				connectionIndex = hexaToRealC + ((r-1) * cols);
				if (pCells[connectionIndex]->IsCube() || pCells[connectionIndex]->IsDisc())
					m_Graph[index].push_back(connectionIndex);
			}
			//top right
			if (c < (cols - 1) && r > 0)
			{
				hexaToRealC = hexaConvert(hexaC + 1);
				connectionIndex = hexaToRealC + ((r-1) * cols);
				if (pCells[connectionIndex]->IsCube() || pCells[connectionIndex]->IsDisc())
					m_Graph[index].push_back(connectionIndex);
			}
			//bottom right
			if (c < (cols - 1) && r < rows-1)
			{
				hexaToRealC = hexaConvert(hexaC + 1);
				connectionIndex = hexaToRealC + ((r + 1) * cols);
				if (pCells[connectionIndex]->IsCube() || pCells[connectionIndex]->IsDisc())
					m_Graph[index].push_back(connectionIndex);
			}
			//bottom left
			if (hexaC > 0 && r < rows - 1)
			{
				hexaToRealC = hexaConvert(hexaC - 1);
				connectionIndex = hexaToRealC + ((r + 1) * cols);
				if (pCells[connectionIndex]->IsCube() || pCells[connectionIndex]->IsDisc())
					m_Graph[index].push_back(connectionIndex);
			}
		}
	}
}

inline int IsNotVisited(int x, std::vector<int>& path)
{
	size_t size = path.size();
	for (int i = 0; i < size; i++)
	{
		if (path[i] == x)
			return 0;
	}
	return 1;
}

void Graph::FindPath(const int& start, const int& end)
{
	std::queue<std::vector<int>> openList;
	std::vector<int> closedList;
	closedList.push_back(start);
	openList.push(closedList);

	int currentNode;
	while (!openList.empty())
	{
		closedList = openList.front();
		openList.pop();

		currentNode = closedList[closedList.size() - 1];

		if (currentNode == end)
		{
			m_Result = closedList;
			break;
		}

		for (int i = 0; i < m_Graph[currentNode].size(); i++)
		{
			if (IsNotVisited(m_Graph[currentNode][i], closedList))
			{
				std::vector<int> newPath(closedList);
				newPath.push_back(m_Graph[currentNode][i]);
				openList.push(newPath);
			}
		}
	}
}

int Graph::GetNextNode()
{
	if (m_Result.size() > 1)
		return m_Result[1];
	return 0;
}
