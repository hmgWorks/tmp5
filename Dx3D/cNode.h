#pragma once

class cNode
{
private:
	SYNTHESIZE(int, m_nId, Id);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cNode();
	cNode(int id, D3DXVECTOR3& const pos);
	~cNode();	
};

