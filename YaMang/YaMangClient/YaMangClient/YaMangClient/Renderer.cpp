#include "stdafx.h"
#include "../../yaMangDxDll/yaMangDxDll/yaMangDxDll.h" // agebreak : �̷��� ��� ��η� ���� ����, ������ Include ������ �߰� ��Ű�� ���� ����.
#include "Renderer.h"
#include "MainWindow.h"

extern float posX;
extern float posY;
extern float posZ;

Renderer::Renderer(): m_Result( false ), m_IsReady( false )
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	if ( S_OK == InitD3D( MainWindow::GetInstance()->Window() ) )
	{
		m_Result = true;
	}

	return m_Result;
}

void Renderer::Destroy()
{
	if ( m_Result )
	{
		D3DCleanUp();
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_IsReady = PreRendering();
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		PostRendering();
		m_IsReady = false;
	}
}

void Renderer::Render( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{
		Rendering( mesh, posX, posY, posZ );
	}
}

bool Renderer::CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitGeometry( MainWindow::GetInstance()->Window(), fileName, mesh ) )
	{
		return true;
	}
	return false;
}

void Renderer::DeleteMesh( MESHOBJECT* mesh )
{
	if ( mesh )
	{
		MeshObjectCleanUp( mesh );
	}
}
