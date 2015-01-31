#pragma once

__interface iPathNavigation;
class cNode;

class cNodeGroup
{
public:
	cNodeGroup();
	virtual ~cNodeGroup();

	void Setup();
	void Setup(D3DXVECTOR3& position);
	void Render();

private:
	std::vector<cNode*>				m_vecNode;
	std::vector<std::vector<float>>	m_vecEdgeCost;

	iPathNavigation*	m_pNavigation;
	LPD3DXMESH			m_pNodeMesh;
	D3DMATERIAL9		m_stMtl;
	D3DXMATRIXA16		m_matWorld;
	D3DXVECTOR3			m_vPosition;

public:
	inline D3DXVECTOR3& const GetPosition() { return m_vPosition; }
	inline void SetPosition(D3DXVECTOR3& const position) { m_vPosition = position; }

private:
	//노드를 이용해서 EdgeCost(인접행렬)을 만든다.
	void SetEdgeCost();
	//노드 2개를 입력 받고, 둘사이의 백터 길이를 반환 한다.
	float CalcEdgeCost(int row, int col);

};

