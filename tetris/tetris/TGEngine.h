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


#pragma once

#ifndef TGENGINE
#define TGENGINE


#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <random>
#include <stdio.h>

#define DEFAULT_GAME_SPEED 30			// GAME STARTING AND MIN SPEED
#define DEFAULT_GAME_MIN_SPEED 4		// GAME STARTING AND MIN SPEED
#define DEFAULT_SPEED_LEVEL_UP 20		// EVERY 'X' CLEANED LINES
#define DEFAULT_GAME_START_X 4			// GAME STARTING POS X AND Y
#define DEFAULT_GAME_START_Y 0			// GAME STARTING POS X AND Y


struct Shape
{
	uint8_t num_temps;
	uint8_t rot;
	uint16_t* shapes;
};

// one shape for all game session so one static object is enough
static Shape tetromino[] =
{
	{0, 0, 0},														// empty temp
	{1, 0, new uint16_t(0x0660)},									// square
	{2, 0, new uint16_t[2] { 0x2222 , 0x00F0} },					// I - block
	{2, 0, new uint16_t[2] { 0x0036, 0x0231 } },					// Z - block
	{2, 0, new uint16_t[2] { 0x0063, 0x0132 } },					// S - block
	{4, 0, new uint16_t[4] { 0x0074, 0x0223, 0x0170, 0x0622 } },	// revers L - block
	{4, 0, new uint16_t[4] { 0x0071, 0x0322, 0x0470, 0x0226 } },	// L - block
	{4, 0, new uint16_t[4] { 0x0072, 0x0232, 0x0270, 0x0262 } }		// T - block
};

struct Game_session
{
	// every 4bit is one block in line. 12 width x 20 height
	// value: 0 - empty space, 1-15 - block id. INFO: there is 7 tetromino shapes (1-7), one boundry id and 'full line' id, so 10 id's in total.
	int64_t field[21];

	// game status
	bool b_gameover = false;
	bool b_force_down = false;

	int n_speed_game = DEFAULT_GAME_SPEED;
	int n_count_tick_game = 0;
	int n_score = 0;

	// for deleting lines
	std::vector<int> v_full_lines;
	bool b_delet_full_line = false;
	int n_count_tick_show_full_line = 0;
	int n_cleaned_lines_count = 0;
	int n_level_count = 1;
	uint32_t m_last_time = 0;		//		miliseconds

	// player status
	int8_t n_current_x = DEFAULT_GAME_START_X;
	int8_t n_current_y = DEFAULT_GAME_START_Y;

	int8_t current_player_piece = 0;
	int8_t current_rot = 0;
	int8_t next_piece = 0;

	// keys status
	bool b_key[7]{ 0 };				//		RIGHT, LEFT, DOWN, ROTATE LEFT, ROTATE RIGHT, RESET, QUIT
	bool b_rot_but_press = false;
	bool m_b_no_wait = false;		//		if is true then is no delay -> for NN to train/play

};

void get_new_piece(Game_session& core);
bool does_piece_fit_in(Game_session& core, int8_t _rot, int8_t _position_col, int8_t _position_row);
void forcedown_piece(Game_session& core);
void delete_fulllines(Game_session& core);
void lock_the_piece(Game_session& core);

int new_rand_piece();
void move_piece(Game_session& core);
int8_t rotate_block(int8_t _id_block, int8_t _rotation);

void game_tick(Game_session& core);
void reset(Game_session& core);


#endif // !TGENGINE