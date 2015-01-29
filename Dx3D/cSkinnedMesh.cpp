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
	, m_eNewAni(ANI_SET::IDLE)
	, m_ePervAni(ANI_SET::IDLE)
	, m_bState(FALSE)
	, m_dwCurTrack(0)
	, m_Perv(0.0f)
	, m_pFont(NULL)
	, m_bTrackPos(0)
{
}


cSkinnedMesh::~cSkinnedMesh(void)
{
	//SAFE_RELEASE(m_pAniSet);
	SAFE_RELEASE(m_pFont);
	if(m_pRootFrame)
	{
		cAllocateHierarchy Alloc;
		D3DXFrameDestroy(m_pRootFrame, &Alloc);
	}
}

void cSkinnedMesh::Setup( std::string sFolder, std::string sFile )
{

	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 20;
	fd.Width = 10;
	fd.Weight = FW_MEDIUM;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//strcpy_s(fd.FaceName, "�ü�ü");//�۲� ��Ÿ��
	// ttf ����ϱ�
	AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "umberto");//�۲� ��Ÿ��

	HRESULT hr = D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
	assert(S_OK == hr);

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


void cSkinnedMesh::AniAttack_1()
{
	
}

void cSkinnedMesh::AniRun()
{
	
	/*m_pAnimControl->SetTrackPriority(0, D3DXPRIORITY_HIGH);
	m_pAnimControl->SetTrackPriority(1, D3DXPRIORITY_HIGH);*/

}

void cSkinnedMesh::Update()
{
	
	if (GetKeyState('1') & 0x8000)
	{
		m_eNewAni = ANI_SET::ATTECK1;
		//SetAnimationIndex(0);		
	}
	if (GetKeyState('2') & 0x8000)
	{
		m_eNewAni = ANI_SET::ATTECK2;
		//SetAnimationIndex(0);
	}
	if (GetKeyState('3') & 0x8000)
	{
		m_eNewAni = ANI_SET::ATTECK3;
		//SetAnimationIndex(0);
	}
	if (GetKeyState('W') & 0x8000)
	{
		m_eNewAni = ANI_SET::RUN;
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_eNewAni = ANI_SET::IDLE;
	}
	if (m_eCurAni != m_eNewAni)
	{
		SetAnimationIndex(0);
	}
	
	LPD3DXANIMATIONSET pAs;
	m_pAnimControl->GetAnimationSet(m_dwCurTrack, &pAs);
	D3DXTRACK_DESC de;
	m_pAnimControl->GetTrackDesc(m_dwCurTrack, &de);
	//m_bTrackPos = de.Position;
	//memset(m_chAtteck, 0, 1024);
	memset(m_chDesc, 0, 1024);
	sprintf(m_chDesc, "%d, %.2f, %.2f, %.2f", de.Enable, de.Speed,de.Weight, de.Position);

	if (de.Position > pAs->GetPeriod()+MOVE_TRANSITION_TIME)
	{
		m_eNewAni = ANI_SET::IDLE;
		SetAnimationIndex(0);
	}
	char* atteck;
	double aa = pAs->GetPeriodicPosition(m_pAnimControl->GetTime());/*
	if (((pAs->GetPeriod() + MOVE_TRANSITION_TIME) /2 <=de.Position
		&& (pAs->GetPeriod() + MOVE_TRANSITION_TIME)/2 >= de.Position)
		|| ((pAs->GetPeriod() + MOVE_TRANSITION_TIME)/2 <=de.Position
		&& (pAs->GetPeriod() + MOVE_TRANSITION_TIME) /2 >=de.Position)*/
	if (pAs->GetPeriodicPosition(m_pAnimControl->GetTime()) > (pAs->GetPeriod()/*+MOVE_TRANSITION_TIME*/)/2
		&& m_eCurAni <= ANI_SET::ATTECK3)
	{
		atteck = "atteck!";
		//m_pAnimControl->ResetTime();
	}
	else
	{
		atteck = "...";
	}

	sprintf(m_chDesc, "%d, %.2f, %.2f, %.2f, %s\n%f", de.Enable, de.Speed, de.Weight, de.Position, atteck, aa);

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
	RECT rc;
	SetRect(&rc, 10, 10, 101, 101);
	char szTemp[1024];
	sprintf(szTemp, "Ani No:%d,\n desc: %s", GetCurrentAni(), m_chDesc);
	
	m_pFont->DrawTextA(NULL, szTemp, strlen(szTemp),
		&rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 0));
	
	

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
	
	//m_pAnimControl->KeyTrackEnable(m_dwCurTrack, FALSE, m_pAnimControl->GetTime() + pNewAni->GetPeriod());
	//m_pAnimControl->KeyTrackSpeed(m_dwCurTrack, 0.0f, m_pAnimControl->GetTime(), pNewAni->GetPeriod(), D3DXTRANSITION_LINEAR);
	//m_pAnimControl->KeyTrackWeight(m_dwCurTrack, 0.0f, m_pAnimControl->GetTime(), pNewAni->GetPeriod(), D3DXTRANSITION_LINEAR);
	//            
	//m_pAnimControl->SetTrackEnable(dwNewTrack, TRUE);
	//m_pAnimControl->KeyTrackSpeed(dwNewTrack, 1.0f, m_pAnimControl->GetTime(), pNewAni->GetPeriod(), D3DXTRANSITION_LINEAR);
	//m_pAnimControl->KeyTrackWeight(dwNewTrack, 1.0f, m_pAnimControl->GetTime(), pNewAni->GetPeriod(), D3DXTRANSITION_LINEAR);

	m_pAnimControl->SetTrackPosition(dwNewTrack, 0);
	m_dwCurTrack = dwNewTrack;
	m_eCurAni = m_eNewAni;
//	SAFE_RELEASE(pCurAni);
	SAFE_RELEASE(pNewAni);
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
	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.

	if(pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if(pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
			// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
			// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
			// ã�Ƽ� �����Ʈ������ �ɾ����.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRootFrame, pSkinInfo->GetBoneName(i));
				pBoneMesh->ppBoneMatrixPtrs[i] = &pBone->matWorldTM;
			}
		}
	}
	
	ST_BONE* pBone = (ST_BONE*)pFrame;
	//��������� ��� �����ӿ� ���ؼ� ����.
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
				// pCurrentBoneMatrices �� ����Ͻÿ�
				// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 

				pBoneMesh->pCurrentBoneMatrices[i] = 
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			BYTE* src = NULL;
			BYTE* dest = NULL;

			pBoneMesh->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&src );
			pBoneMesh->MeshData.pMesh->LockVertexBuffer( 0, (void**)&dest );

			//MeshData.pMesh�� ������Ʈ �����ش�.
			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pCurrentBoneMatrices, NULL, src, dest );

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}

	ST_BONE* pBone = (ST_BONE*)pFrame;
	//��������� ��� �����ӿ� ���ؼ� ����.
	if(pBone->pFrameSibling)
	{
		UpdateSkinnedMesh(pBone->pFrameSibling);
	}

	if(pBone->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pBone->pFrameFirstChild);
	}
}
