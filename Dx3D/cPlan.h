#pragma once
__interface iPickObj;
class cPicking;

class cPlan
	:public iPickObj
{
public:
	cPlan();
	virtual ~cPlan();

	std::vector<ST_PC_VERTEX> m_vecVertex;
	void Setup();
	void Render();
	virtual bool OnPick() override;
	virtual void OnMove(D3DXVECTOR3& pos) override;
	//void Update();

	cPicking* m_pPicker;
private:
};

