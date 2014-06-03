#pragma once

// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	BOOL			Make( D3DXMATRIXA16* viewProjMatrix );

	BOOL			IsIn( D3DXVECTOR3* point );
	BOOL			IsInSphere( D3DXVECTOR3* point, float radius );

	D3DXVECTOR3*	GetPos() { return &m_ViewerPosition; }

private:
	D3DXVECTOR3		m_Vertex[8];		// ���������� ������ ���� 8��
	D3DXVECTOR3		m_ViewerPosition;	// ���� ī�޶��� ������ǥ
	D3DXPLANE		m_Plane[6];			// ���������� �����ϴ� 6���� ���
};