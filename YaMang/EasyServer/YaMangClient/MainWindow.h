﻿#pragma once

#include "MacroSet.h"

static const WCHAR WINDOW_NAME[] = L"YaMang";

class GameManager;

template <class DERIVED_TYPE>
class BaseWindow: public Singleton<DERIVED_TYPE>
{
public:
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		DERIVED_TYPE * pThis = NULL;

		if ( uMsg == WM_NCCREATE )
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)pThis );

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr( hwnd, GWLP_USERDATA );
		}

		if ( pThis )
		{
			return pThis->HandleMessage( uMsg, wParam, lParam );
		}
		else
		{
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
	}

	BaseWindow(): m_hwnd( nullptr ) {}

	BOOL Create( PCWSTR lpWindowName,
				 DWORD dwStyle,
				 DWORD dwExStyle = 0,
				 int x = CW_USEDEFAULT,
				 int y = CW_USEDEFAULT,
				 int nWidth = DERIVED_TYPE::WINDOW_WIDTH,
				 int nHeight = DERIVED_TYPE::WINDOW_HEIGHT,
				 HWND hWndParent = 0,
				 HMENU hMenu = 0
				 )
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle( NULL );
		wc.lpszClassName = ClassName();

		RegisterClass( &wc );

		m_hwnd = CreateWindowEx( dwExStyle, ClassName(), lpWindowName, dwStyle,
								 x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this );

		return ( m_hwnd ? TRUE : FALSE );
	}

	HWND Window() const { return m_hwnd; }
	BOOL ShowWindow( int nCmdShow ) const { return ShowWindow( Window(), nCmdShow ); }

	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

protected:
	virtual PCWSTR	ClassName() const = 0;
	virtual LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND m_hwnd;
};

class MainWindow: public BaseWindow<MainWindow>
{
public:
	MainWindow();
	virtual ~MainWindow();

	PCWSTR	ClassName() const { return WINDOW_NAME; }
	LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	int RunGame();

private:
	MainWindow( const MainWindow & );
	MainWindow & operator=( const MainWindow & );

	GameManager* m_GameManager;
};

