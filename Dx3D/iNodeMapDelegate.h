class cSkinnedMesh;

__interface iNodeMapDelegate
{
public:
	void OnActionStart(cSkinnedMesh* mesh);
	void OnActionFinish(cSkinnedMesh* mesh);
	void OnNewWay(int st, int dest);
};