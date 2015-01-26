#pragma once
#define MOVE_TRANSITION_TIME 0.5f

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
	DWORD	m_dwCurTrack;

	enum ANI_SET{ ATTECK1 = 0, ATTECK2, ATTECK3, RUN, IDLE, MAX };
	ANI_SET m_eCurAni;
	ANI_SET m_eNewAni;
	ANI_SET m_ePervAni;

	float m_Perv;
	BOOL m_bState;

	char m_chDesc[1024];
	char m_chAtteck[1024];
	DOUBLE m_bTrackPos;
	LPD3DXFONT				m_pFont;
public:
	cSkinnedMesh(void);
	~cSkinnedMesh(void);

	void Setup(std::string sFolder, std::string sFile);
	void Update();
	void Render();
	void SetAnimationIndex(DWORD dwIndex);

	void AniAttack_1();
	void AniRun();

	ANI_SET GetCurrentAni()
	{
		return m_eCurAni;
	}

private:
	void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	void Render(D3DXFRAME* pFrame);
	void SetupBoneMatrixPtrs(D3DXFRAME* pFrame);
	void UpdateSkinnedMesh( D3DXFRAME* pFrame );

};

