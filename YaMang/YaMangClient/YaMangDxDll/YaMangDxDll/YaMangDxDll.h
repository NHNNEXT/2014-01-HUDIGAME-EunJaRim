﻿// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 _EXPORTS 기호로
// 컴파일되며, 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// YAMANGDXDLL_API 함수를 DLL에서 가져오는 것으로 보고, 이 DLL은
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
#ifdef YAMANGDXDLL_EXPORTS
#define YAMANGDXDLL_API __declspec(dllexport)
#else
#define YAMANGDXDLL_API __declspec(dllimport)
#endif

#include "MeshObject.h"

extern "C"{
	//처음과 마지막에만 실행하는 함수들
	YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd, long width = 1280, long height = 720 );
	YAMANGDXDLL_API void D3DCleanUp();

	//파일 이름 넣어 mesh object 받아오기
	YAMANGDXDLL_API HRESULT InitGeometry( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal );

	//render 사전 준비 beginScene
	//x,y,z축 이동이 없으면 0을 넣으면 됩니다.
	YAMANGDXDLL_API bool PreRendering();

	//Main rendering MeshObject를 넣어 render 진행
	YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal );

	//rendering 후 디바이스 endScene
	YAMANGDXDLL_API void PostRendering();

	//각 object 해제시 사용
	YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal );

	YAMANGDXDLL_API void SetMatrix( D3DXMATRIXA16* matrix, bool cameraSet = false );

	YAMANGDXDLL_API void SetAspectRatio( long width, long height );
	//내부에서 처리하도록 변경
	//YAMANGDXDLL_API void SetupTranslateMatrices(float moveX, float moveY, float moveZ);
	//YAMANGDXDLL_API void ViewSetting();

	//////////////////////////////////////////////////////////////////////////
	//여기 이하는 height map 출력을 위한 함수입니다.
	//height map은 일반 object와 g_D3D g_D3DDevice를 공유합니다.
	//////////////////////////////////////////////////////////////////////////

	//heightMap(file name) : 2승수 bitmap (128*128 추천)
	//mapTexture(file name) : 2승수 bitmap or tga file
	YAMANGDXDLL_API HRESULT HeightMapTextureImport( HWND hWnd, LPCTSTR heightMap, LPCTSTR mapTexture );
	YAMANGDXDLL_API void HeightMapCleanup();
	YAMANGDXDLL_API void HeightMapRender();

	//////////////////////////////////////////////////////////////////////////
	//Text Render 를 위한 함수
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API void InitTextRenderer( LPCWSTR fontType = L"맑은 고딕", int fontHeight = 20, int fontWidth = 0 );
	YAMANGDXDLL_API void RenderText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );
	YAMANGDXDLL_API void CleanUpTextRenderer();

	//////////////////////////////////////////////////////////////////////////
	// Picking Ray를 위한 부분
	//////////////////////////////////////////////////////////////////////////
	YAMANGDXDLL_API void GetViewPort9( D3DVIEWPORT9* viewport );
	YAMANGDXDLL_API void GetD3DProjMatrix( D3DXMATRIXA16* projMatrix );
	YAMANGDXDLL_API void GetD3DViewMatrix( D3DXMATRIXA16* viewMatrix );
}
//단일 변수 out sample
//extern YAMANGDXDLL_API int nyaMangDxDll;



