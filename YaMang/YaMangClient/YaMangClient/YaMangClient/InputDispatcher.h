﻿#pragma once
#include "MacroSet.h"
#include "KeyInput.h"

class InputDispatcher : public Singleton<InputDispatcher>
{
public:
	InputDispatcher();
	~InputDispatcher();

	void	EventKeyInput( KeyInput key );
	void	DispatchKeyInput();

	bool	IsPressed( KeyInput key ) { return IsPressed( key.GetKeyValue() ); }
	bool	IsPressed( char key ) { return m_IsKeyPressed[key]; }

private:
	std::list<KeyInput>			m_KeyInputList;
	std::array<bool, MAX_KEY>	m_IsKeyPressed;
};