#pragma once
/*
A* �˰��� ����
*/
#include "cNode.h"
#include "iAStarMap.h"

class cPicking;
class cCube;

class cAStar
	:public iAStarMap
{
public:
	cAStar();
	~cAStar();
	enum eDirection
	{
		LEFT = 1 << 0,
		RIGHT = 1 << 1,
		UP = 1 << 2,
		DOWN = 1 << 3
	};

	void Setup();
	void Render();
	void initDirectData();
	void initMapType();
	void FindPath();
	void ReadFile(const char* file);
	cNode* FindNode(cNode::eNodeType type);
	float CalcHeuristic(cNode* node1, cNode* node2);

	cNode* GetMinFOpenList();
	//Ȯ��
	void ExtendNode(cNode* currNode, cNode* destNode);
	cNode* GetAdjNode(cNode* currNode, int direction);
	bool IsInList(cNode* node, std::set<cNode*>& list);
	void MarkNodeType(cNode* pDestNode);

	cPicking* m_pPicker;
	virtual void OnClick() override;

public:
	std::vector<cNode*> m_vecNode;
	std::map<char, cNode::eNodeType> m_mapNodeType;

	//8�� left, right, up, down, �밢 4����
	std::vector<int> m_vecDirection;
	std::map<int, float> m_mapDirLen;

	std::set<cNode*> m_setOpenList;
	std::set<cNode*> m_setcloseList;

	cCube* m_pCube;

	void ReasetNodeType();
};

