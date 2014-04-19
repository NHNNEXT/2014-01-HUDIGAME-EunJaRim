﻿#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

struct MESHOBJECT;

class Renderer: public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	bool	Init();
	void	Destroy();
	void	RenderBegin();
	void	RenderEnd();

	void	RenderMesh( MESHOBJECT* mesh );
	void	RenderMap();
	void	ResizeWindow( LONG width, LONG height );
	
	bool	CreateMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName );
	void	DeleteMap();

	bool	CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh );
	void	DeleteMesh( MESHOBJECT* mesh );
	
	void	SetCamera( D3DXMATRIXA16& matrix );
	
	D3DVIEWPORT9	GetViewPort();
	D3DXMATRIXA16	GetProjMatrix();
	D3DXMATRIXA16	GetViewMatrix();

	void	WriteText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );

private:
	bool	m_Result = false;
	bool	m_IsReady = false;
	bool	m_IsMapReady = false;
	
	UINT	m_PrevTime = 0;
	UINT	m_Frame = 0;
	UINT	m_FPS = 0;
};