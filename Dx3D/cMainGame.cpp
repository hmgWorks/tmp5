#include "StdAfx.h"
#include "cMainGame.h"
#include "cGrid.h"
#include "cCamera.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "iMap.h"
#include "cMtlTex.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cHeightMap.h"
#include "cCubeMan.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMesh.h"
#include "cNodeMap.h"
#include "cNodeGroup.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pFont(NULL)
	, m_pMap(NULL)
	, m_pAseRoot(NULL)
	, m_pCubeMan(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pNodeMap(NULL)
	, m_index(0)
	, m_pNodeGroup(NULL)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pNodeGroup);
	SAFE_DELETE(m_pNodeMap);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pSkinnedMesh);

	if(m_pAseRoot)
		m_pAseRoot->Destroy();

	SAFE_DELETE(m_pCubeMan);

	g_pTextureManager->Destroy();
	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cMainGame::Setup()
{
	m_pNodeGroup = new cNodeGroup;
	m_pNodeGroup->Setup(D3DXVECTOR3(5, 0, 5));
	//m_pNodeMap = new cNodeMap;
	//m_pNodeMap->Setup();
	
	m_pSkinnedMesh = new cSkinnedMesh;
	m_pSkinnedMesh->Setup(std::string("Zealot/"), std::string("zealot.X"));
	//m_pSkinnedMesh->SetStNode(1);
	//m_pSkinnedMesh->SetDestNode(8);	
	//m_pSkinnedMesh->SetPosition(m_pNodeMap->GetNode(0));
	//m_pSkinnedMesh->SetDelegate(m_pNodeMap);	
	
	//m_pSkinnedMesh->SetDestinationPos(m_pNodeMap->GetNextNode(1));
	//m_pCubeMan = new cCubeMan;
	//m_pCubeMan->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30, 1);

	//cHeightMap* pMap =new cHeightMap;
	//pMap->Load("HeightMapData/HeightMap.raw", "HeightMapData/terrain.jpg");
	//m_pMap = pMap;

	//std::string sFolder(RESOURCE_FOLDER);
	//sFolder += std::string("ase/woman/");

	//cAseLoader AseLoader;
	//m_pAseRoot = AseLoader.Load(sFolder, std::string("woman_01_all_stand.ASE"));
	//m_pAseRoot = AseLoader.Load(sFolder, std::string("woman_01_all.ASE"));
	//D3DXCreateSphere(g_pD3DDevice, 3, 100, 100, &m_pMesh, NULL);
	//D3DXCreateBox(g_pD3DDevice, 3, 3, 3, &m_pMesh, NULL);
	//D3DXCreateTeapot(g_pD3DDevice, &m_pMesh, NULL);

	//D3DXIntersectTri(점1, 점2, 점3, 광선의 시작점, 광선의 방향, u, v, 광선의 시작점으로부터 방향으로의 거리비);
	
	m_pCamera = new cCamera;
	m_pCamera->Setup();
	//m_pCamera->SetTarget(&m_pCubeMan->GetPosition());

	//폰트 생성
	D3DXFONT_DESC fd;
	ZeroMemory(&fd,sizeof(D3DXFONT_DESC));
	fd.Height			= 50;
	fd.Width			= 25;
	fd.Weight			= FW_MEDIUM;
	fd.Italic			= false;
	fd.CharSet			= DEFAULT_CHARSET;
	fd.OutputPrecision  = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily   = FF_DONTCARE;
	//strcpy_s(fd.FaceName, "궁서체");//글꼴 스타일
	// ttf 사용하기
	AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "umberto");//글꼴 스타일

	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	assert(S_OK == hr);

	

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir = D3DXVECTOR3(1.5, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}


void cMainGame::Update()
{
	g_pTimeManager->Update();

	if(m_pCubeMan)
		m_pCubeMan->Update(m_pMap);
	


	if (m_pSkinnedMesh)
	{
		if (g_pInputManager->GetKeyDownOnce(VK_RETURN))
		{			
			m_pSkinnedMesh->SetDestNode(m_index);
			m_pSkinnedMesh->SetCurAni(cSkinnedMesh::ANI_SET::RUN);
		}
		m_pSkinnedMesh->Update();
	}

	if(m_pCamera)
		m_pCamera->Update();

	int nKey = (int)(GetTickCount() * 4.8f) % (3200 - 640) + 640;
	//int nKey = (GetTickCount() % 3200)* 4.8f;
	if(m_pAseRoot)
	{
		m_pAseRoot->Update(NULL, nKey);
	}
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(33, 40, 48),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);
	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pGrid->Render();
	
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	if (m_pNodeGroup)
		m_pNodeGroup->Render();

	if (m_pNodeMap)
		m_pNodeMap->Render();	

	if(m_pSkinnedMesh)
		m_pSkinnedMesh->Render();
	//m_pCubeMan->Render();

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	if(m_pMap)
		m_pMap->Render();

	
	if(m_pAseRoot)
		m_pAseRoot->Render();

	
	
 	/*RECT rc;
 	SetRect(&rc, 100, 100, 101, 101);
 	char szTemp[1024];
 	sprintf(szTemp, "ani no: %d", m_pSkinnedMesh->GetCurrentAni());
 	m_pFont->DrawTextA(NULL, szTemp, strlen(szTemp),
 		&rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255,0,0));*/


	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	char ch;
	switch (message)
	{
	case WM_CHAR:
		ch = wParam;		
		m_index = (int)ch - 48;
		break;
	case WM_RBUTTONDOWN:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);
			//m_pCircle1->SetCenter(D3DXVECTOR2(nX, nY));
			//static int n;
			//m_pSkinnedMesh->SetAnimationIndex(n++);
		}
		
		break;
	case WM_MOUSEWHEEL:
		GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}
