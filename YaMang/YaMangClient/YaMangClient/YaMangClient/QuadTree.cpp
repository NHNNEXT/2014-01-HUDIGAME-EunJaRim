#include "stdafx.h"
#include "QuadTree.h"
#include "MacroSet.h"
#include "Frustum.h"


QuadTree::QuadTree( int cx, int cy )
{
	m_Parent = nullptr;

	for ( int i = 0; i < 4; ++i )
	{
		m_Child[i] = nullptr;
		m_Neighbor[i] = nullptr;
	}

	m_Corner[CORNER_TL] = 0;
	m_Corner[CORNER_TR] = cx - 1;
	m_Corner[CORNER_BL] = cx * ( cy - 1 );
	m_Corner[CORNER_BR] = cx * cy - 1;
	m_Center = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;

	m_Culled = false;
	m_Radius = 0.0f;
}

QuadTree::QuadTree( QuadTree* parent )
{
	m_Parent = parent;
	m_Center = 0;
	for ( int i = 0; i < 4; ++i )
	{
		m_Child[i] = nullptr;
		m_Neighbor[i] = nullptr;
		m_Corner[i] = 0;
	}

	m_Culled = false;
	m_Radius = 0.0f;
}

QuadTree::~QuadTree()
{
	Destroy();
}

BOOL QuadTree::Build( CUSTOMVERTEX* heightMap )
{
	// ����Ʈ�� ����
	BuildQuadTree( heightMap );
	// �̿���� ����
	BuildNeighborNode( this, heightMap, m_Corner[CORNER_TR] + 1 );

	return TRUE;
}

//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
int QuadTree::GenerateIndex( LPVOID index, CUSTOMVERTEX* heightMap, Frustum* frustum, float ratioOfLOD )
{
	// �������� �ø�
	FrustumCull( heightMap, frustum );
	// ����� �ε��� ����, �������� ���� ����
	return GenTriIndex( 0, index, heightMap, frustum, ratioOfLOD );
}

QuadTree* QuadTree::AddChild( int topLeft, int topRight, int bottomLeft, int bottomRight )
{
	QuadTree* child = new QuadTree( this );
	child->SetCorners( topLeft, topRight, bottomLeft, bottomRight );

	return child;
}

BOOL QuadTree::SetCorners( int topLeft, int topRight, int bottomLeft, int bottomRight )
{
	m_Corner[CORNER_TL] = topLeft;
	m_Corner[CORNER_TR] = topRight;
	m_Corner[CORNER_BL] = bottomLeft;
	m_Corner[CORNER_BR] = bottomRight;
	m_Center = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;
	
	return TRUE;
}

BOOL QuadTree::SubDivide()
{
	int	topEdgeCenter = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] ) / 2;
	int bottomEdgeCenter = ( m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 2;
	int leftEdgeCenter = ( m_Corner[CORNER_TL] + m_Corner[CORNER_BL] ) / 2;
	int rightEdgeCenter = ( m_Corner[CORNER_TR] + m_Corner[CORNER_BR] ) / 2;
	int centralPoint = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;

	// ���̻� ������ �Ұ����ϸ� SubDivide() ����
	if ( ( m_Corner[CORNER_TR] - m_Corner[CORNER_TL] ) <= 1 )
	{
		return FALSE;
	}

	// 4���� �ڽĳ�� �߰�
	m_Child[CORNER_TL] = AddChild( m_Corner[CORNER_TL], topEdgeCenter, leftEdgeCenter, centralPoint );
	m_Child[CORNER_TR] = AddChild( topEdgeCenter, m_Corner[CORNER_TR], centralPoint, rightEdgeCenter );
	m_Child[CORNER_BL] = AddChild( leftEdgeCenter, centralPoint, m_Corner[CORNER_BL], bottomEdgeCenter );
	m_Child[CORNER_BR] = AddChild( centralPoint, rightEdgeCenter, bottomEdgeCenter, m_Corner[CORNER_BR] );

	return TRUE;
}

int QuadTree::GenTriIndex( int tris, LPVOID index, CUSTOMVERTEX* heightMap, Frustum* frustum, float ratioOfLOD )
{
	if ( m_Culled )
	{
		m_Culled = FALSE;
		return tris;
	}

	LPDWORD p = ( (LPDWORD)index ) + tris * 3;

	if ( IsVisible( heightMap, frustum->GetPos(), ratioOfLOD ) )
	{
		// ���� ������ ����� �κ� ����(SubDivide)�� �Ұ����ϹǷ� �׳� ����ϰ� ����
		if ( m_Corner[CORNER_TR] - m_Corner[CORNER_TL] <= 1 )
		{
			*p++ = m_Corner[0];
			*p++ = m_Corner[1];
			*p++ = m_Corner[2];
			tris++;
			*p++ = m_Corner[2];
			*p++ = m_Corner[1];
			*p++ = m_Corner[3];
			tris++;
			return tris;
		}

		BOOL	b[4] = { 0, 0, 0, 0 };
		// ��� �̿����(neightbor node)�� ��°����Ѱ�?
		if ( m_Neighbor[EDGE_UP] )
		{
			b[EDGE_UP] = m_Neighbor[EDGE_UP]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?
		if ( m_Neighbor[EDGE_DN] )
		{
			b[EDGE_DN] = m_Neighbor[EDGE_DN]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if ( m_Neighbor[EDGE_LT] )
		{
			b[EDGE_LT] = m_Neighbor[EDGE_LT]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if ( m_Neighbor[EDGE_RT] )
		{
			b[EDGE_RT] = m_Neighbor[EDGE_RT]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}

		// �̿������� ��δ� ��°����ϴٸ� ������� �̿������� ���� LOD�̹Ƿ� 
		// �κк����� �ʿ����.
		if ( b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT] )
		{
			*p++ = m_Corner[0];
			*p++ = m_Corner[1];
			*p++ = m_Corner[2];
			tris++;
			*p++ = m_Corner[2];
			*p++ = m_Corner[1];
			*p++ = m_Corner[3];
			tris++;
			return tris;
		}

		// ��� �κк����� �ʿ��Ѱ�?
		if ( !b[EDGE_UP] )
		{
			int n = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] ) / 2;

			*p++ = m_Center;
			*p++ = m_Corner[CORNER_TL];
			*p++ = n; 
			tris++;

			*p++ = m_Center;
			*p++ = n;
			*p++ = m_Corner[CORNER_TR];
			tris++;
		}
		else
		{
			*p++ = m_Center;
			*p++ = m_Corner[CORNER_TL];
			*p++ = m_Corner[CORNER_TR];
			tris++;
		}

		// �ϴ� �κк����� �ʿ��Ѱ�?
		if ( !b[EDGE_DN] )
		{
			int n = ( m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BR]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_BL]; 
			tris++;
		}
		else
		{
			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BR]; 
			*p++ = m_Corner[CORNER_BL]; 
			tris++;
		}

		if ( !b[EDGE_LT] ) // ���� �κк����� �ʿ��Ѱ�?
		{
			int n = ( m_Corner[CORNER_TL] + m_Corner[CORNER_BL] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BL]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_TL]; 
			tris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BL]; 
			*p++ = m_Corner[CORNER_TL]; 
			tris++;
		}

		if ( !b[EDGE_RT] ) // ���� �κк����� �ʿ��Ѱ�?
		{
			int n = ( m_Corner[CORNER_TR] + m_Corner[CORNER_BR] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_TR]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_BR]; 
			tris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = m_Center; *p++ = m_Corner[CORNER_TR]; *p++ = m_Corner[CORNER_BR]; tris++;
		}

		return tris;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	// �ڽ� ���� �˻�
	if ( m_Child[CORNER_TL] )
	{
		tris = m_Child[CORNER_TL]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_TR] )
	{
		tris = m_Child[CORNER_TR]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_BL] )
	{
		tris = m_Child[CORNER_BL]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_BR] )
	{
		tris = m_Child[CORNER_BR]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}

	return tris;
}

void QuadTree::Destroy()
{
	for ( int i = 0; i < 4; ++i )
	{
		SafeDelete( m_Child[i] );
	}
}

void QuadTree::AllIsInFrustum()
{
	m_Culled = FALSE;
	if ( !m_Child[0] )
	{
		return;
	}
	m_Child[0]->AllIsInFrustum();
	m_Child[1]->AllIsInFrustum();
	m_Child[2]->AllIsInFrustum();
	m_Child[3]->AllIsInFrustum();
}

int QuadTree::IsInFrustum( CUSTOMVERTEX* heightMap, Frustum* frustum )
{
	BOOL b[4] = { 0, };
	BOOL isInSphere = false;

	isInSphere = frustum->IsInSphere( (D3DXVECTOR3*)( heightMap + m_Center ), m_Radius );
	// ��豸 �ȿ� ������ �� ������ �������� �׽�Ʈ�� �ʿ�����Ƿ� ����
	if ( !isInSphere )
	{
		return FRUSTUM_OUT;
	}

	// ����Ʈ���� 4���� ��� �������� �׽�Ʈ
	b[0] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[0] ) );
	b[1] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[1] ) );
	b[2] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[2] ) );
	b[3] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[3] ) );

	// 4����� �������� �ȿ� ����
	if ( ( b[0] + b[1] + b[2] + b[3] ) == 4 )
	{
		return FRUSTUM_COMPLETELY_IN;
	}

	// �Ϻκ��� �������ҿ� �ִ� ���
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::FrustumCull( CUSTOMVERTEX* heightMap, Frustum* frustum )
{
	m_Culled = FALSE;

	int ret = IsInFrustum( heightMap, frustum );
	switch ( ret )
	{
		case FRUSTUM_COMPLETELY_IN:		// �������ҿ� ��������, ������� �˻� �ʿ� ����
			AllIsInFrustum();
			return;
		case FRUSTUM_PARTIALLY_IN:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
			m_Culled = FALSE;
			break;
		case FRUSTUM_OUT:				// �������ҿ��� �������, ������� �˻� �ʿ� ����
			m_Culled = TRUE;
			return;
	}

	if ( m_Child[0] )
	{
		m_Child[0]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[1] )
	{
		m_Child[1]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[2] )
	{
		m_Child[2]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[3] )
	{
		m_Child[3]->FrustumCull( heightMap, frustum );
	}
}

// �̿���带 �����.(�ﰢ�� ������ ������)
void QuadTree::BuildNeighborNode( QuadTree* rootNode, CUSTOMVERTEX* heightMap, int cx )
{
	int	n;
	int	_0, _1, _2, _3;

	for ( int i = 0; i < 4; ++i )
	{
		_0 = m_Corner[0];
		_1 = m_Corner[1];
		_2 = m_Corner[2];
		_3 = m_Corner[3];

		// �̿������ 4�� �ڳʰ��� ��´�.
		n = GetNodeIndex( i, cx, _0, _1, _2, _3 );
		// �ڳʰ����� �̿������ �����͸� ���´�.
		if ( n >= 0 )
		{
			m_Neighbor[i] = rootNode->FindNode( heightMap, _0, _1, _2, _3 );
		}
	}

	// ��� ȣ��
	if ( m_Child[0] )
	{
		m_Child[0]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[1]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[2]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[3]->BuildNeighborNode( rootNode, heightMap, cx );
	}
}

BOOL QuadTree::BuildQuadTree( CUSTOMVERTEX* heightMap )
{
	if ( SubDivide() )
	{
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = *( (D3DXVECTOR3*)( heightMap + m_Corner[CORNER_TL] ) ) -
			*( (D3DXVECTOR3*)( heightMap + m_Corner[CORNER_BR] ) );
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		m_Radius = D3DXVec3Length( &v ) / 2.0f;
		m_Child[CORNER_TL]->BuildQuadTree( heightMap );
		m_Child[CORNER_TR]->BuildQuadTree( heightMap );
		m_Child[CORNER_BL]->BuildQuadTree( heightMap );
		m_Child[CORNER_BR]->BuildQuadTree( heightMap );
	}
	return TRUE;
}

QuadTree* QuadTree::FindNode( CUSTOMVERTEX* heightMap, int _0, int _1, int _2, int _3 )
{
	QuadTree* p = nullptr;
	
	if ( ( m_Corner[0] == _0 ) && ( m_Corner[1] == _1 ) && ( m_Corner[2] == _2 ) && ( m_Corner[3] == _3 ) )
	{
		return this;
	}

	if ( m_Child[0] )
	{
		RECT	rc;
		POINT	pt;

		ZeroMemory( &rc, sizeof( RECT ) );
		ZeroMemory( &pt, sizeof( POINT ) );

		int n = ( _0 + _1 + _2 + _3 ) / 4;

		pt.x = (int)heightMap[n].m_VertexPoint.x;
		pt.y = (int)heightMap[n].m_VertexPoint.z;

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect( &rc, (int)heightMap[m_Child[0]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_BR]].m_VertexPoint.z );

		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[0]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[1]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[1]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[2]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[2]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[3]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[3]->FindNode( heightMap, _0, _1, _2, _3 );
		}
	}

	return NULL;
}

int QuadTree::GetNodeIndex( int ed, int cx, int& _0, int& _1, int& _2, int& _3 )
{
	int	n = 0;
	int _a = _0;
	int _b = _1; 
	int _c = _2;
	int _d = _3;
	int gap = _b - _a;	// ���� ����� �¿�����

	switch ( ed )
	{
		case EDGE_UP:	// ���� ���� �̿������ �ε���
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// ���� ���� �̿������ �ε���
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// ���� ���� �̿������ �ε���
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ( ( _0 / cx ) != ( _a / cx ) ) return -1;
			if ( ( _1 / cx ) != ( _b / cx ) ) return -1;
			break;
	}

	n = ( _0 + _1 + _2 + _3 ) / 4;	// ��� �ε���
	if ( !IS_IN_RANGE( n, 0, cx * cx - 1 ) ) return -1;

	return n;
}

// 2�� ������ �ϴ� ���� n�� �αװ��� ���Ѵ�.
int	Log2( int n )
{
	for ( int i = 1; i < 64; ++i )
	{
		n = n >> 1;
		if ( n == 1 )
		{
			return i;
		}
	}

	return 1;
}

// 2^n���� ���Ѵ�
int	Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// pt�� rc�ȿ� ���ԵǴ��� �˻��Ѵ�.(PtInRect()��� API�Լ��� ����(-)�� ó���� ���ߴ�.)
BOOL IsInRect( RECT* rc, POINT pt )
{
	if ( ( rc->left <= pt.x ) && ( pt.x <= rc->right ) &&
		 ( rc->bottom <= pt.y ) && ( pt.y <= rc->top ) )
		 return TRUE;

	return FALSE;
}
