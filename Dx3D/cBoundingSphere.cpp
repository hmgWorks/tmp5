#include "stdafx.h"
#include "cBoundingSphere.h"


cBoundingSphere::cBoundingSphere()
	:m_vCenter(NULL)
	, m_fRadius(0.0f)
	, m_pMeshSphere(NULL)
{
}

cBoundingSphere::~cBoundingSphere()
{
	SAFE_RELEASE(m_pMeshSphere);
}

void cBoundingSphere::Setup(D3DXVECTOR3* center, float radius)
{
	SetCenter(center);
	SetRadius(radius);
	D3DXCreateSphere(g_pD3DDevice, GetRadius(), 100, 100, &m_pMeshSphere, NULL);
}

void cBoundingSphere::Render()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixTranslation(&mat, GetCenter().x, GetCenter().y+0.6f, GetCenter().z);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	m_pMeshSphere->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}