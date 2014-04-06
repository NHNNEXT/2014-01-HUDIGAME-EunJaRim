// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// YAMANGDXDLL_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef YAMANGDXDLL_EXPORTS
#define YAMANGDXDLL_API __declspec(dllexport)
#else
#define YAMANGDXDLL_API __declspec(dllimport)
#endif

#include <d3dx9.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////////
// return struct
// �Լ��� �θ� ��� �ش� struct�� ��ȯ�� ���θ� ����� �� �ֵ��� ��
//////////////////////////////////////////////////////////////////////////
struct MESHOBJECT
{
	//vertexbuffer�� ���� ���� ����ϴ� ���� ���� ���̹Ƿ� ����
	//LPDIRECT3DVERTEXBUFFER9 VertexBuffer = nullptr;

	LPD3DXMESH importedMesh = nullptr;
	D3DMATERIAL9* MeshMarterials = nullptr;
	LPDIRECT3DTEXTURE9* MeshTexture = nullptr;

	DWORD NumMaterials = 0L;
};

extern "C"{
	//ó���� ���������� �����ϴ� �Լ���
	YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd );
	YAMANGDXDLL_API void D3DCleanUp( );

	//���� �̸� �־� mesh object �޾ƿ���
	YAMANGDXDLL_API HRESULT InitGeometry( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal );
	
	//x,y,z�� �̵��� ������ 0�� ������ �˴ϴ�.
	YAMANGDXDLL_API void Render( MESHOBJECT* inputVal, float moveX, float moveY, float moveZ );

	//�� object ������ ���
	YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal );

	//���ο��� ó���ϵ��� ����
	//YAMANGDXDLL_API void SetupTranslateMatrices(float moveX, float moveY, float moveZ);
	//YAMANGDXDLL_API void ViewSetting();
}

//extern YAMANGDXDLL_API int nyaMangDxDll;



