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
//class cNodeMap;
//class cNodeGroup;
class cPicking;
class cPlan;
class cAStar;

class cMainGame : public cObject
{
private:
	//cNodeMap*				m_pNodeMap;
	//cNodeGroup*				m_pNodeGroup;
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	iMap*					m_pMap;
	cFrame*					m_pAseRoot;
	LPD3DXFONT				m_pFont;
	cCubeMan*				m_pCubeMan;
	cSkinnedMesh*			m_pSkinnedMesh;
	cSkinnedMesh*			m_pSkinnedMesh2;
	int m_index;
	
	//picking
	cPicking*	m_pPicker;
	cPlan*		m_pPlan;

	cAStar*		m_pAStar;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

