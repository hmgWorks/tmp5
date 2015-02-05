#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
	: m_fF(0)
	, m_fH(0)
	, m_fG(0)
	, m_eNodeType(eNodeType::E_NONE)
	, m_vPosition(0, 0, 0)
	, m_pParent(NULL)
{
}

cNode::~cNode()
{
}

void cNode::Setup(int index, cNode::eNodeType e)
{
	//노드의 그리드 상의 위치와 모양, 노드 타입을 초기와	
	m_nRow = index / TILE_N;
	m_nCol = index % TILE_N;
	m_eNodeType = e;

	m_rc.left = m_nCol * TILE_SIZE;
	m_rc.top = m_nRow * TILE_SIZE;
	m_rc.right = m_nCol * TILE_SIZE + TILE_SIZE;
	m_rc.bottom = m_nRow * TILE_SIZE + TILE_SIZE;
	m_vPosition = D3DXVECTOR3(m_rc.right - 0.5f, 0.0f, m_rc.bottom - 0.5f);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	if (e != cNode::eNodeType::E_WALL)
	{
		m_stMtl.Ambient = D3DXCOLOR(0.0f, 0.4f, 0.0f, 1.0f);
		m_stMtl.Diffuse = D3DXCOLOR(0.0f, 0.4f, 0.0f, 1.0f);
		m_stMtl.Specular = D3DXCOLOR(0.0f, 0.4f, 0.0f, 1.0f);

		ST_PNC_VERTEX v;
		v.c = D3DCOLOR_XRGB(100, 175, 0);
		v.n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
	}
	else
	{
		m_stMtl.Ambient = D3DXCOLOR(0.4f, 0.0f, 0.4f, 1.0f);
		m_stMtl.Diffuse = D3DXCOLOR(0.4f, 0.0f, 0.4f, 1.0f);
		m_stMtl.Specular = D3DXCOLOR(0.4f, 0.0f, 0.4f, 1.0f);

		ST_PNC_VERTEX v;
		v.c = D3DCOLOR_XRGB(175, 0, 0);
		v.n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//top
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		//front
		v.n = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);

		//back
		v.n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		//left
		v.n = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.left, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.left, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		//right
		v.n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(m_rc.right, 0.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.top);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(m_rc.right, 1.0f, m_rc.bottom);
		m_vecVertex.push_back(v);
	}
}

void cNode::Render()
{
//#pragma region ???
//
//	COLORREF BkColor = RGB(255, 255, 255);
//	switch (m_eNodeType)
//	{
//	case E_EMPTY:
//		break;
//	case E_WALL:
//		BkColor = RGB(80, 80, 80);
//		break;
//	case E_START:
//		BkColor = RGB(0, 255, 0);
//		break;
//	case E_DEST:
//		BkColor = RGB(0, 255, 0);
//		break;
//	case E_PATH:
//		BkColor = RGB(0, 255, 0);
//		break;
//	case E_OPEN:
//		BkColor = RGB(255, 255, 0);
//		break;
//	case E_CLOSE:
//		BkColor = RGB(255, 0, 0);
//		break;
//	default:
//		//assert(false);
//		break;
//	}
//
//	HBRUSH hBrush, hOldBrush;
//	hBrush = CreateSolidBrush(BkColor);
//	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//
//	Rectangle(hdc, m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);
//
//	SelectObject(hdc, hOldBrush);
//	DeleteObject(hBrush);
//
//	if (m_fF > 0.0f)
//	{
//		SetBkMode(hdc, TRANSPARENT);
//
//		char szBuf[32];
//
//		sprintf_s(szBuf, "%.1f", m_fF);
//		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_LEFT | DT_TOP);
//
//		sprintf_s(szBuf, "%.1f", m_fG);
//		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
//
//		sprintf_s(szBuf, "%.1f", m_fH);
//		DrawTextA(hdc, szBuf, -1, &m_rc, DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
//
//		if (m_pParent)
//		{
//			POINT ptMyCenter;
//			ptMyCenter.x = (m_rc.left + m_rc.right) / 2;
//			ptMyCenter.y = (m_rc.top + m_rc.bottom) / 2;
//
//			POINT ptParentCenter;
//			ptParentCenter.x = (m_pParent->m_rc.left + m_pParent->m_rc.right) / 2;
//			ptParentCenter.y = (m_pParent->m_rc.top + m_pParent->m_rc.bottom) / 2;
//
//			POINT ptLineEnd;
//			ptLineEnd.x = (ptMyCenter.x + ptParentCenter.x) / 2;
//			ptLineEnd.y = (ptMyCenter.y + ptParentCenter.y) / 2;
//
//			MoveToEx(hdc, ptMyCenter.x, ptMyCenter.y, NULL);
//			LineTo(hdc, ptLineEnd.x, ptLineEnd.y);
//
//			hBrush = CreateSolidBrush(BkColor);
//			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//
//			Ellipse(hdc,
//				ptMyCenter.x - 3,
//				ptMyCenter.y - 3,
//				ptMyCenter.x + 3,
//				ptMyCenter.y + 3);
//
//			SelectObject(hdc, hOldBrush);
//			DeleteObject(hBrush);
//		}
//	}
//
//#pragma endregion
	if (m_eNodeType == cNode::eNodeType::E_PATH)
	{
		for (auto& v : m_vecVertex)
		{
			v.c = D3DCOLOR_XRGB(255, 255, 255);
		}
	}
	if (m_eNodeType == cNode::eNodeType::E_DEST)
	{
		for (auto& v : m_vecVertex)
		{
			v.c = D3DCOLOR_XRGB(0, 0, 255);
		}
	}
	if (m_eNodeType == cNode::eNodeType::E_START)
	{
		for (auto& v : m_vecVertex)
		{
			v.c = D3DCOLOR_XRGB(255, 255, 0);
		}
	}
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PNC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vecVertex.size() / 3, &m_vecVertex[0], sizeof(ST_PNC_VERTEX));

}

void cNode::ReasetNodeType()
{
	if (m_eNodeType != cNode::eNodeType::E_WALL)
	{
		m_pParent = NULL;
		m_eNodeType = cNode::eNodeType::E_NONE;
		for (auto& v : m_vecVertex)
		{
			v.c = D3DCOLOR_XRGB(100, 175, 0);
		}
	}
}