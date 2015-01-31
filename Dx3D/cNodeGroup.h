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
	//��带 �̿��ؼ� EdgeCost(�������)�� �����.
	void SetEdgeCost();
	//��� 2���� �Է� �ް�, �ѻ����� ���� ���̸� ��ȯ �Ѵ�.
	float CalcEdgeCost(int row, int col);

};

