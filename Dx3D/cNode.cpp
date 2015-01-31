#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
	: m_nId(0)
	, m_vPosition(0, 0, 0)
{
}

cNode::cNode(int id, D3DXVECTOR3& const pos)
	: m_nId(id)
	, m_vPosition(pos)	
{
}

cNode::~cNode()
{
}