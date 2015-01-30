__interface iMove;
__interface iObserver
{
public:
	void UpdateSubject(iMove& dest);
	void SetDijkstra(int st, int dest);
};