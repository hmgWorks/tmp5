#include "StdAfx.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"

cAllocateHierarchy::cAllocateHierarchy(void)
{
}

cAllocateHierarchy::~cAllocateHierarchy(void)
{
}

STDMETHODIMP cAllocateHierarchy::CreateFrame( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
	ST_BONE* pBone = new ST_BONE;
	pBone->Name = NULL;
	if(Name)
	{
		pBone->Name = new CHAR[strlen(Name) + 1];
		strcpy(pBone->Name, Name);
	}
	pBone->pFrameFirstChild = NULL;
	pBone->pFrameSibling = NULL;
	pBone->pMeshContainer = NULL;
	D3DXMatrixIdentity(&pBone->TransformationMatrix);
	D3DXMatrixIdentity(&pBone->matWorldTM);
	*ppNewFrame = pBone;
	
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer( 
	THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		cMtlTex* pMtlTex = new cMtlTex;
		std::string sFile(pMaterials[i].pTextureFilename);
		pMtlTex->pTex = g_pTextureManager->GetTexture((m_sFolder + sFile).c_str());
		pMtlTex->stMtl = pMaterials[i].MatD3D;
		pBoneMesh->vecMtlTex.push_back(pMtlTex);
	}
	
	if(pMeshData && D3DXMESHTYPE_MESH == pMeshData->Type)
	{
		pBoneMesh->MeshData.Type = pMeshData->Type;
		pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
		pMeshData->pMesh->GetAttributeTable(0, &pBoneMesh->dwNumSubset);

		pMeshData->pMesh->AddRef();
	}
	if(pSkinInfo)
	{
		// step 1. pSkinInfo 저장
		pSkinInfo->AddRef();
		pBoneMesh->pSkinInfo = pSkinInfo;

		// step 2. 원본 메쉬 복사
		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&pBoneMesh->pOrigMesh);

		// step 3. pSkinInfo->GetNumBones()를 통해 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
		// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices를 동적할당
		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
		pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];

		// step 4. 동적 할당된 pBoneOffsetMatrices 매트릭스에 값 저장.
		for (DWORD i = 0; i < dwNumBones; ++i)
		{
			pBoneMesh->pBoneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	(*ppNewMeshContainer) = pBoneMesh;
	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame( THIS_ LPD3DXFRAME pFrameToFree )
{
	ST_BONE* pBone = (ST_BONE*)pFrameToFree;
	if(pBone->Name)
	{
		SAFE_DELETE_ARRAY(pBone->Name);
	}
	delete pFrameToFree;

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;
	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);
	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);

	for each(auto p in pBoneMesh->vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	delete pBoneMesh;
	return S_OK;
}
