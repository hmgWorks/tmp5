#pragma once
class cNode;
/*
	heap를 만들자
	노드가있자.
	노드는 자식을 가질수 있는데 최데 2개
	최상위 노드는 루트노드이며 heap이 가지고 있는 데이터중
	가장 작은 값을 가지고 있다.

	insert(cNode* node);
	update();
	cNode* Pop();루트 노드를 돌려 준다.
		-루트 노드를 반환 하고 heap에서는 삭제

*/
template<typename T>
class cHeap
{
public:
	cHeap():m_nIndex(0){};
	virtual ~cHeap(){};

	std::vector<T*> m_vecHeap;
	int m_nIndex;

	void Insert(T* node)
	{
		++m_nIndex;
		if (m_vecHeap.emplace())
		{
			m_vecHeap.push_back();
		}
		else
		{
			int key = m_nIndex;
			while (true)
			{
				if (m_vecHeap[m_nIndex] < m_vecHeap[(m_nIndex - 1) / 2])
				{

				}
				std::vector<T*> tmp;
			}
		}
	}
};

