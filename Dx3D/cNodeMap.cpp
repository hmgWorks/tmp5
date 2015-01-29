#include "stdafx.h"
#include "cNodeMap.h"
#include "cNode.h"

cNodeMap::cNodeMap()
{
}

cNodeMap::~cNodeMap()
{
	if (!m_vecNode.empty())
	{
		for (auto p : m_vecNode)
		{
			SAFE_DELETE(p);
		}
		m_vecNode.clear();
	}
}

void cNodeMap::Setup()
{
	cNode* pNode = new cNode;
	pNode->Setup(1, D3DXVECTOR3(0, 0, 0));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(2, D3DXVECTOR3(2, 0, 2));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(3, D3DXVECTOR3(6, 0, 4));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(4, D3DXVECTOR3(3, 0, 1));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(5, D3DXVECTOR3(9, 0, 3));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(6, D3DXVECTOR3(3, 0, -4));
	m_vecNode.push_back(pNode);
	
	pNode = new cNode;
	pNode->Setup(7, D3DXVECTOR3(9, 0, -3));
	m_vecNode.push_back(pNode);

	pNode = new cNode;
	pNode->Setup(8, D3DXVECTOR3(13, 0, 0));
	m_vecNode.push_back(pNode);

	pNode = NULL;	

	//create edge cost
	int nNodeCount = m_vecNode.size();
	m_vecEdge.resize(nNodeCount);
	for (auto& p : m_vecEdge)
	{
		p.resize(nNodeCount);
	}

	auto VecLen = [](const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)->UINT
	{
		D3DXVECTOR3 v = v1 - v2;
		return D3DXVec3Length(&v);
	};

	m_vecEdge[0][0] = 0;
	m_vecEdge[0][1] =  VecLen(m_vecNode[0]->GetPosition(), m_vecNode[1]->GetPosition());
	m_vecEdge[0][2] = UINT_MAX;
	m_vecEdge[0][3] = UINT_MAX;
	m_vecEdge[0][4] = UINT_MAX;
	m_vecEdge[0][5] = UINT_MAX;
	m_vecEdge[0][6] = VecLen(m_vecNode[0]->GetPosition(), m_vecNode[6]->GetPosition());
	m_vecEdge[0][7] = UINT_MAX;

	m_vecEdge[1][0] = VecLen(m_vecNode[1]->GetPosition(), m_vecNode[0]->GetPosition());
	m_vecEdge[1][1] = 0;
	m_vecEdge[1][2] = VecLen(m_vecNode[1]->GetPosition(), m_vecNode[2]->GetPosition());
	m_vecEdge[1][3] = VecLen(m_vecNode[1]->GetPosition(), m_vecNode[3]->GetPosition());
	m_vecEdge[1][4] = UINT_MAX;
	m_vecEdge[1][5] = UINT_MAX;
	m_vecEdge[1][6] = UINT_MAX;
	m_vecEdge[1][7] = UINT_MAX;

	m_vecEdge[2][0] = UINT_MAX;
	m_vecEdge[2][1] = VecLen(m_vecNode[2]->GetPosition(), m_vecNode[1]->GetPosition());
	m_vecEdge[2][2] = 0;
	m_vecEdge[2][3] = UINT_MAX;
	m_vecEdge[2][4] = VecLen(m_vecNode[2]->GetPosition(), m_vecNode[4]->GetPosition());
	m_vecEdge[2][5] = UINT_MAX;
	m_vecEdge[2][6] = UINT_MAX;
	m_vecEdge[2][7] = UINT_MAX;

	m_vecEdge[3][0] = UINT_MAX;
	m_vecEdge[3][1] = VecLen(m_vecNode[3]->GetPosition(), m_vecNode[1]->GetPosition());
	m_vecEdge[3][2] = UINT_MAX;
	m_vecEdge[3][3] = 0;
	m_vecEdge[3][4] = VecLen(m_vecNode[3]->GetPosition(), m_vecNode[4]->GetPosition());
	m_vecEdge[3][5] = UINT_MAX;
	m_vecEdge[3][6] = VecLen(m_vecNode[3]->GetPosition(), m_vecNode[6]->GetPosition());
	m_vecEdge[3][7] = UINT_MAX;

	m_vecEdge[4][0] = UINT_MAX;
	m_vecEdge[4][1] = UINT_MAX;
	m_vecEdge[4][2] = VecLen(m_vecNode[4]->GetPosition(), m_vecNode[2]->GetPosition());
	m_vecEdge[4][3] = VecLen(m_vecNode[4]->GetPosition(), m_vecNode[3]->GetPosition());
	m_vecEdge[4][4] = 0;
	m_vecEdge[4][5] = UINT_MAX;
	m_vecEdge[4][6] = UINT_MAX;
	m_vecEdge[4][7] = VecLen(m_vecNode[4]->GetPosition(), m_vecNode[7]->GetPosition());

	m_vecEdge[5][0] = VecLen(m_vecNode[5]->GetPosition(), m_vecNode[0]->GetPosition());
	m_vecEdge[5][1] = UINT_MAX;
	m_vecEdge[5][2] = UINT_MAX;
	m_vecEdge[5][3] = UINT_MAX;
	m_vecEdge[5][4] = UINT_MAX;
	m_vecEdge[5][5] = 0;
	m_vecEdge[5][6] = VecLen(m_vecNode[5]->GetPosition(), m_vecNode[6]->GetPosition());
	m_vecEdge[5][7] = UINT_MAX;

	m_vecEdge[6][0] = UINT_MAX;
	m_vecEdge[6][1] = UINT_MAX;
	m_vecEdge[6][2] = UINT_MAX;
	m_vecEdge[6][3] = VecLen(m_vecNode[6]->GetPosition(), m_vecNode[3]->GetPosition());
	m_vecEdge[6][4] = UINT_MAX;
	m_vecEdge[6][5] = VecLen(m_vecNode[6]->GetPosition(), m_vecNode[5]->GetPosition());
	m_vecEdge[6][6] = 0;
	m_vecEdge[6][7] = VecLen(m_vecNode[6]->GetPosition(), m_vecNode[7]->GetPosition());

	m_vecEdge[7][0] = UINT_MAX;
	m_vecEdge[7][1] = UINT_MAX;
	m_vecEdge[7][2] = UINT_MAX;
	m_vecEdge[7][3] = UINT_MAX;
	m_vecEdge[7][4] = VecLen(m_vecNode[7]->GetPosition(), m_vecNode[4]->GetPosition());
	m_vecEdge[7][5] = UINT_MAX;
	m_vecEdge[7][6] = VecLen(m_vecNode[7]->GetPosition(), m_vecNode[6]->GetPosition());
	m_vecEdge[7][7] = 0;

}

void cNodeMap::Render()
{
	for (auto p : m_vecNode)
	{
		p->Render();
	}
}