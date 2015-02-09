#pragma once
class cOutputName
{
public:
	cOutputName();
	virtual ~cOutputName();

	void Setup(std::string text);
	void Render(D3DXMATRIXA16& matWorld);

private:
	LPD3DXMESH	m_pMeshOutputName;
	std::string m_strName;

	
};

