#pragma once
#define MOVE_TRANSITION_TIME 0.25f
__interface iNodeMapDelegate;
class cHUD;
class cSkinnedMesh
	//:/*public iMove, */public iSubject
{
public:
	enum ANI_SET { ATTECK1 = 0, ATTECK2, ATTECK3, RUN, IDLE, MAX};
private:	
	
	D3DXFRAME*					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAnimControl;

	LPD3DXANIMATIONSET	m_pAniSet;
	LPD3DXANIMATIONSET	m_pAniSetNext;
	UINT				m_nCurtTrack;
	DOUBLE				m_dLen;
	D3DXTRACK_DESC		m_stTrackDesc;
	DWORD				m_dwCurTrack;

	SYNTHESIZE(ANI_SET, m_eCurAni, CurAni);
	SYNTHESIZE(ANI_SET, m_eNewAni, NewAni);
	ANI_SET			m_ePervAni;

	float			m_Perv;

	char			m_chDesc[1024];
	char			m_chAtteck[1024];
	DOUBLE			m_bTrackPos;
	//LPD3DXFONT		m_pFont;
	//HUD 
	cHUD*			m_pHUD;

	//dijkstra 
	D3DXVECTOR3		m_vForward;
	float			m_fAngle;
	float			m_fSpeed;
	D3DXMATRIXA16	m_matWorld;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPervPos, PervPos);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDestinationPos, DestinationPos);
	SYNTHESIZE(int, m_nStNode, StNode);
	int m_nDestNode;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(float, m_fPassedTime, PassedTime);
	iNodeMapDelegate*  m_pDelegate;

	SYNTHESIZE(int, m_nCurrNode, CurrNode);

public:
	cSkinnedMesh(void);
	~cSkinnedMesh(void);
	void SetDelegate(iNodeMapDelegate* dele);
	
	void SetDestNode(int n);
	int GetDestNode();
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

public:
	//virtual void SetMoveNext(const D3DXVECTOR3& nextNode) override;
	//virtual void SetObserver(iObserver* observer) override;
	//virtual void Notify() override;

	
};

