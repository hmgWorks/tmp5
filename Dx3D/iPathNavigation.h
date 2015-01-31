#pragma once
class cNodeGroup;
//길찾기 알고리즘의 인터 페이스
__interface iPathNavigation
{
public:
	void CalcPathNavigation(cNodeGroup* nodeGroup);
};