#pragma once
class cBoundingSphere;
class cPlan;
__interface iPickObj;
class cSkinnedMesh;

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
	void RayPlanIntersectionTest(cPlan* plan);
	void RayPlanIntersectionTest(cHeightMap* plan);
	
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

