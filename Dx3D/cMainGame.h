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
class cNodeMap;
class cNodeGroup;

class cMainGame : public cObject
{
private:
	cNodeMap*				m_pNodeMap;
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	iMap*					m_pMap;
	cFrame*					m_pAseRoot;
	LPD3DXFONT				m_pFont;
	cCubeMan*				m_pCubeMan;
	cSkinnedMesh*			m_pSkinnedMesh;
	int m_index;
	cNodeGroup*				m_pNodeGroup;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

