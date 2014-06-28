﻿#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"


/**
 *@breif MeshObject(유닛 등)에 대한 메모리 소거 함수
 *
 */
YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal )
{
	if ( NULL != inputVal->MeshMarterials )
	{
		delete[] inputVal->MeshMarterials;
	}
	if ( inputVal->MeshTexture )
	{
		for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
		{
			if ( inputVal->MeshTexture[i] )
			{
				inputVal->MeshTexture[i]->Release();
			}
		}
		delete[] inputVal->MeshTexture;
	}
	if ( NULL != inputVal->importedMesh )
	{
		( inputVal->importedMesh )->Release();
	}
}


YAMANGDXDLL_API void HeightMapCleanup()
{
	if ( NULL != g_HeightMap )
	{
		delete[] g_HeightMap;
	}
	if ( NULL != g_HeightMapIndexBuffer )
	{
		g_HeightMapIndexBuffer->Release();
	}
	if ( NULL != g_HeightMapVertexBuffer )
	{
		g_HeightMapVertexBuffer->Release();
	}

	if ( NULL != g_MapHeightInfo )
	{
		g_MapHeightInfo->Release();
	}

	if ( NULL != g_MapTexture )
	{
		g_MapTexture->Release();
	}

	if ( g_MapTextureArray )
	{
		for ( DWORD i = 0; i < MAP_TEXTURE_COUNT; ++i )
		{
			if (g_MapTextureArray[i])
			{
				g_MapTextureArray[i]->Release();
			}
		}
	}
}


YAMANGDXDLL_API void CursorCleanUp()
{
	if ( g_CursorSprite )
	{
		g_CursorSprite->Release();
	}

	if ( g_CursorTex )
	{
		for ( int i = 0; i < g_CursorMaxSize; ++i )
		{
			if ( g_CursorTex[i] )
			{
				g_CursorTex[i]->Release();
			}
		}
		delete[] g_CursorTex;
		g_CursorTex = nullptr;
	}
}


YAMANGDXDLL_API void UICleanUp()
{
	if (g_UISprite)
	{
		g_UISprite->Release();
		g_UISprite = nullptr;
	}

	if (g_UITexures)
	{
		for (int i = 0; i < g_UIMaxSize; ++i)
		{
			if (g_UITexures[i])
			{
				g_UITexures[i]->Release();
			}
		}
		delete[] g_UITexures;
		g_UITexures = nullptr;
	}
}