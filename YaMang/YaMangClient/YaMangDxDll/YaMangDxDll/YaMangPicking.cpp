#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// Picking Ray�� ���� �κ�
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY )
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIXA16 projectionMatrix;

	float rayX = 0.f;
	float rayY = 0.f;

	g_D3dDevice->GetViewport( &viewPort );
	g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

	rayX = ( ( ( ( ( mouseX - viewPort.X ) * 2.0f / (float)viewPort.Width ) - 1.0f ) ) - projectionMatrix( 2, 0 ) ) / projectionMatrix( 0, 0 );
	rayY = ( ( -( ( ( mouseY - viewPort.Y ) * 2.0f / (float)viewPort.Height ) - 1.0f ) ) - projectionMatrix( 2, 1 ) ) / projectionMatrix( 1, 1 );
	// Log( "Ray x : %f, y : %f\n", rayX, rayY );

	//viewportƮ����, �������� Ʈ���� ����
	g_RayOrigin = { 0.f, 0.f, 0.f };
	g_RayDirection = { rayX, rayY, 1.f };
	// Log( "����Ʈ, �������� ����\n" );

	//���� Ʈ���� ����
	D3DXMATRIXA16 viewingMatrix;
	g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
	D3DXMatrixInverse( &viewingMatrix, 0, &viewingMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &viewingMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &viewingMatrix );
	// Log( "���� ��ǥ ����\n" );

	//���� ��ǥ�� ����
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
	D3DXMatrixInverse( &worldMatrix, 0, &worldMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &worldMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &worldMatrix );
	// Log( "���� ��ǥ ����\n" );
	// Log( "origin: %f,%f,%f\n direction: %f, %f, %f\n", g_RayOrigin.x, g_RayOrigin.y, g_RayOrigin.z, g_RayDirection.x, g_RayDirection.y, g_RayDirection.z );

}

YAMANGDXDLL_API HRESULT TransPickedTriangle( float* pickedX, float* pickedZ )
{
	if ( !( pickedX && pickedZ ) )
	{
		return S_FALSE;
	}

	LPDIRECT3DVERTEXBUFFER9 presentVertexBuffer;
	g_Mesh->GetVertexBuffer( &presentVertexBuffer );

	CUSTOMVERTEX* VerticesStartPoint;
	presentVertexBuffer->Lock( 0, 0, (void**)&VerticesStartPoint, 0 );

	BOOL Hit1 = false;
	BOOL Hit2 = false;
	float dist1 = 0;
	float dist2 = 0;

	int trianglePointA = NULL, trianglePointB = NULL, trianglePointC = NULL, trianglePointD = NULL;

	for ( UINT z = 0; ( z < ( g_ZHeight - 1 ) ) && !( Hit1 | Hit2 ); ++z )
	{
		if ( z % 2 == 0 )
		{
			for ( UINT x = 0; ( x < ( g_XHeight - 1 ) ) && !( Hit1 | Hit2 ); ++x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x + 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				Hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x + 1 );
				Hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
		else
		{
			for ( UINT x = g_ZHeight - 1; ( x > 0 ) && !( Hit1 | Hit2 ); --x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x - 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				Hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x - 1 );
				Hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
	}

	HRESULT result = S_FALSE;

	if ( ( Hit1 && ( dist1 > 0 ) ) || ( Hit2 && ( dist2 > 0 ) ) )
	{
		CUSTOMVERTEX* intersectedVertexBufferStart;
		g_Mesh->LockVertexBuffer( 0, (void**)&intersectedVertexBufferStart );

		CUSTOMVERTEX* pointA;
		CUSTOMVERTEX* pointB;
		CUSTOMVERTEX* pointC;

		if ( Hit1 )
		{
			pointA = &VerticesStartPoint[trianglePointA];
			pointB = &VerticesStartPoint[trianglePointB];
			pointC = &VerticesStartPoint[trianglePointC];
		}
		else
		{
			pointA = &VerticesStartPoint[trianglePointB];
			pointB = &VerticesStartPoint[trianglePointC];
			pointC = &VerticesStartPoint[trianglePointD];
		}

		*pickedX += pointA->m_VertexPoint.x;
		*pickedZ += pointA->m_VertexPoint.z;

		g_Mesh->UnlockVertexBuffer();

		result = S_OK;
	}

	presentVertexBuffer->Unlock();
	presentVertexBuffer->Release();

	return result;
}

/*

		modeSelector = static_cast<AreaModeType>( modeSelector );

		switch ( modeSelector )
		{
			case AREA_MODE_NONE:

				break;
				// 			case AREA_MODE_COLOR:
				// 			{
				// 				pointA->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 				pointB->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 				pointC->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 			}
				break;
			case AREA_MODE_HIGHER:
				pointA->m_VertexPoint.y += 3;
				pointB->m_VertexPoint.y += 3;
				pointC->m_VertexPoint.y += 3;
				break;
			case AREA_MODE_LOWER:
				pointA->m_VertexPoint.y -= 3;
				pointB->m_VertexPoint.y -= 3;
				pointC->m_VertexPoint.y -= 3;
				break;
			case AREA_MODE_PAINT_TEXTURE:
				//�ؽ��� ������ ����
				pointA->m_VertexTexturePoint1.x = 0.f;
				pointA->m_VertexTexturePoint1.y = 0.f;
				pointB->m_VertexTexturePoint1.x = 1.f;
				pointB->m_VertexTexturePoint1.y = 1.f;
				pointC->m_VertexTexturePoint1.x = 1.f;
				pointC->m_VertexTexturePoint1.y = 1.f;
				break;
			default:
				break;
		}	
	}
}

*/