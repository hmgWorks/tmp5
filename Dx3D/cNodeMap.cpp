#include "stdafx.h"
#include "cNodeMap.h"
#include "cDijkstra.h"
#include "cSkinnedMesh.h"

cNodeMap::cNodeMap()
	:m_iDelegate(NULL)
	, m_pDijkstra(NULL)
	, m_pMeshSphere(NULL)
	, m_nCurrindex(0)
	, m_fStNode(-1)
	, m_fDestNode(-1)
	, m_pNavigation(NULL)
{
}

cNodeMap::~cNodeMap()
{
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_DELETE(m_pDijkstra);
}

void cNodeMap::Setup()
{
	D3DXCreateSphere(g_pD3DDevice, 0.1f, 10, 10, &m_pMeshSphere, NULL);
	ZeroMemory(&m_stMtl, sizeof(m_stMtl));
	m_stMtl.Ambient = D3DXCOLOR(1, 0, 0, 1);
	m_stMtl.Diffuse = D3DXCOLOR(1, 0, 0, 1);
	m_stMtl.Specular = D3DXCOLOR(1, 0, 0, 1);


	m_vecNode.push_back(D3DXVECTOR3(0, 0, 0	));
	m_vecNode.push_back(D3DXVECTOR3(2, 0, 2	));
	m_vecNode.push_back(D3DXVECTOR3(6, 0, 4	));
	m_vecNode.push_back(D3DXVECTOR3(3, 0, 1	));
	m_vecNode.push_back(D3DXVECTOR3(9, 0, 3	));
	m_vecNode.push_back(D3DXVECTOR3(3, 0, -4));	
	m_vecNode.push_back(D3DXVECTOR3(9, 0, -3));
	m_vecNode.push_back(D3DXVECTOR3(13, 0, 0));

	//create edge cost
	int nNodeCount = m_vecNode.size();
	m_vecEdge.resize(nNodeCount);

	//m_vecDest.resize(nNodeCount);
	for (auto& p : m_vecEdge)
	{
		p.resize(nNodeCount);
	}

	auto VecLen = [](const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)->int
	{
		D3DXVECTOR3 v = v1 - v2;
		return D3DXVec3Length(&v);
	};

	m_vecEdge[0][0] = 0;
	m_vecEdge[0][1] = VecLen(m_vecNode[0], m_vecNode[1]);
	m_vecEdge[0][2] = INFINITYNUM;
	m_vecEdge[0][3] = INFINITYNUM;
	m_vecEdge[0][4] = INFINITYNUM;
	m_vecEdge[0][5] = VecLen(m_vecNode[0], m_vecNode[5]);
	m_vecEdge[0][6] = INFINITYNUM;
	m_vecEdge[0][7] = INFINITYNUM;

	m_vecEdge[1][0] = VecLen(m_vecNode[1], m_vecNode[0]);
	m_vecEdge[1][1] = 0;
	m_vecEdge[1][2] = VecLen(m_vecNode[1], m_vecNode[2]);
	m_vecEdge[1][3] = VecLen(m_vecNode[1], m_vecNode[3]);
	m_vecEdge[1][4] = INFINITYNUM;
	m_vecEdge[1][5] = INFINITYNUM;
	m_vecEdge[1][6] = INFINITYNUM;
	m_vecEdge[1][7] = INFINITYNUM;

	m_vecEdge[2][0] = INFINITYNUM;
	m_vecEdge[2][1] = VecLen(m_vecNode[2], m_vecNode[1]);
	m_vecEdge[2][2] = 0;
	m_vecEdge[2][3] = INFINITYNUM;
	m_vecEdge[2][4] = VecLen(m_vecNode[2], m_vecNode[4]);
	m_vecEdge[2][5] = INFINITYNUM;
	m_vecEdge[2][6] = INFINITYNUM;
	m_vecEdge[2][7] = INFINITYNUM;

	m_vecEdge[3][0] = INFINITYNUM;
	m_vecEdge[3][1] = VecLen(m_vecNode[3], m_vecNode[1]);
	m_vecEdge[3][2] = INFINITYNUM;
	m_vecEdge[3][3] = 0;
	m_vecEdge[3][4] = VecLen(m_vecNode[3], m_vecNode[4]);
	m_vecEdge[3][5] = INFINITYNUM;
	m_vecEdge[3][6] = VecLen(m_vecNode[3], m_vecNode[6]);
	m_vecEdge[3][7] = INFINITYNUM;

	m_vecEdge[4][0] = INFINITYNUM;
	m_vecEdge[4][1] = INFINITYNUM;
	m_vecEdge[4][2] = VecLen(m_vecNode[4], m_vecNode[2]);
	m_vecEdge[4][3] = VecLen(m_vecNode[4], m_vecNode[3]);
	m_vecEdge[4][4] = 0;
	m_vecEdge[4][5] = INFINITYNUM;
	m_vecEdge[4][6] = INFINITYNUM;
	m_vecEdge[4][7] = VecLen(m_vecNode[4], m_vecNode[7]);

	m_vecEdge[5][0] = VecLen(m_vecNode[5], m_vecNode[0]);
	m_vecEdge[5][1] = INFINITYNUM;
	m_vecEdge[5][2] = INFINITYNUM;
	m_vecEdge[5][3] = INFINITYNUM;
	m_vecEdge[5][4] = INFINITYNUM;
	m_vecEdge[5][5] = 0;
	m_vecEdge[5][6] = VecLen(m_vecNode[5], m_vecNode[6]);
	m_vecEdge[5][7] = INFINITYNUM;

	m_vecEdge[6][0] = INFINITYNUM;
	m_vecEdge[6][1] = INFINITYNUM;
	m_vecEdge[6][2] = INFINITYNUM;
	m_vecEdge[6][3] = VecLen(m_vecNode[6], m_vecNode[3]);
	m_vecEdge[6][4] = INFINITYNUM;
	m_vecEdge[6][5] = VecLen(m_vecNode[6], m_vecNode[5]);
	m_vecEdge[6][6] = 0;
	m_vecEdge[6][7] = VecLen(m_vecNode[6], m_vecNode[7]);

	m_vecEdge[7][0] = INFINITYNUM;
	m_vecEdge[7][1] = INFINITYNUM;
	m_vecEdge[7][2] = INFINITYNUM;
	m_vecEdge[7][3] = INFINITYNUM;
	m_vecEdge[7][4] = VecLen(m_vecNode[7], m_vecNode[4]);
	m_vecEdge[7][5] = INFINITYNUM;
	m_vecEdge[7][6] = VecLen(m_vecNode[7], m_vecNode[6]);
	m_vecEdge[7][7] = 0;

	ST_PC_VERTEX v;
	v.c = D3DCOLOR_XRGB(100, 100, 100);
	//1-2
	v.p = m_vecNode[0];
	vecLineVertex.push_back(v);
	v.p = m_vecNode[1];
	vecLineVertex.push_back(v);

	//1-6
	v.p = m_vecNode[0];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[5];
	vecLineVertex.push_back(v);

	//6-7
	v.p = m_vecNode[5];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[6];
	vecLineVertex.push_back(v);

	//2-3
	v.p = m_vecNode[1];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[2];
	vecLineVertex.push_back(v);

	//2-4
	v.p = m_vecNode[1];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[3];
	vecLineVertex.push_back(v);

	//3-5
	v.p = m_vecNode[2];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[4];
	vecLineVertex.push_back(v);


	//4-5
	v.p = m_vecNode[3];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[4];
	vecLineVertex.push_back(v);

	//4-7
	v.p = m_vecNode[3];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[6];
	vecLineVertex.push_back(v);

	//5-8
	v.p = m_vecNode[4];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[7];
	vecLineVertex.push_back(v);

	//7-8
	v.p = m_vecNode[6];
	vecLineVertex.push_back(v);

	v.p = m_vecNode[7];
	vecLineVertex.push_back(v);
}

void cNodeMap::Update()
{
	
}

void cNodeMap::Render()
{
	D3DXMATRIXA16 matT;
	for (auto p : m_vecNode)
	{
		D3DXMatrixTranslation(&matT, p.x, p.y, p.z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetMaterial(&m_stMtl);
		g_pD3DDevice->SetTexture(0, NULL);
		m_pMeshSphere->DrawSubset(0);
	}
	D3DXMatrixIdentity(&matT);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		vecLineVertex.size() / 2,
		&vecLineVertex[0],
		sizeof(ST_PC_VERTEX));

}

//void cNodeMap::SetSubject(iSubject* subject)
//{
//	m_iDelegate = subject;
//	m_iDelegate->SetObserver(this);
//}

//void cNodeMap::UpdateSubject(iMove& dest)
//{
//	//질럿이 하나의 노드의 끝에 오면 실행 됀다.
//	//다음 노드가 있을 경우 여기를 싱행해서 넘겨 준다.
//	//dest.SetMoveNext()
//}
//
//void cNodeMap::SetDijkstra(int st, int dest)
//{
//	if(!m_pDijkstra)
//		m_pDijkstra = new cDijkstra;
//	
//	m_pDijkstra->CalcDijkstra(m_vecNode.size(), m_vecEdge, st, dest, m_vecDest);
//
//	//m_iDelegate->
//
//}

void cNodeMap::OnActionStart(cSkinnedMesh* mesh)
{
	CalcDijkstra(mesh->GetStNode(), mesh->GetDestNode());
}

void cNodeMap::OnActionFinish(cSkinnedMesh* mesh)
{	
	static int n = 0;
	if (!m_vecDest.empty())
	{
		n = m_vecDest.top();
		m_vecDest.pop();
	}
	else
	{
		mesh->SetNewAni(cSkinnedMesh::ANI_SET::IDLE);
	}
	mesh->SetDestinationPos(m_vecNode[n]);
	mesh->SetPervPos(mesh->GetPosition());
	mesh->SetStNode(n);
}

void cNodeMap::CalcDijkstra(int start, int dest)
{
//	m_pDijkstra->CalcDijkstra(m_vecNode.size(), m_vecEdge, start, dest, m_vecDest);
}

void cNodeMap::OnNewWay(int st, int dest)
{
	CalcDijkstra(st, dest);
}

D3DXVECTOR3& const cNodeMap::GetNode(int n)
{
	return m_vecNode[n];
}