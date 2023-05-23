#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <initializer_list>


#include "Memo.h"

const enum Titles_Names
{
	f_path_tree = 0,	// frame begin
	f_title = 4,		// title position
	f_tag = 10,			// tag title position
	f_description = 15,	// description tittle position
	f_max_line = 49,	// text height	50 lines
	f_width_char = 89,	// tekt width	90 char
	f_sign_char_updown_position = 88	// sign char if description doesn't fit on screen
};


class UserInterface
{
public:
	UserInterface();

	void print();

private:
	Tree m_notes_register;
	
	std::vector<std::string> m_lines;

	// windows handlers

	HWND m_console;
	RECT m_r;
	int m_width_window = 800, m_height_window = 720;

};

