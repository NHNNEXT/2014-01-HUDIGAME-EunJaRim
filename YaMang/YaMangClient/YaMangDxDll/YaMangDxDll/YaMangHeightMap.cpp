﻿#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"


//////////////////////////////////////////////////////////////////////////
//이하 heightMap 관련 함수
//////////////////////////////////////////////////////////////////////////
/**
*@breif
*
*/
YAMANGDXDLL_API HRESULT HeightMapTextureImport( HWND hWnd, LPCTSTR heightMap, LPCTSTR mapTexture )
{
	if ( FAILED( D3DXCreateTextureFromFileEx( g_D3dDevice, heightMap, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_MapHeightInfo ) ) )
	{
		MessageBox( NULL, L"Could not find heightMap file", L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}


	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, mapTexture, &g_MapTexture ) ) )
	{
		MessageBox( NULL, L"Could not find heightMapTexture file", L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

////////////////////////////////////////////////////////////////////////// triz
//클라이언트 읽기 전용 함수 제작할 것
//1. 제공 받은 높이 파일로 지형 그리기(vertexBuffer)
//   높이값 array return
//2. 제공 받은 텍스쳐 입히기
//3. 인덱스 계산해서 넣기(툴용 함수에서 해당 영역 떼어 함수화)
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//툴용 지형 함수
//이하 툴용 함수
//쓰기를 위한 함수
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT MapToolTextureImport( HWND hWnd, LPCTSTR toolTexture )
{
	for ( int i = 0; i < MAP_TEXTURE_COUNT; ++i )
	{
		if ( nullptr == g_MapTextureArray[i] )
		{
			if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, toolTexture, &g_MapTextureArray[i] ) ) )
			{
				MessageBox( NULL, L"Could not find heightMapTexture file", L"YaMang.DLL", MB_OK );
				return E_FAIL;
			}
			break;
		}

	}
	return S_OK;

}


YAMANGDXDLL_API void InitGroundMesh( int row, int col )
{
	if ( g_Mesh != nullptr )
	{
		g_Mesh->Release( );
	}
	g_XHeight = col + 1;
	g_ZHeight = row + 1;

	int verticesCount = (g_XHeight)* ( g_ZHeight );
	int faceCount = ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2;

	//mesh를 직접 만들어 보자
	//mesh를 직접 만들게 되면 사실 global vertex buffer와 index buffer가 무의미 하다.

	HRESULT hr = NULL;
	hr = D3DXCreateMeshFVF( faceCount, verticesCount, D3DXMESH_MANAGED | D3DXMESH_32BIT, D3DFVF_CUSTOMVERTEX, g_D3dDevice, &g_Mesh );
	if ( hr != S_OK )
	{
		MessageBox( NULL, L"CreateMesh Failed", L"YaMang.DLL", MB_OK );
		return;
	}
}


YAMANGDXDLL_API void CreateRawGround( int row, int col, float pixelSize )
{
	int verticesCount = (g_XHeight)* ( g_ZHeight );

	CUSTOMVERTEX* baseVertex = new CUSTOMVERTEX[verticesCount];
	if ( nullptr == baseVertex )
	{
		assert( false );
	}

	int startIdx = 0;
	D3DLOCKED_RECT d3drc;

	if (g_MapHeightInfo)
	{
// 		D3DSURFACE_DESC ddsd;
// 		g_MapHeightInfo->GetLevelDesc( 0, &ddsd );
		
		g_MapHeightInfo->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	}

	CUSTOMVERTEX vPos0;

	vPos0.m_VertexPoint.x = -1.f * col * pixelSize * 0.5f;
	vPos0.m_VertexPoint.y = 0.0f;
	vPos0.m_VertexPoint.z = -1.f * row * pixelSize * 0.5f;
	for ( int z = 0; z <= row; ++z )
	{
		for ( int x = 0; x <= col; ++x )
		{
			baseVertex[startIdx].m_VertexPoint.x = vPos0.m_VertexPoint.x + ( pixelSize * x );
			if (g_MapHeightInfo)
			{
				baseVertex[startIdx].m_VertexPoint.y = ( static_cast<float>( *( static_cast<LPDWORD>(d3drc.pBits) + col *( row - 1 ) + x - z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) )/20.f;
			}
			else
			{
				baseVertex[startIdx].m_VertexPoint.y = 0.f;
			}
			baseVertex[startIdx].m_VertexPoint.z = vPos0.m_VertexPoint.z + ( pixelSize * z );

			baseVertex[startIdx].m_Diffuse = D3DCOLOR_ARGB( 255, 255, 255, 255 );

			baseVertex[startIdx].m_VertexTexturePoint.x = static_cast<float>(x)* 1 / col;
			baseVertex[startIdx].m_VertexTexturePoint.y = static_cast<float>(z)* 1 / row;

			++startIdx;
		}
	}
	/*
	//vertex 내용 확인
	for ( int i = 0; i < startIdx; ++i )
	{
	printf_s( "vertex %d: (%f, %f)\n", i, baseVertex[i].vertexPoint.x, baseVertex[i].vertexPoint.z );
	}
	*/

	void *pVertices;
	if ( FAILED( g_Mesh->LockVertexBuffer( 0, &pVertices ) ) )
	{
		if ( baseVertex )
		{
			delete[] baseVertex;
			baseVertex = nullptr;
		}
		MessageBox( NULL, L"Fail in lock VertexBuffer", L"YaMang.DLL", MB_OK );
		return;
	}
	memcpy( pVertices, baseVertex, verticesCount * sizeof( CUSTOMVERTEX ) );
	g_Mesh->UnlockVertexBuffer( );
	if (g_MapHeightInfo)
	{
		g_MapHeightInfo->UnlockRect( 0 );
	}
	


	int indicesCount = g_XHeight * g_ZHeight + ( g_ZHeight - 2 )*( g_XHeight + 1 );
	UINT* baseIndex = new UINT[indicesCount];
	if ( nullptr == baseIndex )
	{
		assert( false );
	}

	//퇴적 삼각형 적용 부분
	startIdx = 0;
	for ( int z = 0; z < row; ++z )
	{
		if ( z % 2 == 0 )
		{
			int x;
			for ( x = 0; x < col + 1; ++x )
			{
				UINT a = static_cast<UINT>( x + ( z*g_XHeight ) );
				baseIndex[startIdx++] = a;
				UINT b = static_cast<UINT>( x + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = b;
				//Log( "a: %d | b: %d\n", a, b );
			}
			if ( z != g_ZHeight - 2 )
			{
				UINT c = static_cast<UINT>( ( x - 1 ) + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = c;
				//Log( "c: %d \n", c );
			}
		}
		else
		{
			int x;
			for ( x = col; x >= 0; --x )
			{
				UINT a = static_cast<UINT>( x + ( z*g_XHeight ) );
				baseIndex[startIdx++] = a;
				UINT b = static_cast<UINT>( x + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = b;
				//Log( "a: %d | b: %d\n", a, b );
			}
			if ( z != g_ZHeight - 2 )
			{
				UINT c = static_cast<UINT>( ( x + 1 ) + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = c;
				//Log( "c: %d \n", c );
			}
		}
	}

	/*
	//index buffer에 뭐있나?
	for ( int i = 0; i < startIdx; ++i )
	{
	printf_s( "index %d: %d\n", i, baseIndex[i] );
	}
	*/

	void* pIndices;
	if ( FAILED( g_Mesh->LockIndexBuffer( 0, &pIndices ) ) )
	{
		if ( baseVertex )
		{
			delete[] baseVertex;
			baseVertex = nullptr;
		}
		if ( baseIndex )
		{
			delete[] baseIndex;
			baseIndex = nullptr;
		}
		MessageBox( NULL, L"Fail in lock IndexBuffer", L"YaMang.DLL", MB_OK );
		return;
	}
	memcpy( pIndices, baseIndex, sizeof(UINT)*indicesCount );

	g_Mesh->UnlockIndexBuffer( );

	if ( baseVertex )
	{
		delete[] baseVertex;
		baseVertex = nullptr;
	}
	if ( baseIndex )
	{
		delete[] baseIndex;
		baseIndex = nullptr;
	}
}

//카메라 행렬을 인자값 받아서 정렬되도록 수정 필요 triz
YAMANGDXDLL_API void ToolViewSetting( int width, int height )
{
	// 월드 행렬
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity( &world );
	g_D3dDevice->SetTransform( D3DTS_WORLD, &world );


	D3DXVECTOR3 vEyePt( 0.f, 70.f, -350.f );
	D3DXVECTOR3 vLookatPt( 0.f, 69.8f, -349.f );
	D3DXVECTOR3 vUpVec( 0.f, 1.f, 0.f );

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_D3dDevice->SetTransform( D3DTS_VIEW, &matView );

	// 프로젝션 행렬
	// 창크기 변경에 따라 크고 작아지게 할 것
	//SetAspectRatio( 729, 588 );
	SetAspectRatio( width, height );
}