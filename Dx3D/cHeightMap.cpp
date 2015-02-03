#include "StdAfx.h"
#include "cHeightMap.h"
#include "cPicking.h"

cHeightMap::cHeightMap(void)
	: m_pMesh(NULL)
	, m_pTexture(NULL)
	, m_nTileN(0)
	, m_pPicker(NULL)
{
}


cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Load( char* szFilename, char* szTextureFilename, int nTileN /*= 256*/, int nBytesPerColor /*= 1*/ )
{
	m_nTileN = nTileN;

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	m_pTexture = g_pTextureManager->GetTexture(szTextureFilename);

	
	//std::vector<DWORD>			vecIndex;

	m_vecVertex.resize((nTileN + 1) * (nTileN + 1));
	vecIndex.reserve(nTileN * nTileN * 2 * 3);

	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "rb");

	for (int z = 0; z <= nTileN; ++z)
	{
		for (int x = 0; x <= nTileN; ++x)
		{
			unsigned char y = fgetc(fp);
			m_vecVertex[z * (nTileN + 1) + x].p = D3DXVECTOR3(x, y / 10.0f, z);
			m_vecVertex[z * (nTileN + 1) + x].t = D3DXVECTOR2(x / (float)nTileN, z / (float)nTileN);
			m_vecVertex[z * (nTileN + 1) + x].n = D3DXVECTOR3(0, 1, 0);
		}
	}

	for (int z = 1; z < nTileN; ++z)
	{
		for (int x = 1; x < nTileN; ++x)
		{
			DWORD l = z * (nTileN + 1) + x - 1;
			DWORD r = z * (nTileN + 1) + x + 1;
			DWORD d = (z - 1) * (nTileN + 1) + x;
			DWORD u = (z + 1) * (nTileN + 1) + x;
			D3DXVECTOR3 du = m_vecVertex[u].p - m_vecVertex[d].p;
			D3DXVECTOR3 lr = m_vecVertex[r].p - m_vecVertex[l].p;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);
			m_vecVertex[z * (nTileN + 1) + x].n = n;
		}
	}
	fclose(fp);
	// 1-3
	// |\|
	// 0-2
	for (int z = 0; z < nTileN; ++z)
	{
		for (int x = 0; x < nTileN; ++x)
		{
			DWORD _0 = (z + 0) * (nTileN + 1) + x + 0;
			DWORD _1 = (z + 1) * (nTileN + 1) + x + 0;
			DWORD _2 = (z + 0) * (nTileN + 1) + x + 1;
			DWORD _3 = (z + 1) * (nTileN + 1) + x + 1;
			vecIndex.push_back(_0);
			vecIndex.push_back(_1);
			vecIndex.push_back(_2);
			vecIndex.push_back(_3);
			vecIndex.push_back(_2);
			vecIndex.push_back(_1);
		}
	}
	

	HRESULT hr = D3DXCreateMeshFVF(vecIndex.size() / 3,
		m_vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &m_vecVertex[0], m_vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	for (size_t i = 0; i < vecIndex.size() / 3; ++i)
	{
		pA[i] = 0;
	}
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdjBuffer(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdjBuffer[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdjBuffer[0], 0, 0, 0);
}

void cHeightMap::Render()
{
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
}

float cHeightMap::GetHeight( OUT bool& isLand, IN D3DXVECTOR3* pvPosition )
{
	if(	pvPosition->x < 0 ||
		pvPosition->z < 0 ||
		pvPosition->x >= m_nTileN ||
		pvPosition->z >= m_nTileN)
	{
		isLand = false;
		return 0.0f;
	}

	// 1-3
	// |\|
	// 0-2
	assert(pvPosition);

	isLand = true;

	int x = (int)pvPosition->x;
	int z = (int)pvPosition->z;

	DWORD _0 = (z + 0) * (m_nTileN + 1) + x + 0;
	DWORD _1 = (z + 1) * (m_nTileN + 1) + x + 0;
	DWORD _2 = (z + 0) * (m_nTileN + 1) + x + 1;
	DWORD _3 = (z + 1) * (m_nTileN + 1) + x + 1;

	float fDeltaX = pvPosition->x - x;
	float fDeltaZ = pvPosition->z - z;

	if( fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1].p - m_vecVertex[_0].p;
		D3DXVECTOR3 v02 = m_vecVertex[_2].p - m_vecVertex[_0].p;
		D3DXVECTOR3 vDelta = v01 * fDeltaZ + v02 * fDeltaX;
		return m_vecVertex[_0].p.y + vDelta.y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		D3DXVECTOR3 v31 = m_vecVertex[_1].p - m_vecVertex[_3].p;
		D3DXVECTOR3 v32 = m_vecVertex[_2].p - m_vecVertex[_3].p;
		D3DXVECTOR3 vDelta = v31 * fDeltaX + v32 * fDeltaZ;
		return m_vecVertex[_3].p.y + vDelta.y;
	}

	return 0.0f;
}

void cHeightMap::OnPick()
{
	if (m_pPicker)
		m_pPicker->RayPlanIntersectionTest(this);
}

void cHeightMap::OnMove(D3DXVECTOR3& pos)
{

}