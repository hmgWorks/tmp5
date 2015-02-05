// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <assert.h>
#include <d3dx9.h>
	

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern HWND g_hWnd;

#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete [] p; p = NULL;}
#define SAFE_ADD_REF(p) if(p){p->AddRef();}

#define SINGLETONE(class_name) private: class_name(void); ~class_name(void); \
public: static class_name* GetInstance() { static class_name instance; return &instance; }

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType Get##funName(void) const { return varName; }\
public: virtual void Set##funName(const varType& var){ varName=var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_ADD_REF(var);\
	SAFE_RELEASE(varName);\
	varName = var;\
	}\
}

// �������̽���
#include "iActionDelegate.h"
#include "iObserver.h"
#include "iSubject.h"
#include "iMove.h"
#include "iNodeMapDelegate.h"
#include "iPathNavigation.h"
#include "iPickObj.h"


#include "cObject.h"
#include "cGameObject.h"
#include "cTimeManager.h"
#include "cObjectManager.h"
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cInputManiger.h"
#include "cDijkstra.h"

#define INFINITYNUM 9000
#define RESOURCE_FOLDER "../../Resources/"
#define WINDOW_WIDTH 1000
#define WINDOW_HIEGHT 768
#define TILE_N 24
#define TILE_SIZE 1

struct ST_RHWC_VERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR	c;
	enum {FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE};
};

struct ST_RHW_VERTEX
{
	D3DXVECTOR4 p;
	//D3DCOLOR	c;
	D3DXVECTOR2 t;

	enum {FVF = D3DFVF_XYZRHW | /*D3DFVF_DIFFUSE*/D3DFVF_TEX1};
};

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2	t;
	enum {FVF = D3DFVF_XYZ | D3DFVF_TEX1};
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2	t;
	enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1};
};

struct ST_PNC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE };
};

struct ST_POS_SAMPLE
{
	int				nKey;
	D3DXVECTOR3		v;
};

struct ST_ROT_SAMPLE
{
	int				nKey;
	D3DXQUATERNION	q;
};

struct ST_RAY
{
	D3DXVECTOR3		origin;
	D3DXVECTOR3		direction;
};