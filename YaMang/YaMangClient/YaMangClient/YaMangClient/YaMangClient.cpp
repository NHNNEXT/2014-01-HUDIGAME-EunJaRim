﻿// YaMangClient.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "YaMangClient.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif // DEBUG

#include "Logger.h"
#include "MainWindow.h"
#include "NetworkManager.h"
#include "Exception.h"

#pragma comment(lib,"ws2_32.lib")


int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// 메모리 릭을 체크하려면 아래의 #### 칸에 릭 난 곳 { 숫자 } 표기 된 숫자를 넣어주면 됩니다.
	// _CrtSetBreakAlloc( #### );
	// _CrtSetBreakAlloc( 192 );

	SetUnhandledExceptionFilter( ExceptionFilter );

#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	Log( "!!!:%S \n", lpCmdLine );

	// test code
	std::wstring parameter = lpCmdLine;
	if ( 0 == parameter.size() )
	{
		NetworkManager::GetInstance()->SetRoomNumber( 1 );
	}
	else
	{
		NetworkManager::GetInstance()->SetRoomNumber( std::stoi( parameter ) );
	}
	

	// WS_POPUPWINDOW
	// WS_OVERLAPPEDWINDOW
	if ( false == MainWindow::GetInstance()->Create( L"YaMang", WS_OVERLAPPEDWINDOW ) )
	{
		return 0;
	}

	MainWindow::GetInstance()->Display( nCmdShow );
	

#ifndef DEBUG
	HANDLE mutex = CreateMutex( NULL, FALSE, L"YaMangClientMutex" );
	if ( mutex == NULL )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Mutex Error", L"Mutex", MB_OK | MB_ICONERROR );
		return -1;
	}

	DWORD waitResult = WaitForSingleObject( mutex, 1 );
	switch ( waitResult )
	{
		case WAIT_TIMEOUT:
		{	
			MessageBox( MainWindow::GetInstance()->Window(), L"Client Already Running", L"Mutex", MB_OK | MB_ICONERROR );
			MainWindow::Release();
			return -1;
		}
			break;
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
		default:
			break;
	}
#endif

	int result = MainWindow::GetInstance()->RunGame();
	MainWindow::Release();

#ifdef _PRINT_CONSOLE
	Logger::Release();
#endif

	return result;
}