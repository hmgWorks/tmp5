#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
	:m_pMeshSphere(NULL)
	, m_nNodeId(0)
	, m_bCheckId(FALSE)
	, m_nVia(INFINITY_COST)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cNode::~cNode()
{
	SAFE_RELEASE(m_pMeshSphere);
}

void cNode::Setup(INT id, const D3DXVECTOR3& pos)
{
	D3DXCreateSphere(g_pD3DDevice, 0.1f, 10, 10, &m_pMeshSphere, NULL);
	SetNodeId(id);
	SetPosition(pos);	
	D3DXMatrixTranslation(&m_matWorld, pos.x, pos.y, pos.z);

	ZeroMemory(&m_stMtl,sizeof(m_stMtl));
	m_stMtl.Ambient = D3DXCOLOR(1, 0, 0, 1);
	m_stMtl.Diffuse = D3DXCOLOR(1, 0, 0, 1);
	m_stMtl.Specular = D3DXCOLOR(1, 0, 0, 1);
}

void cNode::Render()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMeshSphere->DrawSubset(0);
}