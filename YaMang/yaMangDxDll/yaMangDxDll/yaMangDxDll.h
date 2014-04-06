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

#include "MeshObject.h"




extern "C"{
	//ó���� ���������� �����ϴ� �Լ���
	YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd );
	YAMANGDXDLL_API void D3DCleanUp( );

	//���� �̸� �־� mesh object �޾ƿ���
	YAMANGDXDLL_API HRESULT InitGeometry( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal );
	
	//render ���� �غ� beginScene
	//x,y,z�� �̵��� ������ 0�� ������ �˴ϴ�.
	YAMANGDXDLL_API void PreRendering( float moveX, float moveY, float moveZ );

	//Main rendering MeshObject�� �־� render ����
	YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal );

	//rendering �� ����̽� endScene
	YAMANGDXDLL_API void PostRendering();

	//�� object ������ ���
	YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal );

	//���ο��� ó���ϵ��� ����
	//YAMANGDXDLL_API void SetupTranslateMatrices(float moveX, float moveY, float moveZ);
	//YAMANGDXDLL_API void ViewSetting();
}

//extern YAMANGDXDLL_API int nyaMangDxDll;



