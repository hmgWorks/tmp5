#pragma once

class cNode
{
private:
	LPD3DXMESH		m_pMeshSphere;
	D3DMATERIAL9	m_stMtl;
	const UINT INFINITY_COST = UINT_MAX;

	SYNTHESIZE(INT, m_nNodeId, NodeId);
	SYNTHESIZE(BOOL, m_bCheckId, CheckId);
	SYNTHESIZE(UINT, m_nVia, Via);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	D3DXMATRIXA16 m_matWorld;
public:
	cNode();
	~cNode();
	void Setup(INT id, const D3DXVECTOR3& pos);
	void Render();
};

