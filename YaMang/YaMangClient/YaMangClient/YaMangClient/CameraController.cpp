#include "stdafx.h"
#include "CameraController.h"
#include "InputDispatcher.h"
#include "Renderer.h"

CameraController* CameraController::m_Instance = nullptr;

CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

void CameraController::Init()
{
// 	m_EyePoint = { 0.0f, 50.0f, 0.0f };
// 	m_LookAtPoint = { 0.0f, 49.8f, -1.0f };

	m_EyePoint = { 0, 0, 0 };
	m_LookAtPoint = { 0, -0.01f, 1.0f };
	
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::MoveForward( float speed )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVec3Normalize( &view, &view );
	m_EyePoint += view * speed;
	m_LookAtPoint += view * speed;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::MoveSide( float speed )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross, &m_UpVector, &view );
	m_EyePoint += cross * speed;
	m_LookAtPoint += cross * speed;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::RotateUp( float angle )
{
	D3DXVECTOR3 preView = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 view = { 0, 0, 0 };
	D3DXVECTOR3 axis = { 0, 0, 0 };

	D3DXVec3Cross( &axis, &m_UpVector, &preView );
	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &preView, &rotateMatrix );

	if ( preView.x * view.x < 0 || preView.y * view.y < 0 || preView.z * view.z < 0 )
	{
		// ȸ������ ������ ��
		return;
	}
	m_LookAtPoint = m_EyePoint + view;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::RotateSide( float angle )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 axis = { 0, 1, 0 };
	// ȸ����

	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &view, &rotateMatrix );
	m_LookAtPoint = m_EyePoint + view;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}
