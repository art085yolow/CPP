// tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// grid 20 x 10 and 2 top are hidden
//
// minimalize used resources (expl: 0/1 for one grid state = 200 bit 
// or 8 state of grid color: 0b000 = no color, 0b001 = red, 0b010 = green, 0b011 = blue, 0b100 = yellow, 0b101 = purple, 0b110 = lime, 0b111 = orange
// and so 3bit * 10 blocks in row give 30 bit[int is 32bit])
// 
//	uint16_t shape_bloks[] = { 1632, 240, 32819, 32798, 32828, 32825, 32826}; // 2x (mat 4x4) and 5x (mat 3x3)
//	bit 16 (0-15) is 0 for 4x4 and 1 for 3x3 
//
//   there is 7 shapes = R/L 'L' (4 rotarion/postion), 'I' (2 rotation/postion), 'Sq' (1 rotarion/postion), R/L 'Z'(2 rotarion/postion), 'T' (4 rotarion/postion)
//  [0010]				[0000]
//  [0010]  = 0x2222	[0110]  = 0x0660
//  [0010]  = 'I'		[0110]  = 'Sq'
//  [0010]				[0000]  
//
//  [0010]				[0100]
//  [0110]	= 0x8462	[0110] = 0x8264
//  [0100]  = 'Z'		[0010]  = 'S'
//  [1000]				[1000]  
//
//  [0100]				[0010]
//  [0100]	= 0x8644	[0010] = 0x8622
//  [0110]  = 'L-L'		[0110]  = 'R-L'
//  [1000]				[1000]
//
//  [0010]
//  [0110] = 0x8262
//  [0010]  = 'T'
//  [1000]  
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

// every 4bit is one block in line. 12 width x 20 height
int64_t field[21];

uint32_t colors[] = { 0x000000FF, 0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFFFF00FF, 0x00FFFFFF, 0xFFFFFFFF };

int8_t rotate_block(int8_t _id_block, int8_t _rotation)
{
	return ((tetromino[_id_block].rot + _rotation) & (tetromino[_id_block].num_temps - 1));
}


bool does_piece_fit_in(int8_t _tet_piece, int8_t _rot, int8_t _position_col, int8_t _position_row)
{

	for (size_t row = 0; row < 4; row++)
	{
		for (size_t col = 0; col < 4; col++)
		{

			// if any doesn't fit return false
			if (_position_row + row >= 0 && _position_row + row < 21/*it is field height with bounds*/)
			{
				if (_position_col + col >= 0 && _position_col + col < 12/*it is field width*/)
				{
					if (((field[_position_row + row ] >> ((_position_col + col) * 4) & 0b1111) != 0) && (((tetromino[_tet_piece].shapes[_rot] >> (row * 4 + col)) & 0b1) != 0))
					return false;

				}
			}
		}
	}

	return true;
}

void lock_the_piece(int8_t _id_piece, int8_t _rot, int8_t _col_x, int8_t _row_y)
{
	for (size_t row = 0; row < 4; row++)
	{
		for (size_t col = 0; col < 4; col++)
		{
			// if any doesn't fit return false
			if (_row_y + row >= 0 && _row_y + row < 21/*it is field height with bounds*/)
			{
				if (_col_x + col >= 0 && _col_x + col < 12/*it is field width*/)
				{
					if (((field[_row_y + row] >> ((_col_x + col) * 4) & 0b1111) == 0) && ((tetromino[_id_piece].shapes[_rot] >> (row * 4 + col)) & 0b1) != 0)
						field[_row_y + row] |= ((uint64_t)(_id_piece + 1) & 0b1111)<<((_col_x + col) * 4);
				}
			}
		}
	}
}


int new_rand_piece()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 6);

	return dis(gen);
}

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
		buffer[5 * screen_width + 25 + i] = score[i];
	}

	for (size_t row = 0; row < 8; row++)
	{
		for (size_t col = 0; col < 7; col++)
		{
			if(row == 0 || row == 7 || col == 0 || col == 6)
				buffer[(row + 8) * screen_width + (col + 23)] = L" ABCDEFGH=#"[10];
		}
	}

	// game logic

	bool b_gameover = false;

	int8_t n_current_x = 4;
	int8_t n_current_y = 0;

	int8_t current_player_piece = new_rand_piece();
	int8_t current_rot = 0;
	int8_t next_piece = new_rand_piece();;
	
	for (size_t i=0; i < 21; i++)
	{
		field[i] = 0x0A0000000000A;
	}
	field[20] = 0x0AAAAAAAAAAAA;

	int n_speed_game = 20;
	int n_count_tick_game = 0;
	int n_score = 0;

	bool b_force_down = false;

	std::vector<int> v_full_lines;
	bool b_delet_full_line = false;
	int n_count_tick_show_full_line = 0;
	int n_line_count = 0;

	//keys
	bool b_key[7];
	bool b_rot_but_press = false;
	
	
	while (!b_gameover)
	{


		// game timing
		Sleep(50);

		n_count_tick_game++;
		b_force_down = (n_speed_game == n_count_tick_game);
		// start counting time to delete full line (== 400ms)
		b_delet_full_line = (!v_full_lines.empty() && (n_count_tick_show_full_line == 8));
		n_count_tick_show_full_line += (!v_full_lines.empty()) ? 1 : 0;
		// input processing

		for (size_t k = 0; k < 7; k++)
			b_key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28ZXRQ"[k]))) != 0;
		

		// input handle - movement
		n_current_x += (b_key[0] && does_piece_fit_in(current_player_piece, current_rot, n_current_x + 1, n_current_y)) ? 1 : 0;
		n_current_x -= (b_key[1] && does_piece_fit_in(current_player_piece, current_rot, n_current_x - 1, n_current_y)) ? 1 : 0;
		if (b_key[2] && does_piece_fit_in(current_player_piece, current_rot, n_current_x, n_current_y + 1))
		{
			n_current_y ++;
			n_score++;
		}
		// rotation
		if (b_key[3] || b_key[4])
		{
			if (b_key[3] && does_piece_fit_in(current_player_piece, rotate_block(current_player_piece, current_rot -1), n_current_x, n_current_y)) {
				if (!b_rot_but_press) current_rot = rotate_block(current_player_piece, current_rot - 1);

			}
			if (b_key[4] && does_piece_fit_in(current_player_piece, rotate_block(current_player_piece, current_rot + 1), n_current_x, n_current_y))
			{
				if(!b_rot_but_press) current_rot = rotate_block(current_player_piece, current_rot + 1);

			};
			b_rot_but_press = true;
		}
		else
		{
			b_rot_but_press = false;
		}
		
		if (b_key[5])
		{
			current_player_piece = new_rand_piece();
			current_rot = 0;

			n_current_x = 4;
			n_current_y = 0;

			next_piece = new_rand_piece();

			n_score = 0;

			for (size_t i = 0; i < 21; i++)
			{
				field[i] = 0x0A0000000000A;
			}
			field[20] = 0x0AAAAAAAAAAAA;
			v_full_lines.clear();
		}

		b_gameover = b_key[6];

		// game mechanic

		if (b_force_down)
		{
			n_count_tick_game = 0;


			if (does_piece_fit_in(current_player_piece, current_rot, n_current_x, n_current_y + 1))
			{
				n_current_y++;
			}
			else
			{
				// it can't progress down. Lock the piece
				lock_the_piece(current_player_piece, current_rot, n_current_x, n_current_y);

				// check logic for full lines
				int check_rows = (n_current_y + 4 < 21) ? n_current_y + 4 : 20;
				for (size_t rows = n_current_y; rows < check_rows; rows++)
				{
					bool b_check_line = true;
					// stay true if all block full
					for (size_t cols = 1; cols < 11; cols++)
					{
						b_check_line &= (field[rows] >> (cols * 4) & 0b1111) != 0;
					}
					// if true preper to remove
					if (b_check_line)
					{
						field[rows] = 0x0A9999999999A;
						v_full_lines.push_back(rows);
						n_line_count++;
					}
				}

				// give player new piece to play with
				current_player_piece = next_piece;
				current_rot = 0;

				n_current_x = 4;
				n_current_y = 0;

				next_piece = new_rand_piece();

				// but check if new piec have place for new round
				n_score += 25;
				if (!v_full_lines.empty()) n_score += (1 << v_full_lines.size()) * 100;
				
				if (n_line_count % 10 == 0)
					if (n_speed_game >= 5) n_speed_game--;

				b_gameover = !does_piece_fit_in(current_player_piece, current_rot, n_current_x, n_current_y);
			}
		}

		if (b_delet_full_line)
		{
			int move_line = 0;
			for (int row = v_full_lines[v_full_lines.size() - 1]; row >= 0; row--)
			{
				if (row == (!v_full_lines.empty() ? v_full_lines[v_full_lines.size() - 1] : -1))
				{
					move_line++;
					if(!v_full_lines.empty()) v_full_lines.pop_back();
				}
				else
				{
					field[row + move_line] = field[row];
					field[row] = 0x0A0000000000A;
				}
			}

			n_count_tick_show_full_line = 0;
		}



		// DISPLAY DRAWING
		// draw 'field' of game(include boarders)
		for (size_t row = 0; row < 21; row++)
		{
			for (size_t col = 0; col < 12; col++)
			{
				buffer[(row + 2) * screen_width + (col + 2)] = L" ABCDEFGH=#"[(field[row]>>((col)*4))&0b1111];
			}
		}

		for (size_t row = 0; row < 4; row++)
		{
			for (size_t col = 0; col < 4; col++)
			{
				if (((tetromino[current_player_piece].shapes[current_rot] >> (row * 4 + (col))) & 1) != 0)
				{
					buffer[(row + 2 + n_current_y) * screen_width + (col + 2 + n_current_x)] = L" ABCDEFGH=#"[current_player_piece + 1];
				};
			}
		}

		// draw score 
		swprintf_s(&buffer[6 * screen_width + 25], 16, L"%15d", n_score);

		// draw next piece

		for (size_t row = 0; row < 4; row++)
		{
			for (size_t col = 0; col < 4; col++)
			{
				buffer[(row + 10) * screen_width + (col + 25)] = L" ABCDEFGH=#"[((tetromino[next_piece].shapes[0] >> (row * 4 + (col))) & 1) != 0 ? next_piece + 1 : 0];

//				if (((tetromino[next_piece].shapes[0] >> (row * 4 + (col))) & 1) != 0)
//				{
//					buffer[(row + 10) * screen_width + (col + 25)] = L" ABCDEFGH=#"[next_piece + 1];
//				}
//				else
//				{
//					buffer[(row + 10) * screen_width + (col + 25)] = L" ABCDEFGH=#"[0];
//				}
			}
		}
		
		// display frame
		WriteConsoleOutputCharacter(screen_buff, buffer, screen_width * screen_height, { 0,0 }, &dwBitesWritten);
	}
	
	CloseHandle(screen_buff);

	std::cout << "\nSCORE: "<< std::to_string(n_score) << "\nGAME OVER\n\n";
	return 0;
}
