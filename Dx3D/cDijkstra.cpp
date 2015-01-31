#include "stdafx.h"
#include "cDijkstra.h"
#include "cNodeMap.h"

cDijkstra::cDijkstra()
{
}

cDijkstra::~cDijkstra()
{
}

void cDijkstra::CalcDijkstra(int NodeCount, const std::vector<std::vector<int>> EdgeCost, int StartNode, int DestNode, OUT std::stack<int>& DijkstraValue)
{
	//isVisited
	std::vector<bool>	isVisted(NodeCount, false);
	std::vector<int>	Via(NodeCount, -1);
	std::vector<int>	Cost(NodeCount, INT_MAX);

	Cost[StartNode - 1] = 0;

	while (true)
	{
		int ViaNode = -1;
		int nMinCost = INT_MAX;

		//������� ã��
		for (int i = 0; i < NodeCount; i++)
		{
			if (isVisted[i])
			{
				continue;
			}
			if (nMinCost > Cost[i])
			{
				ViaNode = i;
				nMinCost = Cost[i];
			}
		}

		if (ViaNode == -1)
		{
			break;
		}

		//��尪 �����ϱ�
		for (int i = 0; i < NodeCount; i++)
		{
			if (isVisted[i])
			{
				continue;
			}
			if (Cost[ViaNode] + EdgeCost[ViaNode][i] < Cost[i])
			{
				Cost[i] = Cost[ViaNode] + EdgeCost[ViaNode][i];
				Via[i] = ViaNode;
			}
		}

		isVisted[ViaNode] = true;
	}

	//���� ���Ϳ� �ִ�. ���� ������ �ε��������� ��������.
	int destVia = DestNode - 1;
	DijkstraValue.push(destVia);
	while (true)
	{
		destVia = Via[destVia];
		if (destVia == -1)
		{
			break;
		}
		DijkstraValue.push(destVia);
	}
}
