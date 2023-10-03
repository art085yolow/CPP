#include "WindowHandler.h"

#include <iostream>

WindowHandler::WindowHandler()
{
}

WindowHandler::~WindowHandler()
{
}

void WindowHandler::init(WindowHandler* _parent)
{
	this->m_parent = _parent;
}

void WindowHandler::init(HWND _parent)
{
	m_hwnd_parent = _parent;
}

void WindowHandler::AddComponent(HWND _component, UINT _ID, std::function<void()> _custom_function)
{
	this->m_screen_components.push_back(_component);
	this->m_ID_function[_ID] = _custom_function;
}

void WindowHandler::AddComponent(HWND _component)
{
	this->m_screen_components.push_back(_component);
}

void WindowHandler::show()
{
	for (HWND& n : this->m_screen_components)
	{
		ShowWindow(n, SW_SHOW);
	}
}

void WindowHandler::hide()
{
	for (HWND& n : this->m_screen_components)
	{
		ShowWindow(n, SW_HIDE);
	}
}

void WindowHandler::release()
{
	for (HWND& n : this->m_screen_components)
	{
		DestroyWindow(n);
	}
	this->m_screen_components.clear();

	if (this->m_parent)
	{
		this->m_parent->show();
	}
}

void WindowHandler::customFunction(HWND _hwnd, WPARAM _wParam, LPARAM _lParam)
{
	/// check what call for what. unordered map ect.. to work

	UINT i = LOWORD(_wParam);

	if (m_ID_function.find(i) != m_ID_function.end()) m_ID_function[i]();

	// std::cout << "->\tYour called ID:\t" << i << "\n";
}

WindowHandler* WindowHandler::get()
{
	static WindowHandler storage;
	return &storage;
}

void WindowHandler::CreateMainWindow()
{
	//MyButton b1(m_hwnd, (HMENU)1, [this]() {this->functio1(); });
	//MyButton b1(this->m_hwnd, (HMENU)1);
	//b1.Create("Add note", 100, 10, 70, 20);
	//b1.setFunction([this]() {this->functio1(); });

	//MyButton b2(m_hwnd, (HMENU)2);
	//b2.Create("Option", 100, 40, 70, 20);
	//
	//MyButton b3(m_hwnd, (HMENU)3);
	//b3.Create("Exit", 100, 70, 70, 20);
	//b3.setFunction([this]() {this->functioExit(); });

	//this->m_main_window.AddComponent(b1.GetHanlde());
	//this->m_main_window.AddComponent(b2.GetHanlde());
	//this->m_main_window.AddComponent(b3.GetHanlde());


	// craete first 3 buttons
	// add node -- adjust HMENU
	WindowHandler::get()->AddComponent(
		UserInterface::makeButton(this->m_hwnd_parent, "Add note", 10, 10, 70, 20, (HMENU)1001), 1001, [this]() {this->functio1(); }
	);
	// options - nothing for now	
	WindowHandler::get()->AddComponent(
		UserInterface::makeButton(this->m_hwnd_parent, "Options", 10, 40, 70, 20, (HMENU)1002), 1002, [this]() {this->functio2(); }
	);
	// exit - terminate application	
	WindowHandler::get()->AddComponent(
		UserInterface::makeButton(this->m_hwnd_parent, "Exit", 10, 70, 70, 20, (HMENU)1003), 1003, [this]() {this->functioExit(); }
	);




	show();
}

void WindowHandler::functio1()
{
	WindowHandler::get()->AddComponent(UserInterface::Label_text(this->m_hwnd_parent, "Something from function 1 button 1", 400, 10, 80, 80));
	std::cout << "->\tYour called ID:\t" << 1001 << " -> from functions.\n";
}

void WindowHandler::functio2()
{
	WindowHandler::get()->AddComponent(UserInterface::Label_text(this->m_hwnd_parent, "Something from function 2 button 2", 400, 400, 80, 80));
	std::cout << "->\tYour called ID:\t" << 1002 << " -> from functions.\n";
}

void WindowHandler::functioExit()
{
	std::cout << "->\tYour called ID:\t" << 1003 << " -> from functions.\n";
	::DestroyWindow(this->m_hwnd_parent);

}