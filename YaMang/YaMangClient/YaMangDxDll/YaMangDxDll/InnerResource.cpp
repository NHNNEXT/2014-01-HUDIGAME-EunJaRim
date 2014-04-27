#include "stdafx.h"
#include "InnerResource.h"
#include "GlobalVar.h"

void SetupTranslateMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_D3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//x, y, z�� �Է� ���� ���� �̵� ó��
	D3DXMATRIXA16 thisMatrix;
	D3DXMatrixTranslation(&thisMatrix, 0, 0, 0);
	g_D3dDevice->MultiplyTransform(D3DTS_WORLD, &thisMatrix);

	//���� �߰� ��Ʈ���� ó�� �ʿ� ���뿡 ���� �߰� ����
}

//ī�޶� ������ ���Ŀ� �����ϵ��� ��
//default�� �ϴ� �̷��� ��
void ViewSetting()
{
	D3DXVECTOR3 vEyePt(0.f, 3.f, -5.f);
	D3DXVECTOR3 vLookatPt(0.f, 0.f, 0.f);
	D3DXVECTOR3 vUpVec(0.f, 1.f, 0.f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_D3dDevice->SetTransform(D3DTS_VIEW, &matView);
}

//light ���ÿ� ���ؼ��� ���� �߰�
//light�� �������� �̸� ������ �ְ� �� ���ΰ�?
//�ƴϸ� ����ڰ� �ּҷ� �߰��ϰ� �ش� light�� ���� �ڿ����� �� ���ΰ�?
void Lighting(int lightNum)
{
	//���� �Ӽ� �ο�
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_D3dDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir0;
	D3DLIGHT9 light0;
	ZeroMemory(&light0, sizeof(D3DLIGHT9));
	light0.Type = D3DLIGHT_DIRECTIONAL;
	light0.Diffuse.r = 1.f;
	light0.Diffuse.g = 1.f;
	light0.Diffuse.b = 1.f;
	vecDir0 = D3DXVECTOR3(-1.f, 1.f, 0.f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light0.Direction, &vecDir0);
	light0.Range = 1000.f;


	D3DXVECTOR3 vecDir1;
	D3DLIGHT9 light1;
	ZeroMemory(&light1, sizeof(D3DLIGHT9));
	light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r = 1.f;
	light1.Diffuse.g = 1.f;
	light1.Diffuse.b = 1.f;
	vecDir1 = D3DXVECTOR3(1.f, 1.f, 0.f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light1.Direction, &vecDir1);
	light1.Range = 1000.f;

	g_D3dDevice->SetLight(0, &light0);
	g_D3dDevice->SetLight(1, &light1);

	switch (lightNum)
	{
	case 1:
		g_D3dDevice->LightEnable(0, TRUE);
		g_D3dDevice->LightEnable(1, FALSE);
		break;
	case 2:
		g_D3dDevice->LightEnable(0, FALSE);
		g_D3dDevice->LightEnable(1, TRUE);
		break;
	default:
		break;
	}

	g_D3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_D3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);
}


//////////////////////////////////////////////////////////////////////////
///rendering for tool
//////////////////////////////////////////////////////////////////////////
void SetupTranslateMatricesTool()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_D3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//x, y, z�� �Է� ���� ���� �̵� ó��
	D3DXMATRIXA16 thisMatrix;
	D3DXMatrixTranslation(&thisMatrix, 0, 0, 0);
	g_D3dDevice->MultiplyTransform(D3DTS_WORLD, &thisMatrix);

	//���� �߰� ��Ʈ���� ó�� �ʿ� ���뿡 ���� �߰� ����
}

//////////////////////////////////////////////////////////////////////////
//height map ���迡 ���� ���� ȯ���մϴ�.
// :)
//////////////////////////////////////////////////////////////////////////

HRESULT InitVertexBuffer( HWND hWnd )
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;

	g_TexHeight->GetLevelDesc( 0, &ddsd );
	g_XHeight = ddsd.Width;
	g_ZHeight = ddsd.Height;

	if ( FAILED( g_D3dDevice->CreateVertexBuffer( ddsd.Width * ddsd.Height * sizeof( CUSTOMVERTEX ), D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &g_VertexBuffer, NULL ) ) )
	{
		MessageBox( NULL, L"Fail in Creating VertexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	//surface lock
	//Ȯ�θ� �ϰ� ������ �����Ƿ� readonly
	g_TexHeight->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );

	VOID* vertices;
	if ( FAILED( g_VertexBuffer->Lock( 0, g_XHeight*g_ZHeight*sizeof( CUSTOMVERTEX ), (void**)&vertices, 0 ) ) )
	{
		MessageBox( NULL, L"Fail in lock VertexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}


	// Vertex ����ü ä���
	CUSTOMVERTEX vertex;
	CUSTOMVERTEX* vertexPointer = (CUSTOMVERTEX*)vertices;

	for ( DWORD z = 0; z < g_ZHeight; ++z )
	{
		for ( DWORD x = 0; x < g_XHeight; ++x )
		{
			vertex.vertexPoint.x = (float)x - g_XHeight / 2.0f;
			vertex.vertexPoint.z = -( (float)z - g_ZHeight / 2.0f );
			vertex.vertexPoint.y = ( (float)( *( (LPDWORD)d3drc.pBits + x + z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) ) / 10.f;

			// normal ���̰�
			// 0,0,0 �������� �� ������ normal ���� ���
			// vertex.vertexNormal.x = vertex.vertexPoint.x;
			// vertex.vertexNormal.y = vertex.vertexPoint.y;
			// vertex.vertexNormal.z = vertex.vertexPoint.z;
			// 
			// // ���� ���ͷ� ����� ��
			// // ����ȭ ���ͷ� �����ϴ� ����
			// D3DXVec3Normalize(&vertex.vertexNormal, &vertex.vertexNormal);

			vertex.vertexTexturePoint.x = (float)x / ( g_XHeight - 1 );
			vertex.vertexTexturePoint.y = (float)z / ( g_ZHeight - 1 );
			*vertexPointer++ = vertex;
		}
	}
	g_VertexBuffer->Unlock();

	g_TexHeight->UnlockRect( 0 );

	return S_OK;
}


HRESULT InitIdxBuffer(HWND hWnd)
{
	if (FAILED(g_D3dDevice->CreateIndexBuffer((g_XHeight - 1)*(g_ZHeight - 1) * 2 * sizeof(MYINDEX), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &g_IdxBuffer, NULL)))
	{
		MessageBox(NULL, L"Fail in CreateIndexBuffer", L"YaMang.exe", MB_OK);
		return E_FAIL;
	}

	MYINDEX idx;
	MYINDEX* idxPointer;

	if (FAILED(g_IdxBuffer->Lock(0, (g_XHeight - 1)*(g_ZHeight - 1) * 2 * sizeof(MYINDEX), (void**)&idxPointer, 0)))
	{
		MessageBox(NULL, L"Fail in index locking", L"YaMang.exe", MB_OK);
		return E_FAIL;
	}

	for (UINT z = 0; z < g_ZHeight - 1; ++z)
	{
		for (UINT x = 0; x < g_XHeight - 1; ++x)
		{
			idx._0 = static_cast<UINT>( z * g_XHeight + x );
			idx._1 = static_cast<UINT>( z * g_XHeight + x + 1 );
			idx._2 = static_cast<UINT>( ( z + 1 ) * g_XHeight + x );
			*idxPointer++ = idx;
			idx._0 = static_cast<UINT>( ( z + 1 ) * g_XHeight + x );
			idx._1 = static_cast<UINT>( z * g_XHeight + x + 1 );
			idx._2 = static_cast<UINT>( ( z + 1 ) * g_XHeight + x + 1 );
			*idxPointer++ = idx;
		}
	}
	g_IdxBuffer->Unlock();

	return S_OK;
}


