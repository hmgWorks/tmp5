#pragma once

class cMtlTex;

class cFrame
{
	friend class cAseLoader;
private:
	std::string					m_sNodeName;
	std::string					m_sParentName;
	std::vector<cFrame*>		m_vecChildren;
	D3DXMATRIXA16				m_matLocalTM;
	D3DXMATRIXA16				m_matWorldTM;
	LPD3DXMESH					m_pMesh;
	cMtlTex*					m_pMtlTex;
	std::vector<ST_POS_SAMPLE>	m_vecPosTrack;
	std::vector<ST_ROT_SAMPLE>	m_vecRotTrack;
	D3DXMATRIXA16*				m_pmat;
public:
	cFrame(void);
	~cFrame(void);

	void AddChild(cFrame* pChild);
	void CalcLocalTM(D3DXMATRIXA16* pParent);
	void BuidlMesh(std::vector<ST_PNT_VERTEX>& vecVertex);
	void SetMtlTex(cMtlTex* pMtlTex);
	void Update(D3DXMATRIXA16* pmatParent, int nKey);
	void Render();
	void Destroy();

private:
	void LocalTMatrix(OUT D3DXMATRIXA16& mat, IN int nKey);
	void LocalRMatrix(OUT D3DXMATRIXA16& mat, IN int nKey);
};

