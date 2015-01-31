#pragma once

/*
	경로의 노드,
	position, id
*/

class cNode
{
public:
	cNode();
	cNode(int id, D3DXVECTOR3& const pos);
	~cNode();	


private:	
	int m_nId;
	D3DXVECTOR3 m_vPosition;

public:
	inline int GetId() { return m_nId; }
	inline void SetId(int id) { m_nId = id; }

	inline D3DXVECTOR3& const GetPosition() { return m_vPosition; }
	inline void SetPosition(D3DXVECTOR3& const position) { m_vPosition = position; }
};

