#pragma once
class cBoundingSphere
{
public:
	cBoundingSphere();	
	virtual ~cBoundingSphere();
	void Setup(D3DXVECTOR3* center, float radius);
	void Render();

private:
	D3DXVECTOR3* m_vCenter;
	float m_fRadius;
	LPD3DXMESH m_pMeshSphere;
	bool m_bIsPick;

public:
	inline D3DXVECTOR3& GetCenter() { return (*m_vCenter); }
	inline void SetCenter(D3DXVECTOR3* center) { m_vCenter = center; }

	inline float GetRadius() { return m_fRadius; }
	inline void SetRadius(float radius) { m_fRadius = radius; }

	inline bool GetIsPick() { return m_bIsPick; }
	inline void SetIsPick(bool isPick) { m_bIsPick = isPick; }
};

