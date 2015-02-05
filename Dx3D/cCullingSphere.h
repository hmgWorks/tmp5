#pragma once
class cCullingSphere
{
public:
	cCullingSphere();
	virtual ~cCullingSphere();

	D3DXVECTOR3 m_vCenter;
	float m_fRdius;
};

