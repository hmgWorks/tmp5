#pragma once

class cNode
{
public:
	cNode();
	~cNode();
		
	enum eNodeType
	{
		E_NONE,
		E_EMPTY,
		E_WALL,
		E_START,
		E_DEST,
		E_PATH,
		E_OPEN,
		E_CLOSE,
	};
	
	void Setup(int index, cNode::eNodeType e);
	void Render();
	void ReasetNodeType();

private:
	float			m_fF;//
	float			m_fG;//거리 비용
	float			m_fH;//휴리스틱 값
	int				m_nRow;
	int				m_nCol;

	//그리드 상의 인뎃스
	eNodeType		m_eNodeType;
	cNode*			m_pParent;
	RECT			m_rc;

public:
	std::vector<ST_PNC_VERTEX> m_vecVertex;
	D3DMATERIAL9	m_stMtl;
	D3DXVECTOR3		m_vPosition;


public:
	inline float GetF() { return m_fF; }
	inline void SetF(float f) { m_fF = f; }

	inline float GetG() { return m_fG; }
	inline void SetG(float g) { m_fG = g; }

	inline float GetH() { return m_fH; }
	inline void SetH(float h) { m_fH = h; }
	
	inline int GetRow() { return m_nRow; }
	inline void SetRow(int row) { m_nRow = row; }

	inline int GetCol() { return m_nCol; }
	inline void SetCol(int col) { m_nCol = col; }

	inline eNodeType GetNodeType() { return m_eNodeType; }
	inline void SetNodeType(eNodeType type) { m_eNodeType = type; }

	inline cNode* GetParent() { return m_pParent; }
	inline void SetParent(cNode* parent) { m_pParent = parent; }

	inline D3DXVECTOR3& GetPosition() { return m_vPosition; }
	inline void SetPosition(D3DXVECTOR3& position) { m_vPosition = position; }

};

