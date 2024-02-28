// tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>
#include <random>


#include "TGEngine.h"


int8_t screen_width = 80;
int8_t screen_height = 30;

uint32_t colors[] = { 0x000000FF, 0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFFFF00FF, 0x00FFFFFF, 0xFFFFFFFF };



int main()
{
	std::cout << "Hello World!\n";
	std::cout << "\n";

	Game_session _core;

	HANDLE old_win = GetStdHandle(STD_OUTPUT_HANDLE);

	HANDLE screen_buff = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SMALL_RECT rect{
		.Left = 0,
		.Top = 0,
		.Right = screen_width - 1,
		.Bottom = screen_height,
	};

	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(screen_buff, &buffer_info);
	COORD buffer_size;

	buffer_size.X = buffer_info.dwSize.X;
	buffer_size.Y = buffer_info.dwSize.Y;
	
	rect.Right = buffer_size.X;
	rect.Bottom = buffer_size.Y;

	SetConsoleWindowInfo(screen_buff, false, &rect);
	SetConsoleActiveScreenBuffer(screen_buff);
	
	DWORD dwBitesWritten = 0;
	wchar_t* buffer = new wchar_t[screen_width * screen_height];

	for (int16_t i = 0; i < screen_width * screen_height; i++)
	{
		buffer[i] = L' ';
	};

	std::wstring score = L"SCORE";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[3 * screen_width + 25 + i] = score[i];
	}

	// next piece window
	// top and bottom sides of the frame
	for (int8_t col = 0; col < 7; col++)
	{
		buffer[8 * screen_width + (col + 23)] = '#';
		buffer[15 * screen_width + (col + 23)] = '#';
	}
	// left and right sides of the frame
	for (int8_t row = 0; row < 6; row++)
	{
		buffer[(row + 9) * screen_width + 23] = '#';
		buffer[(row + 9) * screen_width + 29] = '#';
	}

	score = L"INPUTS";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[14 * screen_width + 35 + i] = score[i];
	}

	score = L"MOVE   - LEFT, DOWN, RIGHT ARROWS";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[15 * screen_width + 37 + i] = score[i];
	}

	score = L"ROTATE - Z, X";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[16 * screen_width + 37 + i] = score[i];
	}

	score = L"RESET  - R";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[17 * screen_width + 37 + i] = score[i];
	}

	score = L"QUIT   - Q";
	for (int8_t i = 0; i < score.length(); i++)
	{
		buffer[18 * screen_width + 37 + i] = score[i];
	}

	reset(_core);


	while (!_core.b_gameover)
	{


		// game timing
		// Sleep(50);

		game_tick(_core);

		// input processing

		for (int8_t k = 0; k < 7; k++)
			_core.b_key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28ZXRQ"[k]))) != 0;


		// DISPLAY DRAWING
		// draw 'field' of game(include boarders)
		for (int8_t row = 0; row < 21; row++)
		{
			for (int8_t col = 0; col < 12; col++)
			{
				buffer[(row + 2) * screen_width + (col + 2)] = L" ABCDEFGH=#"[(_core.field[row] >> ((col) * 4)) & 0b1111];
			}
		}

		for (int8_t row = 0; row < 4; row++)
		{
			for (int8_t col = 0; col < 4; col++)
			{
				if (((tetromino[_core.current_player_piece].shapes[_core.current_rot] >> (row * 4 + (col))) & 1) != 0)
				{
					buffer[(row + 2 + _core.n_current_y) * screen_width + (col + 2 + _core.n_current_x)] = L" ABCDEFGH=#"[_core.current_player_piece];
				};
			}
		}

		// draw n_score - score value
		swprintf_s(&buffer[5 * screen_width + 25], 16, L"%15d", _core.n_score);
		swprintf_s(&buffer[20 * screen_width + 20], 18, L"LEVEL %11d", _core.n_level_count);
		swprintf_s(&buffer[21 * screen_width + 20], 18, L"CLEARED %9d", _core.n_cleaned_lines_count);
		//swprintf_s(&buffer[22 * screen_width + 20], 18, L"SPEED   %9d", _core.n_speed_game);

		// draw next piece

		for (int8_t row = 0; row < 4; row++)
		{
			for (int8_t col = 0; col < 4; col++)
			{
				buffer[(row + 10) * screen_width + (col + 25)] = L" ABCDEFGH=#"[((tetromino[_core.next_piece].shapes[0] >> (row * 4 + (col))) & 1) != 0 ? _core.next_piece : 0];
			}
		}

		// display frame
		WriteConsoleOutputCharacterW(screen_buff, buffer, screen_width * screen_height, { 0,0 }, &dwBitesWritten);
	}

	SetConsoleActiveScreenBuffer(old_win);

	CloseHandle(screen_buff);

	std::cout << "\nSCORE: " << std::to_string(_core.n_score) << "\nGAME OVER\n\n";

	return 0;
}
