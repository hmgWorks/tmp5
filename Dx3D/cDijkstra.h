#pragma once
class cDijkstra
{
public:
	cDijkstra();
	virtual ~cDijkstra();

	void CalcDijkstra(int NodeCount, const std::vector<std::vector<int>> EdgeCost, int StartNode, int DestNode, OUT std::stack<int>& DijkstraValue);
};

