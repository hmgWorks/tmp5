#pragma once

class cCube;
class cGrid;
class cCamera;
class cGroup;
class iMap;
class cMtlTex;
class cFrame;
class cCubeMan;
class cSkinnedMesh;

class cMainGame : public cObject
{
private:
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	iMap*					m_pMap;
	cFrame*					m_pAseRoot;
	LPD3DXFONT				m_pFont;
	cCubeMan*				m_pCubeMan;
	cSkinnedMesh*			m_pSkinnedMesh;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

