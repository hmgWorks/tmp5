#include "stdafx.h"
#include "cHUD.h"


cHUD::cHUD()
	:m_pFont(NULL)
{	
	ZeroMemory(&m_stFontDesc, sizeof(m_stFontDesc));
	m_stFontDesc.Width = 10;
	m_stFontDesc.Height = 20;
	m_stFontDesc.Weight = FW_MEDIUM;
	m_stFontDesc.Italic = FALSE;
	m_stFontDesc.CharSet = DEFAULT_CHARSET;
	m_stFontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_stFontDesc.PitchAndFamily = FF_DONTCARE;
	strcpy_s(m_stFontDesc.FaceName, "궁서체");//글꼴 스타일
	/*	
		ttf 사용하기
		AddFontResource("umberto.ttf");
		strcpy_s(fd.FaceName, "umberto");//글꼴 스타일
	*/
	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &m_stFontDesc, &m_pFont);
	assert(S_OK == hr && "font error!");
}

cHUD::~cHUD()
{
	SAFE_RELEASE(m_pFont);
}

void cHUD::Setup(const D3DXVECTOR3& position)
{
	SetPosition(position);	
}

void cHUD::Render(char* output, D3DCOLOR fonstColor, DWORD fontFormat)
{
	RECT rc;
	SetRect(&rc, m_vPosition.x, m_vPosition.y, 0, 0);
	m_pFont->DrawTextA(NULL, output, strlen(output), &rc, fontFormat, fonstColor);
}

void cHUD::ChangeFontSize(int width, int height)
{
	m_stFontDesc.Width = width; 
	m_stFontDesc.Height = height; 
	
	SAFE_RELEASE(m_pFont);
	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &m_stFontDesc, &m_pFont);
	assert(S_OK == hr && "font error!");
}