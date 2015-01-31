#include "stdafx.h"
#include "cNodeGroup.h"
#include "cNode.h"

cNodeGroup::cNodeGroup()
	:m_pNavigation(NULL)
	, m_pNodeMesh(NULL)
	, m_vPosition(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cNodeGroup::~cNodeGroup()
{
	SAFE_RELEASE(m_pNodeMesh);

	if (!m_vecNode.empty())
	{
		for (auto p : m_vecNode)
		{
			SAFE_DELETE(p);

		}
	}
}

void cNodeGroup::Setup()
{
	ZeroMemory(&m_stMtl, sizeof(m_stMtl));
	m_stMtl.Ambient =	D3DXCOLOR(1, 1, 0, 1);
	m_stMtl.Diffuse =	D3DXCOLOR(1, 1, 0, 1);
	m_stMtl.Specular =	D3DXCOLOR(1, 1, 0, 1);
	
	D3DXCreateSphere(g_pD3DDevice, 0.3f, 10, 10, &m_pNodeMesh, NULL);
	
	//À§Ä¡
	m_vecNode = {
		{ new cNode(1, D3DXVECTOR3(0, 0, 0)	) },
		{ new cNode(2, D3DXVECTOR3(2, 0, 2)	) },
		{ new cNode(3, D3DXVECTOR3(6, 0, 4)	) },
		{ new cNode(4, D3DXVECTOR3(3, 0, 1)	) },
		{ new cNode(5, D3DXVECTOR3(9, 0, 3)	) },
		{ new cNode(6, D3DXVECTOR3(3, 0, -4)) },
		{ new cNode(7, D3DXVECTOR3(9, 0, -3)) },
		{ new cNode(8, D3DXVECTOR3(13, 0, 0)) }
	};
}

void cNodeGroup::Setup(D3DXVECTOR3& position)
{
	SetPosition(position);
	D3DXMatrixTranslation(&m_matWorld, GetPosition().x, GetPosition().y, GetPosition().z);
	Setup();
}

void cNodeGroup::Render()
{
	D3DXMATRIXA16 mat;
	for (auto node : m_vecNode)
	{
		D3DXMatrixTranslation(&mat, node->GetPosition().x, node->GetPosition().y, node->GetPosition().z);
		mat *= m_matWorld;
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pNodeMesh->DrawSubset(0);
	}
}


void cNodeGroup::SetEdgeCost()
{
	m_vecEdgeCost.resize(8, std::vector<float>(8, 0));
	m_vecEdgeCost = {
		{ 0, },
	};
}

float cNodeGroup::CalcEdgeCost(int row, int col)
{
	D3DXVECTOR3 v;
	v = m_vecNode[row]->GetPosition() - m_vecNode[col]->GetPosition();
	return D3DXVec3Length(&v);
}