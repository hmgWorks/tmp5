#pragma once
#include "iMap.h"

class cHeightMap : public iMap
{
private:
	std::vector<ST_PNT_VERTEX>	m_vecVertex;
	int					m_nTileN;
	LPD3DXMESH			m_pMesh;
	LPDIRECT3DTEXTURE9	m_pTexture;
	D3DMATERIAL9		m_stMtl;
public:
	cHeightMap(void);
	~cHeightMap(void);

	void Load(char* szFilename, char* szTextureFilename, int nTileN = 256, int nBytesPerColor = 1);

	//iMap override
	virtual void Render() override;

	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;
};

