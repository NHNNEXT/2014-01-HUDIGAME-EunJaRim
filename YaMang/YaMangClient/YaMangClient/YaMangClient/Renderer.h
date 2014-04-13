﻿#pragma once

#include "MacroSet.h"

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

	void	Render( MESHOBJECT* mesh );
	void	RenderMap();
	
	bool	CreateMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName );
	void	DeleteMap();

	bool	CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh );
	void	DeleteMesh( MESHOBJECT* mesh );
	
	void	SetCamera();

private:
	bool	m_Result = false;
	bool	m_IsReady = false;
	bool	m_IsMapReady = false;
};