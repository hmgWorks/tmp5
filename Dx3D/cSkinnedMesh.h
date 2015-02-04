#pragma once
/*
	xfile을 로드해서 
*/
#define MOVE_TRANSITION_TIME 0.25f
__interface iNodeMapDelegate;
class cPicking;
class cHUD;
class cBoundingSphere;

class cSkinnedMesh
	: public iPickObj//:/*public iMove, */public iSubject
{
public:
	enum ANI_SET { ATTECK1 = 0, ATTECK2, ATTECK3, RUN, IDLE, MAX};

public:
	cSkinnedMesh(void);
	~cSkinnedMesh(void);
	void Setup(std::string sFolder, std::string sFile);
	void Update();
	void Render();
	
	//void SetDelegate(iNodeMapDelegate* dele);
	void SetDestNode(int n);
	int GetDestNode();
	void SetAnimationIndex(DWORD dwIndex);
	
	inline ANI_SET GetCurrentAni()	{ return m_eCurAni; }
	
	inline D3DXVECTOR3& GetPosition() { return m_vPosition; }
	inline void SetPosition(D3DXVECTOR3& position) { m_vPosition = position; m_vPervPosition = m_vPosition; }
	
	inline D3DXVECTOR3& GetPervPosition() { return m_vPervPosition; }
	inline void SetPervPosition(D3DXVECTOR3& position) { m_vPervPosition = position; }

	inline D3DXVECTOR3& GetDestPosition() { return m_vDestinationPos; }
	inline void SetDestPosition(D3DXVECTOR3& position) { m_vDestinationPos = position; }


private:
	void UpdateWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIXA16* pmatParent);
	void Render(D3DXFRAME* pFrame);
	void SetupBoneMatrixPtrs(D3DXFRAME* pFrame);
	void UpdateSkinnedMesh(D3DXFRAME* pFrame);

	void AniRun();
	void AniIdle();
private:	
	
	D3DXFRAME*					m_pRootFrame;
	LPD3DXANIMATIONCONTROLLER	m_pAnimControl;

	LPD3DXANIMATIONSET	m_pAniSet;
	DOUBLE				m_dLen;
	D3DXTRACK_DESC		m_stTrackDesc;
	DWORD				m_dwCurTrack;

	SYNTHESIZE(ANI_SET, m_eCurAni, CurAni);
	SYNTHESIZE(ANI_SET, m_eNewAni, NewAni);

	float			m_Perv;

	char			m_chDesc[1024];
	char			m_chAtteck[1024];
	
	//HUD 
	cHUD*			m_pHUD;

	//dijkstra 
	D3DXVECTOR3		m_vForward;
	float			m_fAngle;
	float			m_fSpeed;
	D3DXMATRIXA16	m_matWorld;
	//
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vPervPosition;
	D3DXVECTOR3 m_vDestinationPos;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(float, m_fPassedTime, PassedTime);

	//picking
	cBoundingSphere* m_pSphere;
	bool m_bIsPick;
public:	

	//virtual void SetMoveNext(const D3DXVECTOR3& nextNode) override;
	//virtual void SetObserver(iObserver* observer) override;
	//virtual void Notify() override;
	cPicking* m_pPicker;
	virtual bool OnPick() override;
	virtual void OnMove(D3DXVECTOR3& pos) override;
};

