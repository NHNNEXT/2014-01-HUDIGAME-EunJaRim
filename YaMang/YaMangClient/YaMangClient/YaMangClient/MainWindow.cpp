﻿#include "stdafx.h"
#include "MainWindow.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "Renderer.h"
#include "InputDispatcher.h"

MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}

LRESULT MainWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case WM_CREATE:
		{
			SetTimer( MainWindow::GetInstance()->Window(), 337, 20, NULL );
		}
			return 0;

		case WM_SOCKET:
		{
			NetworkManager::GetInstance()->HandleMessage( wParam, lParam );
		}
			return 0;

		case WM_SIZE:
		{
			RECT rect;
			GetWindowRect( m_HandleOfWindow, &rect );
			
			LONG width = static_cast<LONG>( LOWORD( lParam ) );
			LONG height = static_cast<LONG>( HIWORD( lParam ) );
			
			Renderer::GetInstance()->ResizeWindow( width, height );
			InvalidateRect( m_HandleOfWindow, NULL, FALSE );

			Log( " 사이즈 : %d %d \n", width, height );
		}
			return 0;

		case WM_DESTROY:
		{	
			if ( GameManager::GetInstance()->Process() )
			{
				// 창이 강제 제거 되었을 때(창 닫기 등)
				// 여기서 이 이벤트를 발생시켜서 RunGame()의 루프에서 빠져나오게 해야
				// 안전하게 리소스가 해제 되면서 메모리 릭을 피할 수 있다.
				GameManager::GetInstance()->Stop();
			}
			PostQuitMessage( 0 );
		}
			return 0;

		/*
		case WM_MOUSEMOVE:
		{
			MouseX = LOWORD( IParam );
			MouseY = HIWORD( IParam );
		}
			break;
		*/

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			KeyInput key;
			key.SetKeyValue( static_cast<unsigned char>( wParam ) );
			key.SetKeyStatus( KeyStatus::KEY_DOWN );
			if ( InputDispatcher::GetInstance()->IsPressed( key ) )
			{
				key.SetKeyStatus( KeyStatus::KEY_PRESSED );
			}

			InputDispatcher::GetInstance()->EventKeyInput( key );
			InputDispatcher::GetInstance()->DispatchKeyInput();
		}
			return 0;

		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			KeyInput key;
			key.SetKeyValue( static_cast<unsigned char>( wParam ) );
			key.SetKeyStatus( KeyStatus::KEY_UP );

			InputDispatcher::GetInstance()->EventKeyInput( key );
			InputDispatcher::GetInstance()->DispatchKeyInput();
		}
			return 0;

		case WM_ERASEBKGND:
			return 1;

		default:
			return DefWindowProc( m_HandleOfWindow, uMsg, wParam, lParam );
	}

	return 0;
}

int MainWindow::RunGame()
{
	MSG msg = {0,};

	if ( false == GameManager::GetInstance()->Init() )
	{
		// 사실 여기 이렇게 그냥 두었지만,
		// Init() 함수 내부에서 방어 코드 종류별로 케바케로 실패 뜬 부분 전부 잡아서 Release() 해줘야 됨.
		GameManager::GetInstance()->Destroy();
		GameManager::Release();
				
		MessageBox( m_HandleOfWindow, L"Gama Manager Initialization Error", L"Game Manager Init Error!", MB_ICONINFORMATION | MB_OK );
		SendMessage( m_HandleOfWindow, WM_DESTROY, NULL, NULL );
		return false;
	}

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( false == GameManager::GetInstance()->Process() )
		{
			GameManager::GetInstance()->Destroy();
			GameManager::Release();
			PostQuitMessage( 0 );
			break;
		}
	}

	return (int)msg.wParam;
}

BOOL MainWindow::Create( PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle /*= 0*/, int x /*= CW_USEDEFAULT*/, int y /*= CW_USEDEFAULT*/, int nWidth /*= WINDOW_WIDTH*/, int nHeight /*= WINDOW_HEIGHT*/, HWND hWndParent /*= 0*/, HMENU hMenu /*= 0 */ )
{
	WNDCLASS wc = { 0 };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle( NULL );
	wc.lpszClassName = ClassName();

	wc.hCursor = LoadCursor( NULL, IDC_ARROW );

	RegisterClass( &wc );

	m_HandleOfWindow = CreateWindowEx( dwExStyle, ClassName(), lpWindowName, dwStyle,
									   x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this );

	return ( m_HandleOfWindow ? TRUE : FALSE );
}

LRESULT CALLBACK MainWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	MainWindow * pThis = NULL;

	if ( uMsg == WM_NCCREATE )
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (MainWindow*)pCreate->lpCreateParams;
		SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)pThis );

		pThis->m_HandleOfWindow = hwnd;
	}
	else
	{
		pThis = (MainWindow*)GetWindowLongPtr( hwnd, GWLP_USERDATA );
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