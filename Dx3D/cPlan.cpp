#include "stdafx.h"
#include "cPlan.h"
#include "cPicking.h"

cPlan::cPlan()
	:m_pPicker(NULL)
{
}

cPlan::~cPlan()
{
}

void cPlan::Setup()
{
	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(0, 0, 100);
	v.p = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(10.0f, 0.0f, -10.0f);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(10.0f, 0.0f, -10.0f);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
	m_vecVertex.push_back(v);
}

void cPlan::Render()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PC_VERTEX));
}

bool cPlan::OnPick()
{
	return m_pPicker->RayPlanIntersectionTest(this);
}

void cPlan::OnMove(D3DXVECTOR3& pos)
{

}