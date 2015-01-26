#pragma once
class cSkinnedMesh
{
private:
	D3DXFRAME*					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAnimControl;

	LPD3DXANIMATIONSET	m_pAniSet;
	LPD3DXANIMATIONSET	m_pAniSetNext;
	UINT				m_nCurtTrack;
	DOUBLE m_dLen;
	D3DXTRACK_DESC	m_stTrackDesc;
	
	enum ANI_SET{ ATTECK1 = 0, ATTECK2, ATTECK3, RUN, IDLE };
	ANI_SET m_eCurAni;

	BOOL m_bState;
public:
	cSkinnedMesh(void);
	~cSkinnedMesh(void);

	void Setup(std::string sFolder, std::string sFile);
	void Update();
	void Render();
	void SetAnimationIndex(DWORD dwIndex, DWORD dwIndex2);
	void AniAttack_1();

private:
	void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	void Render(D3DXFRAME* pFrame);
	void SetupBoneMatrixPtrs(D3DXFRAME* pFrame);
	void UpdateSkinnedMesh( D3DXFRAME* pFrame );

};

