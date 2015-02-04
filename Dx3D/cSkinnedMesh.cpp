#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"
#include "cNodeMap.h"
#include "cHUD.h"
#include "cBoundingSphere.h"
#include "cPicking.h"
#include "iMap.h"

cSkinnedMesh::cSkinnedMesh(void)
	: m_pRootFrame(NULL)
	, m_pAniSet(NULL)
	, m_eCurAni(ANI_SET::IDLE)
	, m_eNewAni(ANI_SET::IDLE)
	, m_dwCurTrack(0)
	, m_Perv(0.0f)
	//, m_pFont(NULL)
	, m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vForward(0, 0, -1)
	, m_fAngle(0.0f)
	, m_fSpeed(0.05f)
	, m_fActionTime(1.0f)
	, m_fPassedTime(0.0f)
	, m_pHUD(NULL)
	, m_pSphere(NULL)
	, m_pPicker(NULL)
	, m_bIsPick(FALSE)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_DELETE(m_pSphere);
	//SAFE_RELEASE(m_pAniSet);
	//SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pHUD);
	if(m_pRootFrame)
	{
		cAllocateHierarchy Alloc;
		D3DXFrameDestroy(m_pRootFrame, &Alloc);
	}
}

void cSkinnedMesh::Setup( std::string sFolder, std::string sFile )
{
	SetActionTime(2.0f);
	SetCurAni(ANI_SET::IDLE);
	
	//m_pHUD = new cHUD;
	//m_pHUD->Setup(D3DXVECTOR3(5.0f, 5.0f, 0.0f));
	//m_pHUD->ChangeFontSize(7, 12);
	
	m_pSphere = new cBoundingSphere;
	m_pSphere->Setup(&m_vPosition, 0.6f);

	cAllocateHierarchy Alloc;
	Alloc.SetFolder("Zealot/");
	D3DXLoadMeshHierarchyFromX(
		(sFolder + sFile).c_str(), 
		D3DXMESH_MANAGED, 
		g_pD3DDevice, 
		&Alloc,
		NULL,
		&m_pRootFrame,
		&m_pAnimControl);		
	
	LPD3DXANIMATIONSET pAniSet;
	m_pAnimControl->GetAnimationSet(ANI_SET::IDLE, &pAniSet);
	m_pAnimControl->SetTrackAnimationSet(m_dwCurTrack, pAniSet);
	SAFE_RELEASE(pAniSet);
	
	m_Perv = m_pAnimControl->GetTime();

	SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::Update(iMap* pMap)
{
	//
	if (m_eCurAni == ANI_SET::RUN)
	{
		m_fPassedTime += g_pTimeManager->GetDeltaTime();
		AniRun();
		float t = m_fPassedTime / 0.2f;// m_fActionTime;
		if (t < 1.0f)
		{
			D3DXVec3Lerp(&m_vPosition, &m_vPervPosition, &m_vDestinationPos, t);
			D3DXVECTOR3 v = m_vPosition - m_vDestinationPos;
			if (pMap)
			{
				bool  isLand = false;
				float fHeight = pMap->GetHeight(isLand, &m_vPosition);
				if (true)
				{
					m_vPosition.y = fHeight;
				}
			}
			m_fAngle = atan2(v.x, v.z);
		}
		else
		{
			OnMoveAStar();
		}
	}	

	m_pAnimControl->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);

	D3DXMATRIXA16 matT, matR;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;
	UpdateWorldMatrix(m_pRootFrame, NULL);
	UpdateSkinnedMesh(m_pRootFrame);
}

void cSkinnedMesh::Render()
{
	if (m_bIsPick)
		m_pSphere->Render();

	Render(m_pRootFrame);
}


void cSkinnedMesh::UpdateWorldMatrix( D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent )
{
	ST_BONE* pBone = (ST_BONE*)pFrame;	
	
	if(pmatParent)
	{
		pBone->matWorldTM = pBone->TransformationMatrix * (*pmatParent);
	}
	else
	{
		pBone->matWorldTM = pBone->TransformationMatrix;
	}

	if(pBone->pFrameSibling)
	{
		UpdateWorldMatrix(pBone->pFrameSibling, pmatParent);
	}
	if(pBone->pFrameFirstChild)
	{
		UpdateWorldMatrix(pBone->pFrameFirstChild, &pBone->matWorldTM);
	}
}

void cSkinnedMesh::Render( D3DXFRAME* pFrame )
{
	//head up display
	//char szTemp[1024];
	//sprintf(szTemp, "Ani No:%d,\n desc: %s", GetCurrentAni(), m_chDesc);
	
	//m_pHUD->Render(szTemp);

	ST_BONE* pBone = (ST_BONE*)pFrame;
	D3DXMATRIXA16 matW;
	matW = pBone->matWorldTM * m_matWorld;
	 //m_matWorld
	//D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	//matWorld = pBone->matWorldTM * matT;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matW);
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);

	if(pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		for (size_t i = 0; i < pBoneMesh->dwNumSubset; ++i)
		{
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecMtlTex[i]->pTex);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMtlTex[i]->stMtl);
			pBoneMesh->MeshData.pMesh->DrawSubset(i);
		}
	}
	if(pBone->pFrameSibling)
	{
		Render(pBone->pFrameSibling);
	}
	
	if(pBone->pFrameFirstChild)
	{
		Render(pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::SetAnimationIndex(DWORD dwIndex)
{
	DWORD dwNewTrack = (m_dwCurTrack == 0) ? 1 : 0;

	LPD3DXANIMATIONSET pNewAni;
	
	m_pAnimControl->UnkeyAllTrackEvents(m_dwCurTrack);
	m_pAnimControl->UnkeyAllTrackEvents(dwNewTrack);
	
	m_pAnimControl->GetAnimationSet(m_eNewAni, &pNewAni);
	m_pAnimControl->SetTrackAnimationSet(dwNewTrack, pNewAni);

	m_pAnimControl->KeyTrackEnable(m_dwCurTrack, FALSE, m_pAnimControl->GetTime() + MOVE_TRANSITION_TIME);
	m_pAnimControl->KeyTrackSpeed(m_dwCurTrack, 0.0f, m_pAnimControl->GetTime(), MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimControl->KeyTrackWeight(m_dwCurTrack, 0.0f, m_pAnimControl->GetTime(), MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);

	m_pAnimControl->SetTrackEnable(dwNewTrack, TRUE);
	m_pAnimControl->KeyTrackSpeed(dwNewTrack, 1.0f, m_pAnimControl->GetTime(), MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	m_pAnimControl->KeyTrackWeight(dwNewTrack, 1.0f, m_pAnimControl->GetTime(), MOVE_TRANSITION_TIME, D3DXTRANSITION_LINEAR);
	
	
	m_pAnimControl->SetTrackPosition(dwNewTrack, 0);
	m_dwCurTrack = dwNewTrack;
	m_eCurAni = m_eNewAni;
//	SAFE_RELEASE(pCurAni);
	SAFE_RELEASE(pNewAni);	
}

void cSkinnedMesh::SetupBoneMatrixPtrs( D3DXFRAME* pFrame )
{
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.

	if(pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRootFrame, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &pBone->matWorldTM;
			}
		}
	}
	
	ST_BONE* pBone = (ST_BONE*)pFrame;
	//재귀적으로 모든 프레임에 대해서 실행.
	if(pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pBone->pFrameSibling);
	}

	if(pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::UpdateSkinnedMesh( D3DXFRAME* pFrame )
{

	if(pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				// pCurrentBoneMatrices 를 계산하시오
				// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 

				pBoneMesh->pCurrentBoneMatrices[i] = 
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&src );
			pBoneMesh->MeshData.pMesh->LockVertexBuffer( 0, (void**)&dest );

			//MeshData.pMesh을 업데이트 시켜준다.
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest );

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	ST_BONE* pBone = (ST_BONE*)pFrame;
	//재귀적으로 모든 프레임에 대해서 실행.
	if(pBone->pFrameSibling)
	{
		UpdateSkinnedMesh(pBone->pFrameSibling);
	}

	if(pBone->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pBone->pFrameFirstChild);
	}
}

bool cSkinnedMesh::OnPick()
{	
	return m_pPicker->RaySphereIntersectionTest(this);	
}

void cSkinnedMesh::OnMove(D3DXVECTOR3& pos)
{
	m_eCurAni = ANI_SET::RUN;
	m_vPervPosition = m_vPosition;
	SetDestPosition(pos);
	m_fPassedTime = 0.0f;
}

void cSkinnedMesh::OnMoveAStar()
{
	if (!m_vecDestList.empty())
	{
		m_eCurAni = ANI_SET::RUN;
		m_vPervPosition = m_vPosition;	
		SetDestPosition(m_vecDestList.front());
		m_vecDestList.pop_front();
		m_fPassedTime = 0.0f;
	}
	else
	{
		m_eCurAni = ANI_SET::IDLE;
		AniIdle();
		m_fPassedTime = 0.0f;
	}
}

void cSkinnedMesh::AniRun()
{
	LPD3DXANIMATIONSET pAs;
	m_pAnimControl->GetAnimationSet(ANI_SET::RUN, &pAs);
	m_pAnimControl->SetTrackEnable(0, TRUE);
	m_pAnimControl->SetTrackAnimationSet(0, pAs);
	//m_pAnimControl->ResetTime();
	
}

void cSkinnedMesh::AniIdle()
{
	LPD3DXANIMATIONSET pAs;
	m_pAnimControl->GetAnimationSet(ANI_SET::IDLE, &pAs);
	m_pAnimControl->SetTrackEnable(0, TRUE);
	m_pAnimControl->SetTrackAnimationSet(0, pAs);
	//m_pAnimControl->ResetTime();
	
}