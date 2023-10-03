#include "Harmonographic.h"

Harmonographic::Harmonographic() 
{
}

Harmonographic::~Harmonographic()
{
	m_main_window.release();
}

void Harmonographic::onCreate()
{
	UserInterface::onCreate();
	WindowHandler::get()->init(this->m_hwnd);
	m_main_window = *WindowHandler::get();

	this->m_main_window.CreateMainWindow();
}

void Harmonographic::onUpdate()
{
	UserInterface::onUpdate();
}

void Harmonographic::onDestroy()
{
	UserInterface::onDestroy();
}


