/*
	picking에서 클릭 이밴트시에 호출함
	이 인터 패이스를 상속 받으면 클릭 했을때의 처리를 해야 함
	node의 삼각형을 충돌처리 함. 충돌하면 해당 노드가 도착지로 설정 find path를 호출해서
	경로를 찾는다.
*/
__interface iAStarMap
{
public:
	void OnClick();
};