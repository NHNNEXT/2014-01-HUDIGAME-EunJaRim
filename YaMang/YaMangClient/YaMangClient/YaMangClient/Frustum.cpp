#include "stdafx.h"
#include "Frustum.h"


Frustum::Frustum()
{
	ZeroMemory( m_Vertex, sizeof( m_Vertex[0] ) * 8 );
	ZeroMemory( m_Plane, sizeof( m_Plane[0] ) * 6 );
}


Frustum::~Frustum()
{
}

BOOL Frustum::Make( D3DXMATRIXA16* viewProjMatrix )
{
	int				i;
	D3DXMATRIXA16	invMatrix;

	m_Vertex[0].x = -1.0f;	m_Vertex[0].y = -1.0f;	m_Vertex[0].z = 0.0f;
	m_Vertex[1].x = 1.0f;	m_Vertex[1].y = -1.0f;	m_Vertex[1].z = 0.0f;
	m_Vertex[2].x = 1.0f;	m_Vertex[2].y = -1.0f;	m_Vertex[2].z = 1.0f;
	m_Vertex[3].x = -1.0f;	m_Vertex[3].y = -1.0f;	m_Vertex[3].z = 1.0f;
	m_Vertex[4].x = -1.0f;	m_Vertex[4].y = 1.0f;	m_Vertex[4].z = 0.0f;
	m_Vertex[5].x = 1.0f;	m_Vertex[5].y = 1.0f;	m_Vertex[5].z = 0.0f;
	m_Vertex[6].x = 1.0f;	m_Vertex[6].y = 1.0f;	m_Vertex[6].z = 1.0f;
	m_Vertex[7].x = -1.0f;	m_Vertex[7].y = 1.0f;	m_Vertex[7].z = 1.0f;

	D3DXMatrixInverse( &invMatrix, NULL, viewProjMatrix );

	for ( i = 0; i < 8; i++ )
	{
		D3DXVec3TransformCoord( &m_Vertex[i], &m_Vertex[i], &invMatrix );
	}
	
	m_CameraPosition = ( m_Vertex[0] + m_Vertex[5] ) / 2.0f;

	//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);				// �� ���(top)
	//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);				// �� ���(bottom)
	//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);				// �� ���(near)
	D3DXPlaneFromPoints( &m_Plane[3], m_Vertex + 2, m_Vertex + 6, m_Vertex + 7 );	// �� ���(far)
	D3DXPlaneFromPoints( &m_Plane[4], m_Vertex, m_Vertex + 3, m_Vertex + 7 );		// �� ���(left)
	D3DXPlaneFromPoints( &m_Plane[5], m_Vertex + 1, m_Vertex + 5, m_Vertex + 6 );	// �� ���(right)

	return TRUE;
}

BOOL Frustum::IsIn( D3DXVECTOR3* point )
{
	float		fDist;
	//	int			i;

	// ����� left, right, far plane�� �����Ѵ�.
	//	for( i = 0 ; i < 6 ; i++ )
	{
		fDist = D3DXPlaneDotCoord( &m_Plane[3], point );
		if ( fDist > PLANE_EPSILON )
		{
			// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
			return FALSE;
		}
		fDist = D3DXPlaneDotCoord( &m_Plane[4], point );
		if ( fDist > PLANE_EPSILON )
		{
			// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
			return FALSE;
		}
		fDist = D3DXPlaneDotCoord( &m_Plane[5], point );
		if ( fDist > PLANE_EPSILON )
		{
			// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Frustum::IsInSphere( D3DXVECTOR3* point, float radius )
{
	float		fDist;

	fDist = D3DXPlaneDotCoord( &m_Plane[3], point );
	if ( fDist > ( radius + PLANE_EPSILON ) )
	{
		// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
		return FALSE;
	}
	fDist = D3DXPlaneDotCoord( &m_Plane[4], point );
	if ( fDist > ( radius + PLANE_EPSILON ) )
	{
		// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
		return FALSE;
	}
	fDist = D3DXPlaneDotCoord( &m_Plane[5], point );
	if ( fDist > ( radius + PLANE_EPSILON ) )
	{
		// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
		return FALSE;
	}

	return TRUE;
}
