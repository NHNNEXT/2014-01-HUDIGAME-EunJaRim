#pragma once

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

// ����Ʈ���� �����Ǵ� 4���� �ڳʰ��� ���� �����
enum CornerType
{
	CORNER_TL, 
	CORNER_TR, 
	CORNER_BL, 
	CORNER_BR
};

// �̿���� ó���� �����
enum NeighborType
{
	EDGE_UP,
	EDGE_DN,
	EDGE_LT,
	EDGE_RT
};

// ����Ʈ���� �������Ұ��� ����
enum QuadLocation
{
	FRUSTUM_OUT = 0,			// �������ҿ��� ���� ���
	FRUSTUM_PARTIALLY_IN = 1,	// �������ҿ� �κ� ����
	FRUSTUM_COMPLETELY_IN = 2,	// �������ҿ� ���� ����
	FRUSTUM_UNKNOWN = -1
};

struct CUSTOMVERTEX
{
	D3DXVECTOR3	m_VertexPoint = { 0, 0, 0 };
	DWORD		m_Diffuse = 0;
	D3DXVECTOR2	m_VertexTexturePoint = { 0, 0 };
};

class Frustum;

class QuadTree
{
public:
	// ���� ��� ������
	QuadTree( int cx, int cy );
	// ���� ��� ������
	QuadTree( QuadTree* parent );
	~QuadTree();

	BOOL		IsCulled() { return m_Culled; }
	BOOL		IsVisible( CUSTOMVERTEX* heightMap, D3DXVECTOR3* cameraPosition, float ratioOfLOD ) const
	{
		return ( ( m_Corner[CORNER_TR] - m_Corner[CORNER_TL] ) <= GetLODLevel( heightMap, cameraPosition, ratioOfLOD ) );
	}
	
	BOOL		Build( CUSTOMVERTEX* heightMap );
	void		GetCorner( int& _0, int& _1, int& _2, int& _3 ) const
	{
		_0 = m_Corner[0]; _1 = m_Corner[1]; _2 = m_Corner[2]; _3 = m_Corner[3];
	}
	int			GenerateIndex( LPVOID index, CUSTOMVERTEX* heightMap, Frustum* frustum, float ratioOfLOD );
	
private:
	QuadTree();
	
	QuadTree*	AddChild( int topLeft, int topRight, int bottomLeft, int bottomRight );
	BOOL		SetCorners( int topLeft, int topRight, int bottomLeft, int bottomRight );
	BOOL		SubDivide();
	int			GenTriIndex( int tris, LPVOID index, CUSTOMVERTEX* heightMap,
							 Frustum* frustum, float ratioOfLOD );
	void		Destroy();

	float		GetDistance( D3DXVECTOR3* vector1, D3DXVECTOR3* vector2 ) const
	{
		return D3DXVec3Length( &( *vector2 - *vector1 ) );
	}

	int			GetLODLevel( CUSTOMVERTEX* heightMap, D3DXVECTOR3* cameraPosition, float ratioOfLOD ) const
	{
		float distance = GetDistance( (D3DXVECTOR3*)( heightMap + m_Center ), cameraPosition );
		return __max( (int)( distance * ratioOfLOD ), 1 );
	}

	void		AllIsInFrustum();
	int			IsInFrustum( CUSTOMVERTEX* heightMap, Frustum* frustum );
	void		FrustumCull( CUSTOMVERTEX* heightMap, Frustum* frustum );
	void		BuildNeighborNode( QuadTree* rootNode, CUSTOMVERTEX* heightMap, int cx );
	BOOL		BuildQuadTree( CUSTOMVERTEX* heightMap );
	QuadTree*	FindNode( CUSTOMVERTEX* heightMap, int _0, int _1, int _2, int _3 );
	int			GetNodeIndex( int ed, int cx, int& _0, int& _1, int& _2, int& _3 );

	
	QuadTree*	m_Child[4];

	QuadTree*	m_Parent;			// Triangle Crack�� ���� ���� ����Ѵ�.
	QuadTree*	m_Neighbor[4];		// Triangle Crack�� ���� ���� ����Ѵ�.

	int			m_Center;
	int			m_Corner[4];
	
	BOOL		m_Culled = false;
	float		m_Radius = 0;
};

// 2�� ������ �ϴ� ���� n�� �αװ��� ���Ѵ�.
int	Log2( int n );

// 2^n���� ���Ѵ�
int	Pow2( int n );

// pt�� rc�ȿ� ���ԵǴ��� �˻��Ѵ�.(PtInRect()��� API�Լ��� ����(-)�� ó���� ���ߴ�.)
BOOL IsInRect( RECT* rc, POINT pt );
