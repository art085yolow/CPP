#pragma once
#include <vector>
#include <map>
#include <functional>
#include <Windows.h>
#include "WindowInterface/UserInterface.h"

class WindowHandler
{
public:
	WindowHandler();
	~WindowHandler();

	void init(WindowHandler* _parent);
	void init(HWND _parent);
	//window behaviour -virtuale
	void AddComponent(HWND _component, UINT _ID, std::function<void()> _custom_function);
	void AddComponent(HWND _component);
	void show();
	void hide();
	void release();

	// check for activity
	void customFunction(HWND _hwnd, WPARAM _wParam, LPARAM _lParam);

	// make visual window
	void CreateMainWindow();
	void functio1();
	void functio2();
	void functioExit();


	static WindowHandler* get();

protected:
	WindowHandler* m_parent;
	HWND m_hwnd_parent;

	std::vector<HWND> m_screen_components;

	std::unordered_map<UINT, std::function<void()>> m_ID_function;

};

