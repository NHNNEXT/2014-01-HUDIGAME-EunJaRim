﻿#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

class UIObject;

class UIManager :public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void		Init();
	void		Destroy();

	void		Update();
	void		Render() const;

	void		AddUIObject( UISpriteKeyType key, int PosX, int PosY, bool visible);

private:
	std::list<UIObject*> m_UIList;
};

