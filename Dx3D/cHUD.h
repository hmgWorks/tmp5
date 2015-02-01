#pragma once
/*
	화면에 정보를 표시 한다.
*/
class cHUD
{
public:
	cHUD();
	virtual ~cHUD();

	void Setup(const D3DXVECTOR3& position);
	void Render(char* output
		, D3DCOLOR fonstColor = D3DCOLOR_XRGB(255, 255, 255)
		, DWORD fontFormat = (DT_LEFT | DT_TOP | DT_NOCLIP));

private:
	D3DXVECTOR3		m_vPosition;
	LPD3DXFONT		m_pFont;
	D3DXFONT_DESC	m_stFontDesc;

public:
	inline void SetPosition(const D3DXVECTOR3& position) { m_vPosition = position; }
	void ChangeFontSize(int width, int height);

};

