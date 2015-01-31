#pragma once
/*
	노드를 가지고 있으며 노드를 이용한 작업을 한다.
	노드 그리기
*/
__interface iPathNavigation;
class cNode;

class cNodeGroup
{
	friend class cDijkstra;
public:
	cNodeGroup();
	virtual ~cNodeGroup();
	//position은 노드의 월드상의 위치이다.
	void Setup(D3DXVECTOR3& position);
	void Render();

private:	
	std::vector<cNode*>				m_vecNode;
	std::vector<std::vector<float>>	m_vecEdgeCost;
	//계산된 경로를 담는 스택
	std::stack<int>					m_stackPath;

	iPathNavigation*	m_pNavigation;
	LPD3DXMESH			m_pNodeMesh;
	D3DMATERIAL9		m_stMtl;
	D3DXMATRIXA16		m_matWorld;
	D3DXVECTOR3			m_vPosition;

	//시작노드와 목표 노드, 캐릭터에 의해 주어진다.
	//다익스트라 에서 이 값 으로 경로를 찾는다.
	int					m_nStNode;
	int					m_nDestNode;

public:
	inline D3DXVECTOR3& const GetPosition() { return m_vPosition; }
	inline void SetPosition(D3DXVECTOR3& const position) { m_vPosition = position; }

	inline int GetStNode() { return m_nStNode; }
	inline void SetStNode(int startNode) { m_nStNode = startNode; }

	inline int GetDestNode() { return m_nDestNode; }
	inline void SetDestNode(int destNode) { m_nDestNode = destNode; }

private:
	void Setup();
	//노드를 이용해서 EdgeCost(인접행렬)을 만든다.
	void SetEdgeCost();
	//노드 2개를 입력 받고, 둘사이의 백터 길이를 반환 한다.
	float CalcEdgeCost(int n1, int n2);

};

