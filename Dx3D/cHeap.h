#pragma once
class cNode;
/*
	heap�� ������
	��尡����.
	���� �ڽ��� ������ �ִµ� �ֵ� 2��
	�ֻ��� ���� ��Ʈ����̸� heap�� ������ �ִ� ��������
	���� ���� ���� ������ �ִ�.

	insert(cNode* node);
	update();
	cNode* Pop();��Ʈ ��带 ���� �ش�.
		-��Ʈ ��带 ��ȯ �ϰ� heap������ ����

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

