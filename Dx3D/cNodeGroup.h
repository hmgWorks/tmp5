#pragma once
/*
	��带 ������ ������ ��带 �̿��� �۾��� �Ѵ�.
	��� �׸���
*/
__interface iPathNavigation;
class cNode;

class cNodeGroup
{
	friend class cDijkstra;
public:
	cNodeGroup();
	virtual ~cNodeGroup();
	//position�� ����� ������� ��ġ�̴�.
	void Setup(D3DXVECTOR3& position);
	void Render();

private:	
	std::vector<cNode*>				m_vecNode;
	std::vector<std::vector<float>>	m_vecEdgeCost;
	//���� ��θ� ��� ����
	std::stack<int>					m_stackPath;

	iPathNavigation*	m_pNavigation;
	LPD3DXMESH			m_pNodeMesh;
	D3DMATERIAL9		m_stMtl;
	D3DXMATRIXA16		m_matWorld;
	D3DXVECTOR3			m_vPosition;

	//���۳��� ��ǥ ���, ĳ���Ϳ� ���� �־�����.
	//���ͽ�Ʈ�� ���� �� �� ���� ��θ� ã�´�.
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
	//��带 �̿��ؼ� EdgeCost(�������)�� �����.
	void SetEdgeCost();
	//��� 2���� �Է� �ް�, �ѻ����� ���� ���̸� ��ȯ �Ѵ�.
	float CalcEdgeCost(int n1, int n2);

};

