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
class cCullingSphere;
class cHUD;

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
	//Frustum Culling
	std::vector<cCullingSphere> m_vecSphere;
	LPD3DXMESH m_pSphere;
	D3DMATERIAL9 m_stMtl;
	cHUD* m_pHUD;
	std::vector<D3DXPLANE> m_vecPlane;
	std::vector<D3DXVECTOR3> m_vecVertex;
	enum PLANE_INDEX{ F1 = 0, F2, F3, B1, B2, B3, L1, L2, L3, R1, R2, R3, T1, T2, T3, BT1, BT2, BT3 };
	enum PLANE{ PLANE_FRONT = 0, PLANE_BACK, PLANE_LEFT, PLANE_RIGHT, PLANE_NEAR, PLANE_FAR };
	bool m_bCulling;

	char m_pFPS[1024];
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();

	void DrawSphere();
	void InitPlane();
	void InitSphere();
	void MakePlane();
	bool IsInFrustom(cCullingSphere sphere);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

