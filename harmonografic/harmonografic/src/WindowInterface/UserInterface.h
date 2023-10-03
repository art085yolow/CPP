#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

// #define ADD_NOTES 1
// #define OPTIONS 2
// #define EXIT 3

enum class MenuID
{
	MAIN_MENU=1,
	OPTION_MENU,
	ADD_NODE_MENU,
	TEXTLABEL,
	COMBOBOX,
	LISTBOX,
	EDITBOX,
	BUTTON
};


class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	// create window
	bool init();
	void setHWND(HWND _hwnd);

	// make them run
	bool RunMessageLoop();
	bool isRun();
	
	// release/delete window
	bool release();
	bool static chitchat();



	std::wstring static StringToWstring(const std::string& _str);
	std::string static WstringToString(const std::wstring& _wstr);

	// Button
	HWND static makeButton(HWND _hwnd, std::string  _title, int _x, int _y, int _w, int _h, HMENU _hMenu /*WPARAM*/,
						DWORD _dwStyle = WS_VISIBLE | WS_CHILD, HINSTANCE _hInstance=NULL, LPVOID _lparam=NULL);
	HWND static makeButton(HWND _hwnd, std::wstring _title, int _x, int _y, int _w, int _h, HMENU _hMenu /*WPARAM*/,
						DWORD _dwStyle = WS_VISIBLE | WS_CHILD, HINSTANCE _hInstance = NULL, LPVOID _lparam = NULL);
	
	// Message Box
	int static MsgBox(HWND _hWnd, std::string  _lpText =  "Something went very wrong!", std::string  _lpTitle =  "Not specified!", UINT _uType = MB_OK);
	int static MsgBox(HWND _hWnd, std::wstring _lpText = L"Something went very wrong!", std::wstring _lpTitle = L"Not specified!", UINT _uType = MB_OK);
	int static MsgBox(HWND _hWnd, std::string  _lpText =  "Something went very wrong!", std::wstring _lpTitle = L"Not specified!", UINT _uType = MB_OK);
	int static MsgBox(HWND _hWnd, std::wstring _lpText = L"Something went very wrong!", std::string  _lpTitle =  "Not specified!", UINT _uType = MB_OK);
	
	// Label text
	HWND static Label_text(HWND _hwnd, std::string  _text, int _x, int _y, int _w, int _h, DWORD _dwStyle = WS_VISIBLE | WS_CHILD | SS_LEFT);
	HWND static Label_text(HWND _hwnd, std::wstring _text, int _x, int _y, int _w, int _h, DWORD _dwStyle = WS_VISIBLE | WS_CHILD | SS_LEFT);

	// Combo Box
	HWND static ComboBox(HWND _hwnd, int _x, int _y, int _w, int _h,
		std::vector<std::string> _vec_text ,
		HMENU _hMenu /*WPARAM*/, 
		DWORD _dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWN,
		HINSTANCE _hInstance = NULL,
		LPVOID _lparam = NULL);

	HWND static ComboBox(HWND _hwnd, int _x, int _y, int _w, int _h,
		std::vector<std::wstring> _vec_text,
		HMENU _hMenu /*WPARAM*/, 
		DWORD _dwStyle = WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_DROPDOWN,
		HINSTANCE _hInstance = NULL, LPVOID _lparam = NULL);

	// List Box
	HWND static ListBox(HWND _hwnd, int _x, int _y, int _w, int _h,
		std::vector<std::string> _vec_text,
		HMENU _hMenu /*WPARAM*/, 
		DWORD _dwStyle = WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_STANDARD,
		HINSTANCE _hInstance = NULL,
		LPVOID _lparam = NULL);
	HWND static ListBox(HWND _hwnd, int _x, int _y, int _w, int _h,
		std::vector<std::wstring> _vec_text,
		HMENU _hMenu /*WPARAM*/, 
		DWORD _dwStyle = WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_STANDARD,
		HINSTANCE _hInstance = NULL,
		LPVOID _lparam = NULL);

	// Edit box
	HWND static EditBox(HWND _hwnd, int _x, int _y, int _w, int _h, 
		HMENU _hMenu /*WPARAM*/, 
		DWORD _dwStyle = WS_VISIBLE | WS_CHILD ,
		HINSTANCE _hInstance = NULL,
		LPVOID _lparam = NULL);

	// EVENTS virtual
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();


	/// other

protected:
	HWND m_hwnd;

	bool m_is_run = false;

};

