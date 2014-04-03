#pragma once


#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define MAX_STRING		128

#define BUF_SIZE		32
#define SERVER_PORT_NUM	9000


template <class DERIVED_TYPE>
class BaseWindow
{
public:
	
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		DERIVED_TYPE *pThis = NULL;

		// �ʱ� ���� ��
		if ( uMsg == WM_NCCREATE )
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;

			pThis = (DERIVED_TYPE*) pCreate->lpCreateParams;

			SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
			// �ϴ� ������� ���� �� ���� �����صα�
		
			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*) GetWindowLongPtr( hwnd, GWLP_USERDATA );
			// ������ ����
		}
		if ( pThis )
		{
			// â�� �� ������ �޽��� �ڵ�
			return pThis->HandleMessage( uMsg, wParam, lParam );
		}
		else
		{
			// â�� �� �� ������ DefWindowProc()�� �޽��� �ڵ鸵�� �ѱ�
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
	}

	BaseWindow(): m_hwnd( NULL ) {}

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = WINDOW_WIDTH,
		int nHeight = WINDOW_HEIGHT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle( NULL );
		wc.lpszClassName = ClassName();

		wc.hCursor = LoadCursor( NULL, IDC_ARROW );
		// ��� �߰��ص�

		RegisterClass( &wc );

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this
			);

		
		return ( m_hwnd ? TRUE : FALSE );
	}

	HWND Window() const { return m_hwnd; }

protected:

	// ���� ���� �Լ� ���� �� ��
	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND m_hwnd;
};


class MainWindow: public BaseWindow<MainWindow>
	// ���ø����� ���� �Ǿ� �ִ� BaseWindow�� ��� �޾Ƽ� MainWindow ����
{
//	Scene   m_scene;
// �������� �پ�� �� �κ���

public:
	// �Ʒ� �� ���� ���� ���� �Լ������Ƿ� �ݵ�� ���� �ؾ� ��
	PCWSTR  ClassName() const { return L"P2P Demo"; }
	LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
};