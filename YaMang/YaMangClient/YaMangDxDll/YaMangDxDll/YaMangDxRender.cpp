#include "stdafx.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include "GlobalVar.h"
#include "InnerResource.h"


//////////////////////////////////////////////////////////////////////////
//render�� pre - main - post renderring���� ����
//pre������ matrix, view, light�� ���� setting ����
//main������ mesh object�� ���� ���� rendering ����
//post������ pre���� ������ setting �ʱ�ȭ
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API bool PreRendering()
{
	if ( NULL == g_D3dDevice )
	{
		return false;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 10, 10, 10 ), 1.0f, 0 );

	bool flag = false;

	// ���� ����ڵ�
	// pre rendering �ܰ迡�� ������� ������ ���� render ��� ����
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatrices();

		// lightsetting
		// �ϴ� 1�� ����, ���� ����Ʈ ���� �� Ȯ���Ǹ� ���� �޾� ����
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "������ ����!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Log( "Render Begin \n" );
		// Log( "pre render �Ϸ�!\n" );

		flag = true;
	}

	return flag;
}


YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal )
{
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	// Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
}


YAMANGDXDLL_API void PostRendering()
{
	D3DXMATRIXA16 identityMatrix;
	D3DXMatrixIdentity( &identityMatrix );
	SetMatrix( &identityMatrix );

	g_D3dDevice->EndScene();

	//Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}

//////////////////////////////////////////////////////////////////////////
//heightMap�� ���� ���� �Լ�
//render�� pre - main - post renderring �������� main�� ���
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderHeightMap()
{
	//���� ������ ������� �ʾ���
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	IDirect3DVertexBuffer9* RenderVertexBuffer = nullptr;
	g_Mesh->GetVertexBuffer( &RenderVertexBuffer );

	IDirect3DIndexBuffer9* RenderIndexBuffer = nullptr;
	g_Mesh->GetIndexBuffer( &RenderIndexBuffer );

	g_D3dDevice->SetStreamSource( 0, RenderVertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_Tex0 );
	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	g_D3dDevice->SetIndices( RenderIndexBuffer );
	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, g_XHeight * g_ZHeight, 0, ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2 + ( g_ZHeight - 2 ) * 3 );
}




//////////////////////////////////////////////////////////////////////////
//SkyBox�� ���� ���� �Լ�
//render�� pre - main - post renderring �������� main�� ���
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderSkyBox()
{
	if ( g_SkyBoxMesh )
	{
		g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		for ( int i = 0; i < 6; i++ )
		{
			g_D3dDevice->SetTexture( 0, g_SkyBoxTextures[i] );
			g_SkyBoxMesh->DrawSubset( i );
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//ĳ���� ���� ���� ���� �Լ�
//render�� pre - main - post renderring �������� main�� ���
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderingTool( MESHOBJECT* inputVal )
{
	if ( NULL == g_D3dDevice )
	{
		return;
	}

	if ( g_Mesh == nullptr )
	{
		InitGroundMesh( 100, 100 );
	}
	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	// ���� ����ڵ�
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatricesTool();
		// ViewSetting();

		// lightsetting
		// �ϴ� 1�� ����, ���� ����Ʈ ���� �� Ȯ���Ǹ� ���� �޾� ����
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "������ ����!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Log( "Render Begin \n" );
		// Log( "pre render �Ϸ�!\n" );
	}

	// ī�޶� ����
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector );
	SetMatrix( &viewMatrix, true );

	// �����ֱ� ���� ���� ����
	//InitGroundMesh(100, 100);
	CreateRawGround( 100, 100, 10 );
	RenderHeightMap();

	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	// Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
	g_D3dDevice->EndScene();

	// Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}