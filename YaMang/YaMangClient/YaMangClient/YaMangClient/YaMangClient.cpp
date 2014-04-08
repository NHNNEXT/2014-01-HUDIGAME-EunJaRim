// YaMangClient.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "YaMangClient.h"

#include <stdio.h>
#include <time.h>

#include "Logger.h"
#include "MainWindow.h"

#pragma comment(lib,"ws2_32.lib")

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	Logger::GetInstance();

	MainWindow* window = MainWindow::GetInstance();

	if ( false == window->Create( L"YaMang", WS_POPUPWINDOW, MAKEINTRESOURCE( IDC_YAMANGCLIENT ) ) )
	{
		return 0;
	}

	window->Display( nCmdShow );

	int result = window->RunGame();
	Logger::Release();

	return result;
}


//////////////////////////////////////////////////////////////////////////
// �� ���� MainWindow�� HandleMessage ������ �쿩�� ��
//
// ������ ���� �� �ؾ� �ȴ�!
//////////////////////////////////////////////////////////////////////////

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
/*
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( message )
	{


		case WM_CREATE:
		{
			// Create a push button
			CreateWindow( L"BUTTON", L"CONNECT", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  10, 10, 175, 23, hWnd, (HMENU)IDC_SEND_BUTTON, GetModuleHandle( NULL ), NULL );

			// ���� ���� �ʱ�ȭ �۾�
			//////////////////////////////////////////////////////////////////////////
			// ��Ʈ��ũ �Ŵ������� ���� ��
			if ( false == Initialize() )
			{
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			// http://blog.naver.com/alsduddl525/140123329159
			// http://blog.naver.com/merds/150000138666
			// http://blog.naver.com/popssong/70133058993 ����
			//
			// g_Socket �� ���� �͵��� WM_SOCKET �̺�Ʈ�� ó���ϰ���
			//////////////////////////////////////////////////////////////////////////
			int	nResult = WSAAsyncSelect( g_Socket, hWnd, WM_SOCKET, ( FD_CLOSE | FD_CONNECT ) );
			if ( nResult )
			{
				MessageBox( hWnd, L"WSAAsyncSelect failed", L"Critical Error", MB_ICONERROR );
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

		}
			break;

		case WM_TIMER:
		{
			/// �ֱ������� ä�� ��������.

			ChatBroadcastRequest sendData;

			sendData.mPlayerId = g_MyClientId;

			/// ���� ���ڿ��� ä������ ������
			char* buff = sendData.mChat;
			for ( int i = 0; i < 300; ++i )
			{
				sendData.mChat[i] = (char)( 65 + ( rand() % 26 ) );
			}
			sendData.mChat[300] = '\0';

			if ( g_SendBuffer.Write( (const char*)&sendData, sendData.mSize ) )
				// sendData.mSize = sizeof(ChatBroadcastRequest);
			{
				PostMessage( hWnd, WM_SOCKET, wParam, FD_WRITE );
				//////////////////////////////////////////////////////////////////////////
				// http://blog.naver.com/gkqxhq324456/110177315036 ����
				//
				// ä���� �������� ���ۿ� �����͵� �־� �ξ�����, WM_SOCKET �̺�Ʈ�� �߻���Ű��
				//////////////////////////////////////////////////////////////////////////
			}


		}
			break;

		case WM_COMMAND:
		{
			wmId = LOWORD( wParam );
			wmEvent = HIWORD( wParam );
			// �޴� ������ ���� �м��մϴ�.

			switch ( wmId )
			{
				case IDC_SEND_BUTTON:
				{
					if ( !g_LoginComplete && !Connect( szServer, nPort ) )
					{
						SendMessage( hWnd, WM_DESTROY, NULL, NULL );
						break;
					}

				}
					break;
				default:
					return DefWindowProc( hWnd, message, wParam, lParam );
			}
		}
			break;

			//////////////////////////////////////////////////////////////////////////
			// ���� �̺�Ʈ
			//////////////////////////////////////////////////////////////////////////
		case WM_SOCKET:
		{
			// lParam �� �������� ���� �غ���
			if ( WSAGETSELECTERROR( lParam ) )
			{
				MessageBox( hWnd, L"WSAGETSELECTERROR", L"Error", MB_OK | MB_ICONERROR );
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

			// ���� �ƴϸ� �̺�Ʈ �����ؼ� switch
			switch ( WSAGETSELECTEVENT( lParam ) )
			{
				case FD_CONNECT:
					// ������ �Ǿ���
				{
					/// NAGLE ����
					int opt = 1;
					::setsockopt( g_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof( int ) );

					srand( static_cast<unsigned int> ( time( NULL ) ) );
					/// �뷫 1000~1100 �� ID�� �α��� �غ���
					LoginRequest sendData;
					sendData.mPlayerId = 1000 + rand() % 101;

					if ( g_SendBuffer.Write( (const char*)&sendData, sendData.mSize ) )
					{
						PostMessage( hWnd, WM_SOCKET, wParam, FD_WRITE );
					}


					int nResult = WSAAsyncSelect( g_Socket, hWnd, WM_SOCKET, ( FD_CLOSE | FD_READ | FD_WRITE ) );
					if ( nResult )
					{
						assert( false );
						break;
					}
				}
					break;

				case FD_READ:
				{
					char inBuf[4096] = { 0, };

					int recvLen = recv( g_Socket, inBuf, 4096, 0 );
					// send() �Լ��� �ݴ�

					// ���Ͽ��� �о�� �����͸� �ϴ� ���ۿ� ����
					if ( !g_RecvBuffer.Write( inBuf, recvLen ) )
					{
						/// ���� ��á��.
						assert( false );
					}

					ProcessPacket( hWnd );
					//////////////////////////////////////////////////////////////////////////
					// ��Ŷ �ڵ鸵!
					//////////////////////////////////////////////////////////////////////////
				}
					break;

					//////////////////////////////////////////////////////////////////////////
					// �����͸� ������ -> ���ۿ� �� �Ŀ�, �ڵ鸵 �ϴ� �ʿ��� ���� ������ �̾Ƽ� ó��
					//
					// �����͸� ������ -> ���ۿ� �� �Ŀ�, FD_WRITE �ʿ��� ���� ������ �̾Ƽ� send()
					//////////////////////////////////////////////////////////////////////////

				case FD_WRITE:
				{
					/// ������ ���ۿ� �ִ°͵� ������ ������
					int size = g_SendBuffer.GetCurrentSize();
					if ( size > 0 )
					{
						char* data = new char[size];
						g_SendBuffer.Peek( data );

						int sent = send( g_Socket, data, size, 0 );

						/// �ٸ��� �ִ�
						if ( sent != size )
							OutputDebugStringA( "sent != request\n" );

						// ���� �����ʹ� ������
						g_SendBuffer.Consume( sent );

						delete[] data;
					}

				}
					break;

				case FD_CLOSE:
				{
					MessageBox( hWnd, L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
					closesocket( g_Socket );
					SendMessage( hWnd, WM_DESTROY, NULL, NULL );

					// ����� ��Ʈ��ũ �Ŵ������� Destroy �ϰ� �Ŵ����� Release �ϵ��� �Ѵ�
				}
					break;
			}
		}
			break;

		case WM_DISPLAYCHANGE:
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
			EndPaint( hWnd, &ps );
			break;

		case WM_SIZE:
		{
			int x = (int)LOWORD( lParam );
			int y = (int)HIWORD( lParam );
			// m_scene.Resize( x, y );
			InvalidateRect( hWnd, NULL, FALSE );
		}
			return 0;

		case WM_ERASEBKGND:
			return 1;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

*/