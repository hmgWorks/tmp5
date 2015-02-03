#include "stdafx.h"
#include "cPicking.h"
#include "cBoundingSphere.h"
#include "iPickObj.h"
#include "cPlan.h"
#include "cSkinnedMesh.h"
#include "cHeightMap.h"

cPicking::cPicking()
	:m_pPlan(NULL)
	, m_pZealot(NULL)
{
}

cPicking::~cPicking()
{

}

ST_RAY cPicking::CalcPickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	px = (((2.0f * x) / vp.Width) - 1.0f) / matProj(0, 0);
	py = (((-2.0f * y) / vp.Height) + 1.0f) / matProj(1, 1);

	ST_RAY stRay;
	stRay.origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	stRay.direction = D3DXVECTOR3(px, py, 1.0f);
	
	return stRay;
}

void cPicking::TransformRay()
{	
	D3DXMATRIXA16 matView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&m_stRay.origin, &m_stRay.origin, &matView);
	D3DXVec3TransformNormal(&m_stRay.direction, &m_stRay.direction, &matView);
		
	D3DXVec3Normalize(&m_stRay.direction, &m_stRay.direction);
}

bool cPicking::RaySphereIntersectionTest(cSkinnedMesh* sphere)
{
	
	D3DXVECTOR3 qq;
	qq = sphere->m_pSphere->GetCenter();
	qq.y += 0.6f;
	D3DXVECTOR3 q = m_stRay.origin - qq;

	float b = 2.0f * D3DXVec3Dot(&m_stRay.direction, &q);
	float c = D3DXVec3Dot(&m_stRay.direction, &m_stRay.direction)*(D3DXVec3Dot(&q, &q) - (sphere->m_pSphere->GetRadius() * sphere->m_pSphere->GetRadius()));

	//판별식을 찾는다.
	float discriminant = (b*b) - (4.0f*c);

	//가상의 수에 대한 테스트
	if (discriminant < 0.0f)
	{
		return false;
	}

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	//해가 >= 0일경우 구체를 교차 하는 것
	if (s0 >= 0.0f || s1 >= 0.0f)
	{		
		if (m_pZealot)
		{
			if (sphere != m_pZealot)
			{
				m_pZealot->m_bIsPick = false;
				m_pZealot = sphere;
				m_pZealot->m_bIsPick = true;
			}		
		}
		else
		{
			m_pZealot = sphere;
			m_pZealot->m_bIsPick = true;
		}
	}
	
}

void cPicking::RayPlanIntersectionTest(cPlan* plan)
{
	float u, v, fDist;
	for (size_t i = 0; i < plan->m_vecVertex.size(); i += 3)
	{
		if (D3DXIntersectTri(&plan->m_vecVertex[i].p, &plan->m_vecVertex[i + 1].p, &plan->m_vecVertex[i + 2].p,
			&m_stRay.origin, &m_stRay.direction, &u, &v, &fDist))
		{	
			D3DXVECTOR3 v = m_stRay.origin + (m_stRay.direction * fDist);			
			if (m_pZealot)
			{
				for (auto p : m_listObj)
				{
					if (p == m_pZealot)
					{
						m_pZealot->OnMove(v);
					}
				}				
			}			
		}		
	}
}

void cPicking::RayPlanIntersectionTest(cHeightMap* plan)
{
	float u, v, fDist;
	for (size_t i = 0; i < plan->vecIndex.size(); i += 3)
	{
		DWORD _0, _1, _2;
		_0 = plan->vecIndex[i];
		_1 = plan->vecIndex[i + 1];
		_2 = plan->vecIndex[i + 2];
			
		if (D3DXIntersectTri(&plan->m_vecVertex[_0].p, &plan->m_vecVertex[_1].p, &plan->m_vecVertex[_2].p,
			&m_stRay.origin, &m_stRay.direction, &u, &v, &fDist))
		{
			D3DXVECTOR3 v = m_stRay.origin + (m_stRay.direction * fDist);
			if (m_pZealot)
			{
				for (auto p : m_listObj)
				{
					if (p == m_pZealot)
					{
						m_pZealot->OnMove(v);
					}
				}
			}
		}
	}
}

void cPicking::AddObj(iPickObj* obj)
{
	m_listObj.push_back(obj);
}

void cPicking::Remove(iPickObj* obj)
{
	if (!m_listObj.empty())
		m_listObj.remove(obj);
}

void cPicking::AddPlan(iPickObj* plan)
{
	m_pPlan = plan;
}

void cPicking::Notify()
{	
	for (auto obj : m_listObj)
	{
		obj->OnPick();
	}
}

void cPicking::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	D3DXMATRIXA16 matView;
	switch (message)
	{	
	case WM_LBUTTONDOWN:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		m_stRay = CalcPickingRay(x, y);
		TransformRay();
		
		Notify();	
	}
	break;
	case WM_LBUTTONUP:
	{
		m_isLButtonDown = false;
	}
	break;
	case WM_RBUTTONDOWN:
		m_pZealot->m_bIsPick = false;
		m_pZealot = nullptr;

	break;
	}
}