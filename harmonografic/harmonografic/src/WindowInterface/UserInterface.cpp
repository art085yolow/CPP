#include "UserInterface.h"
#include "WindowHandler.h"


// callback-> very, very, very important
LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	//UserInterface dir;
	//static int x, y;

	// here procces all msg

	switch (_msg)
	{
	case WM_KEYDOWN:
	{
		switch(_wparam)
		{
			case VK_RETURN:
				{
					//UserInterface::MsgBox(_hwnd, "DZIALA!!!!!!!!!!!!!!!!!!!!!!!","WTF!!!", MB_ICONINFORMATION);
				}
			break;
			case VK_SHIFT:
				{
					//UserInterface::Label_text(_hwnd, "Napis M3 Funkcja", 200, 200, 150, 50);
				}
			break;
			case VK_TAB:
				{
					//UserInterface::ComboBox(_hwnd, 300, 300, 100, 200, { {"Element 1"}, {"Element 2"} },(HMENU)MenuID::COMBOBOX);

					//UserInterface::ComboBox(_hwnd, 500, 300, 100, 200, { {L"Element W1" }, { L"Element W2" }}, (HMENU)MenuID::COMBOBOX);
				}
			break;
			case VK_CONTROL:
				{
					UserInterface::MsgBox(_hwnd, "string", "string");
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
			case VK_BACK:
				{
					
					//UserInterface::ListBox(_hwnd, 210, 10, 100, 50, { { L"String 1"}, { L"String 2"} }, (HMENU)MenuID::LISTBOX);
				}
			break;
			case VK_ESCAPE:
				{
					if (UserInterface::MsgBox(_hwnd, "Really quit?", "Exit app", MB_OKCANCEL) == IDOK)
					{

						DestroyWindow(_hwnd);
					}
				}
			break;
			case VK_DELETE:
				{	

					//UserInterface::EditBox(_hwnd, 10, 500, 100, 20, (HMENU)MenuID::EDITBOX);
					//UserInterface::makeButton(_hwnd, "button from string", x, y, 250, 50, (HMENU)MenuID::MAIN_MENU);
				
					//x += 10;
					//y += 10;
				}
			break;
		
		}
	}
	break;
	case WM_COMMAND:
	{
		switch (HIWORD(_wparam))
		{
		case CBN_SELCHANGE /* or LBN_SELCHANGE */:
			{
				int controlId = LOWORD(_wparam);
				
				switch (controlId)
				{
				case (int)MenuID::COMBOBOX:
					{
						int ItemIndex = SendMessage((HWND)_lparam, (UINT)CB_GETCURSEL, NULL, NULL);

						//std::wstring ListItem;
						TCHAR ListItem[256];

						(TCHAR)SendMessage((HWND)_lparam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);

						//printf("Not equil!!", ListItem.length() != UserInterface::WstringToString(ListItem).length());

						UserInterface::MsgBox(_hwnd, UserInterface::WstringToString(ListItem), "Item selected (ComboBox)", MB_OK);
					}
				break;

				case (int)MenuID::LISTBOX:
					{
						//HWND listHandle = GetDlgItem(_hwnd, (int)MenuID::LISTBOX);

						int ItemIndex = SendMessage((HWND)_lparam, LB_GETCURSEL, NULL, NULL);

						if (ItemIndex != LB_ERR)
						{

						//std::wstring ListItem;
						TCHAR ListItem[256];

						SendMessage((HWND)_lparam, (UINT)LB_GETTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);

						//printf("Not equil!!", ListItem.length() != UserInterface::WstringToString(ListItem).length());

						UserInterface::MsgBox(_hwnd, UserInterface::WstringToString(ListItem), "Item selected (ListBox)", MB_OK);
						}
					}
				break;

				default:
					UserInterface::MsgBox(_hwnd, UserInterface::StringToWstring("Called control ID: " + std::to_string(controlId)), "ID Control call", MB_OK);
					break;
				}
				
				/*
				if (kontrolId == 5)
				{

					int ItemIndex = SendMessage((HWND)_lparam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

					//std::wstring ListItem;
					TCHAR ListItem[256];

					(TCHAR)SendMessage((HWND)_lparam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);

					//printf("Not equil!!", ListItem.length() != UserInterface::WstringToString(ListItem).length());

					UserInterface::MsgBox(_hwnd, UserInterface::WstringToString(ListItem), "Item selected", MB_OK);
				}
				else
				{
					UserInterface::MsgBox(_hwnd, UserInterface::StringToWstring(std::to_string(kontrolId)), "ID Control call", MB_OK);

				}
				*/

			}
		break;
		/*
		case LBN_SELCHANGE:
			{
				int ItemIndex = SendMessage((HWND)_lparam, (UINT)LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

				//std::wstring ListItem;
				TCHAR ListItem[256];

				(TCHAR)SendMessage((HWND)_lparam, (UINT)LB_GETITEMDATA, (WPARAM)ItemIndex, (LPARAM)ListItem);

				//printf("Not equil!!", ListItem.length() != UserInterface::WstringToString(ListItem).length());

				UserInterface::MsgBox(_hwnd, UserInterface::WstringToString(ListItem), "Item selected", MB_OK);
			}
		break; */
		}
		
		if (LOWORD(_wparam) > 1000)
		 {
		 	// HWND but = (HWND)_lparam;
		 	// //dir.chitchat();
		 	// //HWND but = GetDlgItem(_hwnd, 1);
		 	// UserInterface::MsgBox(_hwnd, "click click men", "click event");
		 	// DestroyWindow(but);
		 	// //x -= 10;
		 	// //y -= 10;

			WindowHandler::get()->customFunction(_hwnd, _wparam, _lparam);

		 }

		
	}
	break;
	/* case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(_hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));

		EndPaint(_hwnd, &ps);
	}*/
	break; 
	case WM_CREATE:
	{
		// on create windows call
		UserInterface* windowUI = (UserInterface*)((LPCREATESTRUCT)_lparam)->lpCreateParams;

		SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)windowUI);
		windowUI->setHWND(_hwnd);
		WindowHandler::get()->init(_hwnd);
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

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//wc.style = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = NULL;
	
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

void UserInterface::setHWND(HWND _hwnd)
{
	this->m_hwnd = _hwnd;
}

bool UserInterface::RunMessageLoop()
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



std::wstring UserInterface::StringToWstring(const std::string& _str)
{
	if (_str.length() == 0 )
					return L"";

	std::wstring wstr((int)MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), -1, nullptr, 0), L'\0');
	MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), -1, &wstr[0], (int)MultiByteToWideChar(CP_UTF8, 0, _str.c_str(), -1, nullptr, 0));

	return wstr;
}

std::string UserInterface::WstringToString(const std::wstring& _wstr)
{
	if (_wstr.length() == 0)
		return "";

	std::string str((int)WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), -1, nullptr, 0, nullptr, nullptr), '\0');
	WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), -1, &str[0], (int)WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), -1, nullptr, 0, nullptr, nullptr), nullptr, nullptr);

	return str; 
}

HWND UserInterface::makeButton(HWND _hwnd, std::string _title, int _x, int _y, int _w, int _h, HMENU _hMenu ,DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	return makeButton(_hwnd, StringToWstring(_title), _x, _y, _w, _h, _hMenu, _dwStyle, _hInstance , _lparam);
}

HWND UserInterface::makeButton(HWND _hwnd, std::wstring _title, int _x, int _y, int _w, int _h, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	return CreateWindow(L"Button", _title.c_str(), _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);
}

int UserInterface::MsgBox(HWND _hWnd, std::string _lpText, std::string _lpTitle, UINT _uType)
{
	return MsgBox(_hWnd, StringToWstring(_lpText), StringToWstring(_lpTitle), _uType);
}

int UserInterface::MsgBox(HWND _hWnd, std::wstring _lpText, std::wstring _lpTitle, UINT _uType)
{
	return MessageBox(_hWnd, _lpText.c_str(), _lpTitle.c_str(), _uType);
}

int UserInterface::MsgBox(HWND _hWnd, std::string _lpText, std::wstring _lpTitle, UINT _uType)
{
	return (_hWnd, StringToWstring(_lpText), _lpTitle, _uType);
}

int UserInterface::MsgBox(HWND _hWnd, std::wstring _lpText, std::string _lpTitle, UINT _uType)
{

	return MsgBox(_hWnd, _lpText, StringToWstring(_lpTitle), _uType);
}

HWND UserInterface::Label_text(HWND _hwnd, std::string _text, int _x, int _y, int _w, int _h, DWORD _dwStyle)
{
	return CreateWindow(L"STATIC", StringToWstring(_text).c_str(), _dwStyle, _x, _y, _w, _h, _hwnd, NULL, NULL, NULL);
}

HWND UserInterface::Label_text(HWND _hwnd, std::wstring _text, int _x, int _y, int _w, int _h, DWORD _dwStyle)
{
	return CreateWindow(L"STATIC", _text.c_str(), _dwStyle, _x, _y, _w, _h, _hwnd, NULL, NULL, NULL);
}

HWND UserInterface::ComboBox(HWND _hwnd, int _x, int _y, int _w, int _h, std::vector<std::string> _vec_text, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	HWND cmbBox = CreateWindow(L"COMBOBOX", NULL, _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);

	for (std::string& s : _vec_text)
	{
			SendMessage(cmbBox, CB_ADDSTRING, 0, (LPARAM)StringToWstring(s).c_str());
	}

	return cmbBox;
}

HWND UserInterface::ComboBox(HWND _hwnd, int _x, int _y, int _w, int _h, std::vector<std::wstring> _vec_text, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	HWND cmbBox = CreateWindow(L"COMBOBOX", NULL, _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);

	for (std::wstring& ws : _vec_text)
	{
		SendMessage(cmbBox, CB_ADDSTRING, 0, (LPARAM)ws.c_str());
	}

	return cmbBox;
}

HWND UserInterface::ListBox(HWND _hwnd, int _x, int _y, int _w, int _h, std::vector<std::wstring> _vec_text, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	HWND lsBox = CreateWindow(L"LISTBOX", NULL, _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);

	for (std::wstring& ws : _vec_text)
	{
		SendMessage(lsBox, LB_ADDSTRING, 0, (LPARAM)ws.c_str());
	}

	return lsBox;

}

HWND UserInterface::ListBox(HWND _hwnd, int _x, int _y, int _w, int _h, std::vector<std::string> _vec_text, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	HWND lsBox = CreateWindow(L"LISTBOX", NULL, _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);

	for (std::string& s : _vec_text)
	{
		SendMessage(lsBox, LB_ADDSTRING, 0, (LPARAM)StringToWstring(s).c_str());
	}

	return lsBox;

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

HWND UserInterface::EditBox(HWND _hwnd, int _x, int _y, int _w, int _h, HMENU _hMenu, DWORD _dwStyle, HINSTANCE _hInstance, LPVOID _lparam)
{
	return CreateWindow(L"EDIT", L"", _dwStyle, _x, _y, _w, _h, _hwnd, _hMenu, _hInstance, _lparam);;
}


