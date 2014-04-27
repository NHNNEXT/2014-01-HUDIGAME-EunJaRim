#include "stdafx.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// ���� ���� �ʱ�ȭ ����
//////////////////////////////////////////////////////////////////////////
//�θ� Process ���� �ڿ�
LPDIRECT3D9 g_D3D = nullptr;
LPDIRECT3DDEVICE9 g_D3dDevice = nullptr;

//Height Map ������ ���� �ڿ�
LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = nullptr;
LPDIRECT3DINDEXBUFFER9 g_IdxBuffer = nullptr;

LPDIRECT3DTEXTURE9 g_TexHeight = nullptr;
LPDIRECT3DTEXTURE9 g_TexDiffuse = nullptr;

DWORD g_XHeight = 0;
DWORD g_ZHeight = 0;

//�ؽ�Ʈ ��¿� �ʿ��� �ڿ�
ID3DXFont*		g_Font = nullptr;		// ���ڸ� �׸� ��Ʈ ��Ÿ�� ��ü
ID3DXSprite*	g_Sprite = nullptr;		// ��Ʈ�� �׸� ��������Ʈ ��ü 

///D3D cursor Set�� �ʿ��� �ڿ�
//IDirect3DSurface9* g_surfcursor = nullptr; //�׽�Ʈ ��
//IDirect3DTexture9* g_cursortex = nullptr; //�׽�Ʈ ��
LPDIRECT3DTEXTURE9 g_cursorTex = 0;
LPD3DXSPRITE g_cursorSprite = 0;
D3DXVECTOR3 g_cursorPos;

/// tool Camera ���� ����
D3DXVECTOR3		g_EyePoint = { 0, 0, 0 };
D3DXVECTOR3		g_LookAtPoint = { 0, 0, 1.0f };
D3DXVECTOR3		g_UpVector = { 0, 1, 0 };

float	g_Ratio = 1280 / 720;
float	g_Width = 1280;