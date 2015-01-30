__interface iObserver;
__interface iSubject
{
public:
	void SetObserver(iObserver* observer);	
	void Notify();
};