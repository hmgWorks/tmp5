#pragma once

class cDijkstra;
__interface iPathNavigation;

class cNodeMap
	:public iNodeMapDelegate
{
private:
	int m_nCurrindex;
	std::vector<D3DXVECTOR3> m_vecNode;
	std::vector<std::vector<int>> m_vecEdge;
	std::stack<int> m_vecDest;
	//
	iSubject* m_iDelegate;
	cDijkstra* m_pDijkstra;
	std::stack<int> m_stcDijkstra;
	
	LPD3DXMESH		m_pMeshSphere;
	D3DMATERIAL9	m_stMtl;
	std::vector<ST_PC_VERTEX> vecLineVertex;
	SYNTHESIZE(int, m_fStNode, StNode);
	SYNTHESIZE(int, m_fDestNode, DestNode);
	
	iPathNavigation* m_pNavigation;

public:
	cNodeMap();
	~cNodeMap();

	void Setup();
	void Update();
	void Render();
	
	D3DXVECTOR3& const GetNode(int n);
	D3DXVECTOR3 GetNextNode(int index)
	{
		return m_vecNode[index];
	}
	void CalcDijkstra(int start, int dest);
	void SetSubject(iSubject* subject);
	
	virtual void OnActionStart(cSkinnedMesh* mesh)override;
	virtual void OnActionFinish(cSkinnedMesh* mesh)override;
	virtual void OnNewWay(int st, int dest) override;
};

