#pragma once
class CellComponent;

class Graph
{
public:
	Graph() = default;
	~Graph() = default;

	void CreateGraphFromGrid(const int& cols, const int& rows, const std::vector<std::shared_ptr<CellComponent>>& pCells);
	void FindPath(const int& start, const int& end);
	int GetNextNode();
private:
	std::vector<std::vector<int>> m_Graph;
	std::vector<int> m_Result;
};

