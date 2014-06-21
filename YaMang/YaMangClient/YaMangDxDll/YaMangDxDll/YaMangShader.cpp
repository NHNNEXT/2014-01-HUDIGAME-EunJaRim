#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"
#include <timeapi.h>

YAMANGDXDLL_API HRESULT ShaderCreate( int size )
{
	if ( size <= 0 || g_EffectSize != 0 )
	{
		return E_FAIL;
	}

	g_Effects = new LPD3DXEFFECT[size];
	g_EffectSize = size;

	for ( int i = 0; i < g_EffectSize; ++i )
	{
		g_Effects[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT ShaderImport( LPCTSTR effectFile, int id )
{
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG | D3DXSHADER_NO_PRESHADER;

	HRESULT hr = S_OK;
	hr = D3DXCreateEffectFromFile( g_D3dDevice, effectFile, NULL, NULL, dwShaderFlags, NULL, &g_Effects[id], NULL );

	if ( FAILED( hr ) )
	{
		MessageBox( NULL, L"Effect Load Failed", effectFile, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// �̰Ž� �ؽ��� �� �ڵ尡 �ƴϹǴϴ� - �Űܾ� �� �Ф� ��¼�ٺ��� �ϴ� ����
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void DrawBillboardByTexture( int id )
{
	if ( id <= 0 || id >= g_MeshTextureSize )
	{
		return;
	}

	struct MYVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		float px, py, pz;
		float tu, tv;
	};

	// ������ ����
	MYVERTEX vtx[4] =
	{
		{ -1, 0, 0, 0, 1 },
		{ -1, 4, 0, 0, 0 },
		{ 1, 0, 0, 1, 1 },
		{ 1, 4, 0, 1, 0 }
	};

	if ( g_IsEffectReady && g_Effects[1] )
	{
		UINT nPass;
		float thisTime = D3DX_PI * ( timeGetTime() % 600 ) / 300;

		g_Effects[1]->SetFloat( "time", thisTime );

		// fx��¿� ����� ��ũ�� ����
		g_Effects[1]->SetTechnique( "MyShader" );

		D3DXMATRIXA16 worldMatrix;
		g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
		D3DXMATRIXA16 viewingMatrix;
		g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
		D3DXMATRIXA16 projectionMatrix;
		g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

		D3DXMATRIXA16 thisMatrix = worldMatrix * viewingMatrix * projectionMatrix;

		g_Effects[1]->SetMatrix( "matWVP", &thisMatrix );

		// fx�� ����� ��°���
		g_Effects[1]->Begin( &nPass, D3DXFX_DONOTSAVESHADERSTATE );

		// PASS ������ŭ ���
		for ( UINT i = 0; i < nPass; ++i )
		{
			g_Effects[1]->BeginPass( i );

			g_Effects[1]->SetTexture( "tex0", g_MeshTextures[id] );
			g_D3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( MYVERTEX ) );
			
			g_Effects[1]->EndPass();
		}

		g_Effects[1]->End();
	}
	else
	{
		g_D3dDevice->SetVertexShader( NULL );
		g_D3dDevice->SetPixelShader( NULL );
				
		// 0�� �ؽ�ó�� ������ �ؽ�ó�� �ø���
		g_D3dDevice->SetTexture( 0, g_MeshTextures[id] );
		g_D3dDevice->SetFVF( MYVERTEX::FVF );

		g_D3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( MYVERTEX ) );
	}

	g_IsEffectReady = false;
}

YAMANGDXDLL_API HRESULT MeshTextureCreateBySize( INT size )
{
	if ( size <= 0 || g_MeshTextureSize != 0)
	{
		return E_FAIL;
	}

	g_MeshTextures = new LPDIRECT3DTEXTURE9[size];
	g_MeshTextureSize = size;

	for ( int i = 0; i < g_MeshTextureSize; ++i )
	{
		g_MeshTextures[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT MeshTextureImport( LPCTSTR meshTexture, int id )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, meshTexture, &g_MeshTextures[id] ) ) )
	{
		MessageBox( NULL, L"Mesh Texture Load Failed", meshTexture, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}