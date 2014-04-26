#include <d3dx9.h>


//vertex, index ����
struct CUSTOMVERTEX
{
	D3DXVECTOR3 vertexPoint;
	DWORD Diffuse;
	D3DXVECTOR2 vertexTexturePoint;
};

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct MYINDEX
{
	UINT _0, _1, _2;
};


//////////////////////////////////////////////////////////////////////////
//Global val ����
//////////////////////////////////////////////////////////////////////////
//�θ� Process ���� �ڿ�
extern LPDIRECT3D9 g_D3D;
extern LPDIRECT3DDEVICE9 g_D3dDevice;

//////////////////////////////////////////////////////////////////////////
//Height Map ������ ���� �ڿ�
//////////////////////////////////////////////////////////////////////////
extern LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer;
extern LPDIRECT3DINDEXBUFFER9 g_IdxBuffer;

extern LPDIRECT3DTEXTURE9 g_TexHeight;
extern LPDIRECT3DTEXTURE9 g_TexDiffuse;

extern DWORD g_XHeight;
extern DWORD g_ZHeight;

//////////////////////////////////////////////////////////////////////////
//�ؽ�Ʈ ��¿� �ʿ��� �ڿ�
//////////////////////////////////////////////////////////////////////////
extern ID3DXFont*	g_Font;		// ���ڸ� �׸� ��Ʈ ��Ÿ�� ��ü
extern ID3DXSprite*	g_Sprite;		// ��Ʈ�� �׸� ��������Ʈ ��ü 

//////////////////////////////////////////////////////////////////////////
///D3D cursor Set�� �ʿ��� �ڿ�
//////////////////////////////////////////////////////////////////////////
//IDirect3DSurface9* g_surfcursor = nullptr; //�׽�Ʈ ��
//IDirect3DTexture9* g_cursortex = nullptr; //�׽�Ʈ ��

extern LPDIRECT3DTEXTURE9	g_cursorTex;
extern LPD3DXSPRITE			g_cursorSprite;
extern D3DXVECTOR3			g_cursorPos;


//////////////////////////////////////////////////////////////////////////
/// tool Camera ���� ����
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_EyePoint;
extern D3DXVECTOR3		g_LookAtPoint;
extern D3DXVECTOR3		g_UpVector;