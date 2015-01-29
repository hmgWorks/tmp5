#pragma once
class cNode;

class cNodeMap
{
private:
	std::vector<cNode*> m_vecNode;
	std::vector<std::vector<UINT>>	m_vecEdge;

	
public:
	cNodeMap();
	~cNodeMap();

	void Setup();
	void Render();
};

