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
#include "cPicking.h"
#include "cPlan.h"
#include "cAStar.h"
#include "cCullingSphere.h"
#include "cHUD.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pFont(NULL)
	, m_pMap(NULL)
	, m_pAseRoot(NULL)
	, m_pCubeMan(NULL)
	, m_pSkinnedMesh(NULL)
	, m_pSkinnedMesh2(NULL)
	//, m_pNodeMap(NULL)
	, m_index(0)
	//, m_pNodeGroup(NULL)
	, m_pPicker(NULL)
	, m_pPlan(NULL)
	, m_pAStar(NULL)	
	, m_pSphere(NULL)
	, m_pHUD(NULL)
	, m_bCulling(false)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pHUD);
	SAFE_RELEASE(m_pSphere);
	SAFE_DELETE(m_pAStar);
	SAFE_DELETE(m_pPlan);
	SAFE_DELETE(m_pPicker);
	//SAFE_DELETE(m_pNodeGroup);
	//SAFE_DELETE(m_pNodeMap);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pMap);

	SAFE_DELETE(m_pSkinnedMesh);
	SAFE_DELETE(m_pSkinnedMesh2);

	if(m_pAseRoot)
		m_pAseRoot->Destroy();

	SAFE_DELETE(m_pCubeMan);

	g_pTextureManager->Destroy();
	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cMainGame::Setup()
{
	m_pHUD = new cHUD;
	m_pHUD->Setup(D3DXVECTOR3(5, 5, 0));
	m_pHUD->ChangeFontSize(25, 50);
	
	InitSphere();
	InitPlane();
	//MakePlane();

	//m_pPicker = new cPicking;
	
	//m_pAStar = new cAStar;
	//m_pAStar->Setup();
	
	//m_pAStar->m_pPicker = m_pPicker;
	//m_pPicker->m_pAStar = m_pAStar;

	//m_pNodeGroup = new cNodeGroup;
	//m_pNodeGroup->Setup(D3DXVECTOR3(5, 0, 5));
	//m_pNodeMap = new cNodeMap;
	//m_pNodeMap->Setup();
	

	//m_pPlan = new cPlan;
	//m_pPlan->Setup();
	//m_pPlan->m_pPicker = m_pPicker;
	//m_pPlan->m_pPicker->AddObj2(m_pPlan);

	//m_pSkinnedMesh = new cSkinnedMesh;
	//m_pSkinnedMesh->Setup(std::string("Zealot/"), std::string("zealot.X"));
	//m_pSkinnedMesh->SetPosition(D3DXVECTOR3(1.5f, 0.0f, 1.5f));
	//m_pSkinnedMesh->m_pPicker = m_pPicker;
	//m_pSkinnedMesh->m_pPicker->AddObj(m_pSkinnedMesh);

	//m_pSkinnedMesh2 = new cSkinnedMesh;
	//m_pSkinnedMesh2->Setup(std::string("Zealot/"), std::string("zealot.X"));
	//m_pSkinnedMesh2->SetPosition(D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	//m_pSkinnedMesh2->m_pPicker = m_pPicker;
	//m_pSkinnedMesh2->m_pPicker->AddObj(m_pSkinnedMesh2);
	
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
	//pMap->m_pPicker = m_pPicker;
	//pMap->m_pPicker->AddObj2(pMap);
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
	//m_pCamera->SetTarget(&m_pSkinnedMesh->GetPosition());

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

	if (g_pInputManager->GetKeyDownOnce(VK_SPACE))
	{
		m_bCulling = !m_bCulling;
	}

	if(m_pCubeMan)
		m_pCubeMan->Update(m_pMap);
	
	if (m_pSkinnedMesh)
	{
		/*if (g_pInputManager->GetKeyDownOnce(VK_RETURN))
		{			
			m_pSkinnedMesh->SetDestNode(m_index);
			m_pSkinnedMesh->SetCurAni(cSkinnedMesh::ANI_SET::RUN);
		}*/
		m_pSkinnedMesh->Update(m_pMap);
	}
	if (m_pSkinnedMesh2)
	{
		m_pSkinnedMesh2->Update(m_pMap);
	}

	if(m_pCamera)
		m_pCamera->Update();

	int nKey = (int)(GetTickCount() * 4.8f) % (3200 - 640) + 640;
	//int nKey = (GetTickCount() % 3200)* 4.8f;
	if(m_pAseRoot)
	{
		m_pAseRoot->Update(NULL, nKey);
	}


	
	memset(m_pFPS, 0, 1024);
	([this]()
	{
		static DWORD frameCount = 0;
		static float timeElapsed = 0.0f;
		static DWORD frameTotalCount = 0;
		static DWORD lastTime = GetTickCount();//g_pTimeManager->GetDeltaTime();//timeGetTime();

		DWORD curTime = GetTickCount();// g_pTimeManager->GetDeltaTime();//timeGetTime();

		float timeDelta = (curTime - lastTime) *0.001f;

		frameCount++;
		timeElapsed += timeDelta;

		if (timeElapsed >= 1.0f)
		{
			float fps = (float)frameCount / timeElapsed;

			frameTotalCount = frameCount;
			frameCount = 0;
			timeElapsed = 0.0f;
		}
		sprintf(this->m_pFPS, "FPS: %d", frameTotalCount); // 문자열처리는 적당히 해준다
		lastTime = curTime;
	}());
	//UpdateFPS();
	

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

	m_pHUD->Render(m_pFPS);
	DrawSphere();
	//a*
	if (m_pAStar)
		m_pAStar->Render();

	//picking
	//if (m_pPlan)
	//	m_pPlan->Render();
	//
	if(m_pSkinnedMesh)
		m_pSkinnedMesh->Render();
	if (m_pSkinnedMesh2)
		m_pSkinnedMesh2->Render();
	
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

void cMainGame::InitSphere()
{
	//m_vecSphere.resize(SPHERE_COUND);
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < SPHERE_COUND; i++)
	{
		cCullingSphere Sphere;
		Sphere.m_vCenter.x = rand() % 10000 -5000;
		Sphere.m_vCenter.y = rand() % 10000 -5000;
		Sphere.m_vCenter.z = rand() % 10000 -5000;
		Sphere.m_fRdius = SPHERE_RADIUS;
		m_vecSphere.push_back(Sphere);
	}

	D3DXCreateSphere(g_pD3DDevice, SPHERE_RADIUS, 8, 8, &m_pSphere, NULL);
}

void cMainGame::InitPlane()
{
	//front
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, 0.0f));
	//back
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	//left
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	//right
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 0.0f));
	//top
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//bottom
	m_vecVertex.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	m_vecVertex.push_back(D3DXVECTOR3(1.0f, -1.0f, 0.0f));

	/*m_vecVertex.resize(8);
	m_vecVertex[0] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);*/
}

void cMainGame::MakePlane()
{
	//InitPlane();

	D3DXMATRIXA16 matProj, matView, mat, matInvers;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	
	m_vecPlane.resize(6);

	mat = matView * matProj;
	D3DXMatrixInverse(&matInvers, 0, &mat);

	for (auto& virtex : m_vecVertex)
	{
		D3DXVec3TransformCoord(&virtex, &virtex, &matInvers);
	}

	D3DXPlaneFromPoints(&m_vecPlane[PLANE_FRONT], &m_vecVertex[F1], &m_vecVertex[F2], &m_vecVertex[F3]);
	D3DXPlaneFromPoints(&m_vecPlane[PLANE_BACK], &m_vecVertex[B1], &m_vecVertex[B2], &m_vecVertex[B3]);
	D3DXPlaneFromPoints(&m_vecPlane[PLANE_LEFT], &m_vecVertex[L1], &m_vecVertex[L2], &m_vecVertex[L3]);
	D3DXPlaneFromPoints(&m_vecPlane[PLANE_RIGHT], &m_vecVertex[R1], &m_vecVertex[R2], &m_vecVertex[R3]);
	D3DXPlaneFromPoints(&m_vecPlane[PLANE_NEAR], &m_vecVertex[T1], &m_vecVertex[T2], &m_vecVertex[T3]);
	D3DXPlaneFromPoints(&m_vecPlane[PLANE_FAR], &m_vecVertex[BT1], &m_vecVertex[BT2], &m_vecVertex[BT3]);
}

bool cMainGame::IsInFrustom(cCullingSphere sphere)
{
	float fCheck = 0.0f;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_FRONT], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_BACK], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_LEFT], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_RIGHT], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_NEAR], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	fCheck = D3DXPlaneDotCoord(&m_vecPlane[PLANE_FAR], &sphere.m_vCenter);
	if (fCheck > 0.0f/* && fabs(fCheck)  > sphere.m_fRdius*/) return false;

	return true;	
}

void cMainGame::DrawSphere()
{
	static int n = 0;
	static int n2 = 0;

	char nn[1024];
	memset(nn, 0, 1024);


	InitPlane();
	MakePlane();
	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetMaterial(&m_stMtl);

	for (auto sphere : m_vecSphere)
	{
		if (m_bCulling)
		{
			if (IsInFrustom(sphere))
			{
				matW._41 = sphere.m_vCenter.x;
				matW._42 = sphere.m_vCenter.y;
				matW._43 = sphere.m_vCenter.z;
				g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
				m_pSphere->DrawSubset(0);
				
				n++;
			}
		}
		else
		{
			matW._41 = sphere.m_vCenter.x;
			matW._42 = sphere.m_vCenter.y;
			matW._43 = sphere.m_vCenter.z;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
			m_pSphere->DrawSubset(0);

			n2++;
		}
				
	}

	sprintf(nn, "%d, %d\n ", n, n2);
	OutputDebugString(nn);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{	
	if(m_pCamera)
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	if (m_pPicker)
		m_pPicker->WndProc(hWnd, message, wParam, lParam);

	g_pInputManager->WndProc(hWnd, message, wParam, lParam);
	
	switch (message)
	{	
	case WM_CHAR:
		//ch = wParam;		
		//m_index = (int)ch - 48;
		break;
	case WM_LBUTTONDOWN:		

		break;
	case WM_RBUTTONDOWN:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);			
		}
		
		break;
	case WM_MOUSEWHEEL:
		GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	}
}
