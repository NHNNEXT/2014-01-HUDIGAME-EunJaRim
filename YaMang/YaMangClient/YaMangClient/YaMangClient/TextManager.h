#pragma once
#include "MacroSet.h"

class TextObject;
class TextManager:public Singleton<TextManager>
{
public:
	TextManager();
	~TextManager();

	void	RegistText( UINT key, LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );
	void	DeleteText( UINT key );

	void	DrawTexts();

private:
	// enum���� �ؽ�Ʈ ������ �̰� �ű�� �迭�� �ϴ°� ������ �����ѵ� ������ ���ϰ� �ְ� ���⿡�� map���°� ������
	typedef std::map<UINT, TextObject*> TextList;
	TextList			m_TextList;
};
