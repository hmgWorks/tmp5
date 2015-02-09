#include "stdafx.h"
#include "cOutputName.h"


cOutputName::cOutputName()
	:m_pMeshOutputName(nullptr)
{
}


cOutputName::~cOutputName()
{
	SAFE_RELEASE(m_pMeshOutputName);
}

void cOutputName::Setup(std::string text)
{
	m_strName = text;
	//3dtext
	HDC	hdc = CreateCompatibleDC(0);
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 25;
	lf.lfWidth = 12;
	lf.lfWeight = 500;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	
	strcpy(lf.lfFaceName, "Times New Roman");

	HFONT hFont;
	HFONT hFontOld;
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	D3DXCreateText(g_pD3DDevice, hdc, m_strName.c_str(), 0.001f, 0.001f, &m_pMeshOutputName, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);	
}

void cOutputName::Render(D3DXMATRIXA16& matWorld)
{
	D3DXMATRIXA16 matView, matInversView, mat, matS, matT, matT2;
	D3DXMatrixScaling(&matS, 0.3f, 0.3f, 0.3f);	
	D3DXMatrixTranslation(&matT, -0.4f, 1.5f, 0.0f);

	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInversView, 0, &matView);
	mat = matInversView;
	mat._41 = matWorld._41;
	mat._42 = matWorld._42;
	mat._43 = matWorld._43;

	mat = matS * matT * mat;
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mat);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMeshOutputName->DrawSubset(0);
}