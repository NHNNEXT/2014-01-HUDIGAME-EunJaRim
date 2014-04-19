﻿#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "ResourceManager.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	if ( S_OK == InitD3D( MainWindow::GetInstance( )->Window( ) ) )
	{
		m_Result = true;

		RECT rect;
		GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.left;

		m_PrevTime = timeGetTime();
	}

	return m_Result;
}

void Renderer::Destroy()
{
	if ( m_Result )
	{
		D3DCleanUp( );
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_IsReady = PreRendering( );
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		UINT nowTime = timeGetTime();
		if ( m_PrevTime + 1000 < nowTime )
		{
			m_PrevTime = nowTime;
			m_FPS = m_Frame;
			m_Frame = 0;
		}

		wchar_t ws[100] = { 0, };
		wsprintf( ws, L"FPS : %d", m_FPS );
		RenderText( ws, 20, 40 );
		m_Frame++;

		PostRendering( );
		m_IsReady = false;
	}
}

void Renderer::RenderMesh( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{	
		Rendering( mesh );
	}
}

void Renderer::RenderMap()
{
	if ( ResourceManager::GetInstance()->IsMapReady() )
	{
		HeightMapRender( );
	}
}

void Renderer::SetViewMatrix( D3DXMATRIXA16& matrix )
{
	SetMatrix( &matrix, true );
}

void Renderer::SetWorldMatrix( D3DXMATRIXA16& matrix )
{
	SetMatrix( &matrix, false );
}

void Renderer::ResizeWindow( LONG width, LONG height )
{
	if ( m_Result )
	{
		SetAspectRatio( width, height );
	}
}

void Renderer::WriteText( LPCWSTR text, float left, float top, int RGB_R /*= 255*/, int RGB_G /*= 255*/, int RGB_B /*= 255*/, float right /*= 0*/, float bottom /*= 0 */ )
{
	if ( m_Result )
	{
		RenderText( text, left, top, RGB_R, RGB_G, RGB_B, right, bottom );
	}
}

D3DVIEWPORT9 Renderer::GetViewPort()
{
	D3DVIEWPORT9	viewPort;
	GetViewPort9( &viewPort );

	return viewPort;
}

D3DXMATRIXA16 Renderer::GetProjMatrix()
{
	D3DXMATRIXA16	projMatrix;
	GetD3DProjMatrix( &projMatrix );

	return projMatrix;
}

D3DXMATRIXA16 Renderer::GetViewMatrix()
{
	D3DXMATRIXA16	viewMatrix;
	GetD3DViewMatrix( &viewMatrix );

	return viewMatrix;
}


