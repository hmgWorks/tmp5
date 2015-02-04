#pragma once
/*
	height map �̿�ÿ� ���� ����. �����Ͱ��� ���� �̶� ���� ������ ���� �̿� ���� ����
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

	
	//Ŭ���� ��ũ�� ����Ʈ�� �̿��� �� �����̽� ���� ������ ���
	ST_RAY CalcPickingRay(int x, int y);
	//������ ���� �����̽��� ��ȯ
	void TransformRay();
	//���߾����� �׽�Ʈ
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

