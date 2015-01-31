#pragma once
class cNodeGroup;
class cDijkstra
	:public iPathNavigation
{
public:
	cDijkstra();
	virtual ~cDijkstra();

	/*void CalcDijkstra(int NodeCount, const std::vector<std::vector<float>> EdgeCost, int StartNode, int DestNode, OUT std::stack<float>& DijkstraValue);*/
	virtual void CalcPathNavigation(cNodeGroup* nodeGroup) override;

};

