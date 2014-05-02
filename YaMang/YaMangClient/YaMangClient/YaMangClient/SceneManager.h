﻿#pragma once
#include "EnumSet.h"
#include "MacroSet.h"

class Scene;

class SceneManager:public Singleton<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void	Init();
	void	Destroy();
	void	ChangeScene( SceneType scene );
	void	Update() const;
	void	Render() const;

	Scene*	GetNowScene() const { return m_NowScene; }

private:
	std::array<Scene*, SCENE_MAX>		m_SceneList;
	Scene*	m_NowScene = nullptr;
};

