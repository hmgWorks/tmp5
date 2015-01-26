#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cSkinnedMesh::cSkinnedMesh(void)
	: m_pRootFrame(NULL)
	, m_pAniSet(NULL)
	, m_pAniSetNext(NULL)
	, m_nCurtTrack(0)
	, m_eCurAni(ANI_SET::IDLE)
	, m_bState(FALSE)
{
}


cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_RELEASE(m_pAniSet);

	if(m_pRootFrame)
	{
		cAllocateHierarchy Alloc;
		D3DXFrameDestroy(m_pRootFrame, &Alloc);
	}
}

void cSkinnedMesh::Setup( std::string sFolder, std::string sFile )
{
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
	
	m_pAnimControl->GetAnimationSet(ANI_SET::IDLE, &m_pAniSet);
	m_pAnimControl->SetTrackAnimationSet(0, m_pAniSet);
	m_dLen = m_pAniSet->GetPeriod();
	
	m_pAnimControl->GetTrackDesc(0, &m_stTrackDesc);
	
	SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::AniAttack_1()
{
	m_pAnimControl->GetAnimationSet(ANI_SET::RUN, &m_pAniSetNext);
	m_pAnimControl->SetTrackAnimationSet(1, m_pAniSetNext);
	m_pAnimControl->SetTrackEnable(1, TRUE);
	m_pAnimControl->SetTrackWeight(1, 0.0f);
	m_bState = TRUE;
}

void cSkinnedMesh::Update()
{
	if (GetKeyState('1') & 0x8000)
	{
		//m_pAnimControl->SetTrackPosition(1, 0);		
		m_pAnimControl->ResetTime();
		m_pAnimControl->SetTrackPosition(1, 0);
		AniAttack_1();		
	}
		//float x = m_pAnimControl->GetPriorityBlend();
	
	if (m_bState)
	{
		
		D3DXTRACK_DESC des;
		m_pAnimControl->GetTrackDesc(0, &des);
		D3DXTRACK_DESC des1;
		m_pAnimControl->GetTrackDesc(1, &des1);

		float tt = des.Position / m_pAniSet->GetPeriod();//0
		float ttt = des1.Position / m_pAniSetNext->GetPeriod();//1
		
		m_pAnimControl->SetTrackWeight(0, 1 - tt);
		m_pAnimControl->SetTrackWeight(1, ttt);

		m_pAnimControl->SetTrackEnable(0, TRUE);
		m_pAnimControl->SetTrackEnable(1, TRUE);

		m_pAnimControl->SetTrackSpeed(0, 1.0f);
		m_pAnimControl->SetTrackSpeed(1, 1.0f);

		m_pAnimControl->SetTrackPriority(0, D3DXPRIORITY_HIGH);
		m_pAnimControl->SetTrackPriority(1, D3DXPRIORITY_HIGH);
		//m_pAnimControl->ResetTime();

		char ch[1024];
		sprintf(ch, "%lf\n", des.Position);
		OutputDebugString(ch);
		static bool bTmp = false;
		if (m_pAniSet->GetPeriod()< des.Position)
		{	
			bTmp = TRUE;			
		}
		if (bTmp)
		{	//m_pAnimControl->ResetTime();
			float tt = des.Position / m_pAniSet->GetPeriod();//0
			float ttt = des1.Position / m_pAniSetNext->GetPeriod();//1
			m_pAnimControl->SetTrackWeight(1, 1 - ttt);
			m_pAnimControl->SetTrackWeight(0, tt);

			if (m_pAniSetNext->GetPeriod()< des1.Position)
			{
				m_pAnimControl->SetTrackPosition(0, 0);
				m_pAnimControl->SetTrackPosition(1, 0);

				m_pAnimControl->SetTrackEnable(1, FALSE);
				m_pAnimControl->ResetTime();
				m_bState = FALSE;
			}

		}
	}
	
	

	m_pAnimControl->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);

	UpdateWorldMatrix(m_pRootFrame, NULL);

	UpdateSkinnedMesh(m_pRootFrame);
}

void cSkinnedMesh::Render()
{
	Render(m_pRootFrame);
}


void cSkinnedMesh::UpdateWorldMatrix( D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent )
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->matWorldTM = pBone->TransformationMatrix;
	
	if(pmatParent)
	{
		pBone->matWorldTM = pBone->matWorldTM * (*pmatParent);
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
	
	ST_BONE* pBone = (ST_BONE*)pFrame;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);

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

void cSkinnedMesh::SetAnimationIndex(DWORD dwIndex, DWORD dwIndex2)
{

	
	/*m_pAnimControl->GetAnimationSet(dwIndex2, &pAnimationSet2);
	
	
	
	D3DXTRACK_DESC desc;
	m_pAnimControl->GetTrackDesc(0, &desc);	

	float tt = (float)desc.Position /(float) ani_1;
	m_pAnimControl->SetTrackAnimationSet(0, pAnimationSet);
	m_pAnimControl->SetTrackAnimationSet(1, pAnimationSet2);	
	
	m_pAnimControl->SetTrackWeight(0, tt);
	m_pAnimControl->SetTrackWeight(1, 1 - tt);

	m_pAnimControl->SetTrackEnable(0, TRUE);
	m_pAnimControl->SetTrackEnable(1, FALSE);
	
	

	m_pAnimControl->SetTrackSpeed(0, 1.0f);
	m_pAnimControl->SetTrackSpeed(1, 1.0f);

	m_pAnimControl->SetTrackPriority(0, D3DXPRIORITY_HIGH);
	m_pAnimControl->SetTrackPriority(1, D3DXPRIORITY_HIGH);
	
	m_pAnimControl->ResetTime();

	SAFE_RELEASE(pAnimationSet);
	SAFE_RELEASE(pAnimationSet2);*/
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
