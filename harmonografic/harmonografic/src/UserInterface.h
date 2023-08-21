#pragma once
#include <Windows.h>
#include <iostream>

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	// create window
	bool init();
	bool broadcast();
	bool isRun();
	// release/delete window
	bool release();
	bool chitchat();

	HWND makeButton(HWND& _hwnd, LPARAM& _lparam);

	// EVENTS virtual
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

private:
	HWND m_hwnd;

	bool m_is_run = false;
};

