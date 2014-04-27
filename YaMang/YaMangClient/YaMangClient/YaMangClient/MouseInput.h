﻿#pragma once

enum class MouseStatus
{
	MOUSE_NONE,
	MOUSE_DOWN,
	MOUSE_PRESSED,
	MOUSE_UP,
};

class MouseInput
{
public:
	MouseInput();
	~MouseInput();

	COORD			GetMousePosition() { return m_MousePosition; }
	void			SetMousePosition(COORD Position) { m_MousePosition = Position; }
	void			SetMousePosition(int xPos, int yPos) { m_xPos = xPos; m_yPos = yPos; }
	MouseStatus		GetMouseStatus() { return m_MouseStatus; }
	void			SetMouseStatus(MouseStatus mouseStatus) { m_MouseStatus = mouseStatus; }
	void			MoveMousePosition(int x, int y) { m_xPos += x; m_yPos += y; }

private:
	int				m_xPos = 200;
	int				m_yPos = 200;
	COORD			m_MousePosition;
	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;

};

