#include "UserInterface.h"


// callback-> very, very, very important
LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	UserInterface dir;
	static int x, y;

	// here procces all msg

	switch (_msg)
	{
	case WM_KEYDOWN:
	{
		Beep(0, 0);
		switch(_wparam)
		{
			case VK_RETURN:
				{
					MessageBox(_hwnd, L"WTF!!!"/*message in the window*/, L"DZIALA!!!!!!!!!!!!!!!!!!!!!!!"/*title of the window*/, MB_ICONINFORMATION);
				}
			break;
			case 0x51:
				/// keys from a-z from VK(virtual keys) must be use in hex numbers.
				{
					LONG_PTR currentStyle = GetWindowLongPtr(_hwnd, GWL_STYLE);

					static bool alternateAppearance = false;

					if (alternateAppearance)
					{
						SetWindowLongPtr(_hwnd, GWL_STYLE, currentStyle | WS_OVERLAPPEDWINDOW);
					}
					else {

						SetWindowLongPtr(_hwnd, GWL_STYLE, currentStyle & ~WS_OVERLAPPEDWINDOW);
					}

					SetWindowPos(_hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

					alternateAppearance = !alternateAppearance;
				}
			break;
			case VK_ESCAPE:
				{
					if (MessageBox(_hwnd, L"Really quit?", L"Exit app", MB_OKCANCEL) == IDOK)
					{

						DestroyWindow(_hwnd);
					}
				}
			break;
			case VK_DELETE:
				{	

				CreateWindowW(L"BUTTON", L"New canvase", WS_VISIBLE | WS_CHILD, x, y, 150, 20, _hwnd, (HMENU)1 , NULL, NULL);
					x += 10;
					y += 10;
				}
			break;
		
		}
	}
	break;
	case WM_COMMAND:
	{
		if (LOWORD(_wparam) == 1)
		{
			HWND but = (HWND)_lparam;
			dir.chitchat();
			//HWND but = GetDlgItem(_hwnd, 1);
			MessageBox(_hwnd, L"click click men", L"click event", MB_OK);
			DestroyWindow(but);
			x -= 10;
			y -= 10;
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(_hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(_hwnd, &ps);
	}
	break;
	case WM_CREATE:
	{
		// on create windows call
		UserInterface* windowUI = (UserInterface*)((LPCREATESTRUCT)_lparam)->lpCreateParams;

		SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)windowUI);

		windowUI->onCreate();
	}
	break;
	//case WM_CLOSE:
	//{
	//	DestroyWindow(_hwnd);
	//	break;
	//}
	case WM_DESTROY:
	{
		// on destroy window call
		UserInterface* windowUI = (UserInterface*)GetWindowLongPtr(_hwnd, GWLP_USERDATA);
		windowUI->onDestroy();
		::PostQuitMessage(0);
	}
	break;
	default:
		return ::DefWindowProc(_hwnd, _msg, _wparam, _lparam);
		break;
	}
	

	return NULL;
}

UserInterface::UserInterface()
{
   
}

UserInterface::~UserInterface()
{
}

bool UserInterface::init()
{
	// window class container
	WNDCLASSEX wc;

	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;


	if (!::RegisterClassEx(&wc)) return false; // fail to register, return false

	
	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"Harmonographic", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, this);

	// failer creation
	if (!m_hwnd) return false;

	// draw/show window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	
	
	m_is_run = true;

	return true;
}

bool UserInterface::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	this->onUpdate();

	//temp cpu stress release
	Sleep(10);

	return true;
}


bool UserInterface::isRun()
{
	return m_is_run;
}

bool UserInterface::release()
{
	// Destroy window
	if (!::DestroyWindow(m_hwnd)) return false;

	return true;
}

bool UserInterface::chitchat()
{
	std::cout << "button click click\n";
	return true;
}

HWND UserInterface::makeButton(HWND& _hwnd, LPARAM& _lparam)
{
	static int x, y;
	return CreateWindowW(L"Button", L"New canvase", WS_VISIBLE | WS_CHILD, x, y, 150, 20, _hwnd, NULL, ((LPCREATESTRUCT)_lparam)->hInstance , NULL);
	//return CreateWindowW(L"Button", L"New canvase", WS_VISIBLE | WS_CHILD, x, y, 150, 20, _hwnd, NULL, ((LPCREATESTRUCT)_lparam)->hInstance ,(LPVOID) chitchat());
}

void UserInterface::onCreate()
{
}

void UserInterface::onUpdate()
{
}

void UserInterface::onDestroy()
{
	m_is_run = false;
}

