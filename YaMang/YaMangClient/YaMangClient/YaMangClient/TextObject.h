﻿#pragma once
#include "Interface3D.h"

class TextObject//: public Interface3D
{
public:
	TextObject( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom );
	~TextObject();

	void	DrawText();

	void	SetText( LPCWSTR text ) { m_Text.clear(); m_Text.append( text ); }
	void	SetLeft( float left ) { m_Left = left; }
	void	SetTop( float top ) { m_Top = top; }

private:
	std::wstring	m_Text;
	float			m_Left = 0;
	float			m_Top = 0;
	int				m_RGB_R = 255;
	int				m_RGB_G = 255;
	int				m_RGB_B = 255;
	float			m_Right = 0;
	float			m_Bottom = 0;

};

