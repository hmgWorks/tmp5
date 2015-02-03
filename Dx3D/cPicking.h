#pragma once
class cBoundingSphere;
class cPlan;
__interface iPickObj;
class cSkinnedMesh;

class cPicking
{
public:
	cPicking();
	virtual ~cPicking();

	
	//클릭한 스크린 포인트를 이용해 뷰 스패이스 내에 광선을 계산
	ST_RAY CalcPickingRay(int x, int y);
	//광선을 월드 스페이스로 변환
	void TransformRay();
	//맞추었는지 테스트
	bool RaySphereIntersectionTest(cSkinnedMesh* sphere);
	D3DXVECTOR3 RayPlanIntersectionTest(cPlan* plan);
	
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void AddObj(iPickObj* obj);
	void Remove(iPickObj* obj);
	void Notify();
	void AddPlan(iPickObj* plan);

	cSkinnedMesh* m_pZealot;
private:
	ST_RAY m_stRay;
	bool m_isLButtonDown;
	iPickObj* m_pPlan;
	std::list<iPickObj*> m_listObj;
};

