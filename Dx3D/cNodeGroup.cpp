#include "stdafx.h"
#include "cNodeGroup.h"
#include "cNode.h"
#include "cDijkstra.h"
#include "iPathNavigation.h"

cNodeGroup::cNodeGroup()
	:m_pNavigation(NULL)
	, m_pNodeMesh(NULL)
	, m_vPosition(0, 0, 0)	
	, m_nStNode(0)
	, m_nDestNode(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cNodeGroup::~cNodeGroup()
{
	SAFE_DELETE(m_pNavigation);

	if (!m_vecNode.empty())
	{
		for (auto p : m_vecNode)
		{
			SAFE_DELETE(p);
		}
	}

	SAFE_RELEASE(m_pNodeMesh);
}

void cNodeGroup::Setup()
{
	ZeroMemory(&m_stMtl, sizeof(m_stMtl));
	m_stMtl.Ambient =	D3DXCOLOR(1, 1, 0, 1);
	m_stMtl.Diffuse =	D3DXCOLOR(1, 1, 0, 1);
	m_stMtl.Specular =	D3DXCOLOR(1, 1, 0, 1);
	
	D3DXCreateSphere(g_pD3DDevice, 0.3f, 10, 10, &m_pNodeMesh, NULL);
	
	//위치
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

	m_pNavigation = new cDijkstra;
}

void cNodeGroup::Setup(D3DXVECTOR3& position)
{
	SetPosition(position);
	D3DXMatrixTranslation(&m_matWorld, GetPosition().x, GetPosition().y, GetPosition().z);
	Setup();
	SetEdgeCost();
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
	/*
		노드사이에 엣지가 있으면 값이 노드 사이의 cost(거리 값)
		노드가 없으면 무한 값(큰값)M, 
		자기 스스로이면 0,
		모든 노드 관계에 값을 가진다.

		노드의 위치값 setup후에 호출 돼어야 한다.
	*/
	/*
		ex)
		{ 0, 2, M, M, M, 3, M, M},
		{ 2, 0, 4, 1, M, M, M, M},
		{ M, 4, 0, M, 3, M, M, M},
		{ M, 1, M, 0, 3, M, 2, M},
		{ M, M, 3, 3, 0, M, M, 4},
		{ 3, M, M, M, M, 0, 6, M},
		{ M, M, M, 2, M, 6, 0, 4},
		{ M, M, M, M, 4, M, 4, 0}
	*/
	const float M = 1000.0f;
	m_vecEdgeCost.resize(8, std::vector<float>(8, 0));
	m_vecEdgeCost = {
		{ 0, CalcEdgeCost(0, 1), M, M, M, CalcEdgeCost(0, 5), M, M },
		{ CalcEdgeCost(1, 0), 0, CalcEdgeCost(1, 2), CalcEdgeCost(1, 3), M, M, M, M },
		{ M, CalcEdgeCost(2, 1), 0, M, CalcEdgeCost(2, 4), M, M, M },
		{ M, CalcEdgeCost(3, 1), M, 0, CalcEdgeCost(3, 4), M, CalcEdgeCost(3, 6), M },
		{ M, M, CalcEdgeCost(4, 2), CalcEdgeCost(4, 3), 0, M, M, CalcEdgeCost(4, 7) },
		{ CalcEdgeCost(5, 0), M, M, M, M, 0, CalcEdgeCost(5, 6), M },
		{ M, M, M, CalcEdgeCost(6, 3), M, CalcEdgeCost(6, 5), 0, CalcEdgeCost(6, 7) },
		{ M, M, M, M, CalcEdgeCost(7, 4), M, CalcEdgeCost(7, 6), 0 }
	};
}

float cNodeGroup::CalcEdgeCost(int n1, int n2)
{
	D3DXVECTOR3 v;
	v = m_vecNode[n1]->GetPosition() - m_vecNode[n2]->GetPosition();
	return D3DXVec3Length(&v);
}