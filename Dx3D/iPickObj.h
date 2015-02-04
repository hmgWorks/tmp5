#pragma once
__interface iPickObj
{
public:
	bool OnPick();
	void OnMove(D3DXVECTOR3& pos);
	void OnMoveAStar();
};

