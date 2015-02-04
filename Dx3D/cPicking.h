#pragma once
/*
	height map 이용시에 보간 문제. 포인터간의 보간 이라 맵의 굴곡을 전혀 이용 하지 못함
*/
__interface iPickObj;

class cBoundingSphere;
class cPlan;
class cSkinnedMesh;
class cAStar;
class cHeightMap;

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
	bool RayPlanIntersectionTest(cPlan* plan);
	bool RayPlanIntersectionTest(cHeightMap* plan);
	bool RayPlanIntersectionTest(cAStar* astar);
	
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void AddObj(iPickObj* obj);
	void AddObj2(iPickObj* obj);
	void Notify();
	void AddPlan(iPickObj* plan);


	cSkinnedMesh* m_pZealot;
private:
	ST_RAY m_stRay;
	bool m_isLButtonDown;
	iPickObj* m_pPlan;
	std::list<iPickObj*> m_listObj;
	std::list<iPickObj*> m_listObj2;

public:
	//astar
	cAStar* m_pAStar;
};

