// tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// grid 20 x 10 and 2 top are hidden
//
// minimalize used resources (expl: 0/1 for one grid state = 200 bit 
// or 8 state of grid color: 0b000 = no color, 0b001 = red, 0b010 = green, 0b011 = blue, 0b100 = yellow, 0b101 = purple, 0b110 = lime, 0b111 = orange
// and so 3bit * 10 blocks in row give 30 bit[int is 32bit])
// 
//	7x (mat 4x4) + rotation version if have
//
//
//   there is 7 shapes = R/L 'L' (4 rotarion/postion), 'I' (2 rotation/postion), 'Sq' (1 rotarion/postion), R/L 'Z'(2 rotarion/postion), 'T' (4 rotarion/postion)
//  [0010]				[0000]
//  [0010]  = 0x2222	[0110]  = 0x0660
//  [0010]  = 'I'		[0110]  = 'Sq'
//  [0010]				[0000]  
//
//  [0001]				[0010]
//  [0011]	= 0x0231	[0011] = 0x0132
//  [0010]  = 'Z'		[0001]  = 'S'
//  [0000]				[0000]  
//
//  [0010]				[0010]
//  [0010]	= 0x0322	[0010] = 0x0622
//  [0011]  = 'L-L'		[0110]  = 'R-L'
//  [0000]				[0000]
//
//  [0010]
//  [0110] = 0x0262
//  [0010]  = 'T'
//  [0000]  
//


#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <thread>
#include <vector>

size_t screen_width = 80;
size_t screen_height = 30;

uint32_t colors[] = { 0x000000FF, 0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFFFF00FF, 0x00FFFFFF, 0xFFFFFFFF };


// Game mechanic ->

uint16_t b_sq[] = { 0x0660 };
uint16_t b_i[] = { 0x2222, 0x00F0 };
uint16_t b_z[] = { 0x0036, 0x0231 };
uint16_t b_s[] = { 0x0063, 0x0132 };
uint16_t b_lr[] = { 0x0074, 0x0223, 0x0170, 0x0622 };
uint16_t b_ll[] = { 0x0071, 0x0322, 0x0470, 0x0226 };
uint16_t b_t[] = {  0x0072, 0x0232, 0x0270, 0x0262 };

struct Shape
{
	uint8_t num_temps;
	uint8_t rot;
	uint16_t* shapes;
};

Shape tetromino[] = 
{
	{1, 0, b_sq},
	{2, 0, b_i},
	{2, 0, b_z},
	{2, 0, b_s},
	{4, 0, b_lr},
	{4, 0, b_ll},
	{4, 0, b_t}
};

struct Game_session
{
	// every 4bit is one block in line. 12 width x 20 height
	int64_t field[21];

	// game status
	bool b_gameover = false;
	bool b_force_down = false;

	int n_speed_game = 20;
	int n_count_tick_game = 0;
	int n_score = 0;

	std::vector<int> v_full_lines;
	bool b_delet_full_line = false;
	int n_count_tick_show_full_line = 0;
	int n_line_count = 0;

	// player status
	int8_t n_current_x = 4;
	int8_t n_current_y = 0;

	int8_t current_player_piece = 0;
	int8_t current_rot = 0;
	int8_t next_piece = 0;

	// keys status
	bool b_key[7]{ 0 };		//		RIGHT, LEFT, DOWN, ROTATE LEFT, ROTATE RIGHT, RESET, QUIT
	bool b_rot_but_press = false;

};

int new_rand_piece()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 6);

	return dis(gen);
}

int8_t rotate_block(int8_t _id_block, int8_t _rotation)
{
	return ((tetromino[_id_block].rot + _rotation) & (tetromino[_id_block].num_temps - 1));
}

bool does_piece_fit_in(Game_session& core, int8_t _rot, int8_t _position_col, int8_t _position_row)
{

	int8_t n_rot = rotate_block(core.current_player_piece, core.current_rot + _rot);
	
	for (size_t row = 0; row < 4; row++)
	{
		int8_t n_row = core.n_current_y + _position_row + row;
		
		for (size_t col = 0; col < 4; col++)
		{
			int8_t n_col = core.n_current_x + _position_col + col;


			// if any doesn't fit return false
			if (n_row >= 0 && n_row < 21/*it is field height with bounds*/)
			{
				if (n_col >= 0 && n_col < 12/*it is field width*/)
				{
					if (((core.field[n_row ] >> ((n_col) * 4) & 0b1111) != 0) && (((tetromino[core.current_player_piece].shapes[n_rot] >> (row * 4 + col)) & 0b1) != 0))
					return false;

				}
			}
		}
	}

	return true;
}

void lock_the_piece(Game_session& core)
{
	for (size_t row = 0; row < 4; row++)
	{
		int8_t n_row = core.n_current_y + row;
		
		for (size_t col = 0; col < 4; col++)
		{
			int8_t n_col = core.n_current_x + col;
			// if any doesn't fit return false
			if (n_row >= 0 && n_row < 21/*it is field height with bounds*/)
			{
				if (n_col >= 0 && n_col < 12/*it is field width*/)
				{
					if (((core.field[n_row] >> ((n_col) * 4) & 0b1111) == 0) && ((tetromino[core.current_player_piece].shapes[core.current_rot] >> (row * 4 + col)) & 0b1) != 0)
						core.field[n_row] |= ((uint64_t)(core.current_player_piece + 1) & 0b1111)<<((n_col) * 4);
				}
			}
		}
	}
}

void get_new_piece(Game_session& core)
{
	core.current_player_piece = core.next_piece;
	core.current_rot = 0;

	core.n_current_x = 4;
	core.n_current_y = 0;

	core.next_piece = new_rand_piece();
}

void reset(Game_session& core)
{

	for (size_t i = 0; i < 21; i++)
	{
		core.field[i] = 0x0A0000000000A;
	}
	core.field[20] = 0x0AAAAAAAAAAAA;

	core.b_gameover = false;
	core.b_force_down = false;

	core.n_speed_game = 20;
	core.n_count_tick_game = 0;
	core.n_score = 0;

	core.v_full_lines.clear();
	core.b_delet_full_line = false;
	core.n_count_tick_show_full_line = 0;
	core.n_line_count = 0;

	// player status
	core.next_piece = new_rand_piece();
	get_new_piece(core);

	// keys status
	for (size_t k = 0; k < 7; k++)
		core.b_key[k] = false;
	core.b_rot_but_press = false;
}

void game_tick(Game_session& core)
{
	core.n_count_tick_game++;
	core.b_force_down = (core.n_speed_game == core.n_count_tick_game);
	// start counting time to delete full line (== 400ms)
	core.b_delet_full_line = (!core.v_full_lines.empty() && (core.n_count_tick_show_full_line == 8));
	core.n_count_tick_show_full_line += (!core.v_full_lines.empty()) ? 1 : 0;
}

void move_piece(Game_session& core)
{
	if (core.b_key[2] && does_piece_fit_in(core, 0, 0, 1))
	{
		core.n_current_y++;
		core.n_score++;
	}
	// rotation
	if (core.b_key[0] || core.b_key[1] || core.b_key[3] || core.b_key[4])
	{
		core.n_current_x -= (core.b_key[1] && !core.b_rot_but_press && does_piece_fit_in(core, 0, -1, 0)) ? 1 : 0;
		core.n_current_x += (core.b_key[0] && !core.b_rot_but_press && does_piece_fit_in(core, 0, 1, 0)) ? 1 : 0;
		
		if (core.b_key[3] && !core.b_rot_but_press && does_piece_fit_in(core, -1, 0, 0)) {
			core.current_rot = rotate_block(core.current_player_piece, core.current_rot - 1);
		}
		
		if (core.b_key[4] && !core.b_rot_but_press && does_piece_fit_in(core, 1, 0, 0))
		{
			core.current_rot = rotate_block(core.current_player_piece, core.current_rot + 1);
		};

		core.b_rot_but_press = true;
	}
	else
	{
		core.b_rot_but_press = false;
	}

	if (core.b_key[5])
	{
		reset(core);
	}

	core.b_gameover = core.b_key[6];
}

void forcedown_piece(Game_session& core)
{
	core.n_count_tick_game = 0;


	if (does_piece_fit_in(core, 0, 0, 1))
	{
		core.n_current_y++;
	}
	else
	{
		// it can't progress down. Lock the piece
		lock_the_piece(core);

		// check logic for full lines
		int check_rows = (core.n_current_y + 4 < 21) ? core.n_current_y + 4 : 20;
		for (int rows = core.n_current_y; rows < check_rows; rows++)
		{
			bool b_check_line = true;
			// stay true if all block full
			for (int cols = 1; cols < 11; cols++)
			{
				b_check_line &= (core.field[rows] >> (cols * 4) & 0b1111) != 0;
			}
			// if true preper to remove
			if (b_check_line)
			{
				core.field[rows] = 0x0A9999999999A;
				core.v_full_lines.push_back(rows);
				core.n_line_count++;
			}
		}

		// give player new piece to play with
		get_new_piece(core);

		// but check if new piec have place for new round
		core.n_score += 25;
		if (!core.v_full_lines.empty()) core.n_score += (1 << core.v_full_lines.size()) * 100;

		if (core.n_line_count % 10 == 0)
			if (core.n_speed_game >= 5) core.n_speed_game--;

		core.b_gameover = !does_piece_fit_in(core, 0, 0, 0);
	}
}

void delete_fulllines(Game_session& core)
{
	int move_line = 0;
	for (int row = core.v_full_lines[core.v_full_lines.size() - 1]; row >= 0; row--)
	{
		if (row == (!core.v_full_lines.empty() ? core.v_full_lines[core.v_full_lines.size() - 1] : -1))
		{
			move_line++;
			if (!core.v_full_lines.empty()) core.v_full_lines.pop_back();
		}
		else
		{
			core.field[row + move_line] = core.field[row];
			core.field[row] = 0x0A0000000000A;
		}
	}

	core.n_count_tick_show_full_line = 0;
}

// -> Game mechanic

int main()
{
	std::cout << "Hello World!\n";
	std::cout << "\n";
	
	HANDLE screen_buff = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(screen_buff);
	DWORD dwBitesWritten = 0;
	wchar_t* buffer = new wchar_t[screen_width * screen_height];
	for (size_t i = 0; i < screen_width * screen_height; i++)
	{
		buffer[i] = L' ';
	};

	std::wstring score = L"SCORE";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[3 * screen_width + 25 + i] = score[i];
	}

	// next piece window
	for (size_t row = 0; row < 8; row++)
	{
		for (size_t col = 0; col < 7; col++)
		{
			if(row == 0 || row == 7 || col == 0 || col == 6)
				buffer[(row + 8) * screen_width + (col + 23)] = L" ABCDEFGH=#"[10];
		}
	}

	score = L"INPUTS";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[14 * screen_width + 35 + i] = score[i];
	}

	score = L"MOVE   - LEFT, DOWN, RIGHT ARROWS";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[15 * screen_width + 37 + i] = score[i];
	}

	score = L"ROTATE - Z, X";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[16 * screen_width + 37 + i] = score[i];
	}

	score = L"RESET  - R";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[17 * screen_width + 37 + i] = score[i];
	}

	score = L"QUIT   - Q";
	for (size_t i = 0; i < score.length(); i++)
	{
		buffer[18 * screen_width + 37 + i] = score[i];
	}

	Game_session core;
	reset(core);

		
	while (!core.b_gameover)
	{


		// game timing
		Sleep(50);

		game_tick(core);
		
		// input processing

		for (size_t k = 0; k < 7; k++)
			core.b_key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28ZXRQ"[k]))) != 0;
		

		// input handle - movement
		move_piece(core);

		// game mechanic

		if (core.b_force_down)
		{
			forcedown_piece(core);
		}

		if (core.b_delet_full_line)
		{
			delete_fulllines(core);
		}



		// DISPLAY DRAWING
		// draw 'field' of game(include boarders)
		for (size_t row = 0; row < 21; row++)
		{
			for (size_t col = 0; col < 12; col++)
			{
				buffer[(row + 2) * screen_width + (col + 2)] = L" ABCDEFGH=#"[(core.field[row]>>((col)*4))&0b1111];
			}
		}

		for (size_t row = 0; row < 4; row++)
		{
			for (size_t col = 0; col < 4; col++)
			{
				if (((tetromino[core.current_player_piece].shapes[core.current_rot] >> (row * 4 + (col))) & 1) != 0)
				{
					buffer[(row + 2 + core.n_current_y) * screen_width + (col + 2 + core.n_current_x)] = L" ABCDEFGH=#"[core.current_player_piece + 1];
				};
			}
		}

		// draw score 
		swprintf_s(&buffer[5 * screen_width + 25], 16, L"%15d", core.n_score);

		// draw next piece

		for (size_t row = 0; row < 4; row++)
		{
			for (size_t col = 0; col < 4; col++)
			{
				buffer[(row + 10) * screen_width + (col + 25)] = L" ABCDEFGH=#"[((tetromino[core.next_piece].shapes[0] >> (row * 4 + (col))) & 1) != 0 ? core.next_piece + 1 : 0];
			}
		}
		
		// display frame
		WriteConsoleOutputCharacterW(screen_buff, buffer, screen_width * screen_height, { 0,0 }, &dwBitesWritten);
	}
	
	CloseHandle(screen_buff);

	std::cout << "\nSCORE: "<< std::to_string(core.n_score) << "\nGAME OVER\n\n";
	return 0;
}
