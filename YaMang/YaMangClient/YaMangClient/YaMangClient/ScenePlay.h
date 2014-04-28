﻿#pragma once
#include "Scene.h"
#include <d3dx9math.h>

class Corps;
class MouseRender;

class ScenePlay:
	public Scene
{
public:
	ScenePlay();
	~ScenePlay();

	void	Create();
	void	Destroy();
	void	Update();
	void	Render();

	void	AddCorps( int corpsID, Corps* corps );
	void	ChangeCorpsFormation( int corpsID, FormationType formationType );
	void	MoveCorpsStart( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector, float speed );
	void	MoveCorpsStop( int corpsID );

private:
	std::hash_map<int, Corps*>	m_CorpsList;

	MouseRender*	m_MouseCursor;
};

