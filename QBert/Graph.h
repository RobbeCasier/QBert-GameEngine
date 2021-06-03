#pragma once
class CellComponent;

class Graph
{
public:
	Graph() = default;
	~Graph() = default;

	void CreateGraphFromGrid(const int& cols, const int& rows, const std::vector<std::shared_ptr<CellComponent>>& pCells);
	void FindPath(const int& start, const std::vector<int>& end);
	int GetNextNode();
	const int& GetRouteIndex() { return m_RouteIndex; }
private:
	std::vector<std::vector<int>> m_Graph;
	std::vector<int> m_Result;
	int m_RouteIndex = 0;
};

