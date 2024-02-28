#include "TGEngine.h"


int new_rand_piece()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 7);

	return dis(gen);
}

int8_t rotate_block(int8_t _id_block, int8_t _rotation)
{
	return ((tetromino[_id_block].rot + _rotation) & (tetromino[_id_block].num_temps - 1));
}

bool does_piece_fit_in(Game_session& _core, int8_t _rot, int8_t _position_col, int8_t _position_row)
{

	int8_t n_rot = rotate_block(_core.current_player_piece, _core.current_rot + _rot);

	for (int8_t row = 0; row < 4; row++)
	{
		int8_t n_row = _core.n_current_y + _position_row + row;

		for (int8_t col = 0; col < 4; col++)
		{
			int8_t n_col = _core.n_current_x + _position_col + col;


			// if any doesn't fit return false
			if (n_row >= 0 && n_row < 21/*it is field height with bounds*/)
			{
				if (n_col >= 0 && n_col < 12/*it is field width*/)
				{
					if (((_core.field[n_row] >> ((n_col) * 4) & 0b1111) != 0) && (((tetromino[_core.current_player_piece].shapes[n_rot] >> (row * 4 + col)) & 0b1) != 0))
						return false;

				}
			}
		}
	}

	return true;
}

void lock_the_piece(Game_session& _core)
{
	for (int8_t row = 0; row < 4; row++)
	{
		int8_t n_row = _core.n_current_y + row;

		for (int8_t col = 0; col < 4; col++)
		{
			int8_t n_col = _core.n_current_x + col;
			// if any doesn't fit return false
			if (n_row >= 0 && n_row < 21)
			{
				if (n_col >= 0 && n_col < 12)
				{
					if (((_core.field[n_row] >> ((n_col) * 4) & 0b1111) == 0) && ((tetromino[_core.current_player_piece].shapes[_core.current_rot] >> (row * 4 + col)) & 0b1) != 0)
						// 4bit range (0-15) masked form 64bit to prevent overwriting bits outside it expeted place 
						_core.field[n_row] |= ((uint64_t)(_core.current_player_piece) & 0b1111) << ((n_col) * 4);
				}
			}
		}
	}
}

void get_new_piece(Game_session& _core)
{
	_core.current_player_piece = _core.next_piece;
	_core.current_rot = 0;

	_core.n_current_x = DEFAULT_GAME_START_X;
	_core.n_current_y = DEFAULT_GAME_START_Y;

	_core.next_piece = new_rand_piece();
}

void reset(Game_session& _core)
{

	for (int8_t i = 0; i < 21; i++)
	{
		_core.field[i] = 0x0A0000000000A;
	}
	_core.field[20] = 0x0AAAAAAAAAAAA;

	_core.b_gameover = false;
	_core.b_force_down = false;

	_core.n_speed_game = DEFAULT_GAME_SPEED;
	_core.n_count_tick_game = 0;
	_core.n_score = 0;

	_core.v_full_lines.clear();
	_core.b_delet_full_line = false;
	_core.n_count_tick_show_full_line = 0;
	_core.n_cleaned_lines_count = 0;
	_core.n_level_count = 1;
	_core.m_last_time = GetTickCount();

	// player status
	_core.next_piece = new_rand_piece();
	get_new_piece(_core);

	// keys status
	for (int8_t k = 0; k < 7; k++)
		_core.b_key[k] = false;
	_core.b_rot_but_press = false;
}

void game_tick(Game_session& _core)
{
	uint32_t now_tick = GetTickCount();
	if (now_tick - _core.m_last_time > 50 || _core.m_b_no_wait)
	{
		_core.n_count_tick_game++;
		_core.b_force_down = (_core.n_speed_game == _core.n_count_tick_game);
		// start counting time to delete full line (== 400ms)
		_core.b_delet_full_line = (!_core.v_full_lines.empty() && (_core.n_count_tick_show_full_line == 8));
		_core.n_count_tick_show_full_line += (!_core.v_full_lines.empty()) ? 1 : 0;

		// input handle - movement
		move_piece(_core);

		// game mechanic

		if (_core.b_force_down)
		{
			forcedown_piece(_core);
		}

		if (_core.b_delet_full_line)
		{
			delete_fulllines(_core);
		}

		_core.m_last_time = now_tick;
	}
}

void move_piece(Game_session& _core)
{
	if (_core.b_key[2] && does_piece_fit_in(_core, 0, 0, 1))
	{
		_core.n_current_y++;
		_core.n_score++;
	}

	_core.n_current_x += (_core.b_key[0] && !_core.b_rot_but_press && does_piece_fit_in(_core, 0, 1, 0)) ? 1 : 0;
	_core.n_current_x -= (_core.b_key[1] && !_core.b_rot_but_press && does_piece_fit_in(_core, 0, -1, 0)) ? 1 : 0;
	
		// rotation
	if (_core.b_key[3] || _core.b_key[4])
	{

		if (_core.b_key[3] && !_core.b_rot_but_press && does_piece_fit_in(_core, 1, 0, 0)) {
			_core.current_rot = rotate_block(_core.current_player_piece, _core.current_rot + 1);
		}

		if (_core.b_key[4] && !_core.b_rot_but_press && does_piece_fit_in(_core, -1, 0, 0))
		{
			_core.current_rot = rotate_block(_core.current_player_piece, _core.current_rot - 1);
		};

		_core.b_rot_but_press = true;
	}
	else
	{
		_core.b_rot_but_press = false;
	}

	if (_core.b_key[5])
	{
		reset(_core);
	}

	_core.b_gameover = _core.b_key[6];
}

void forcedown_piece(Game_session& _core)
{
	_core.n_count_tick_game = 0;


	if (does_piece_fit_in(_core, 0, 0, 1))
	{
		_core.n_current_y++;
	}
	else
	{
		// it can't progress down. Lock the piece
		lock_the_piece(_core);

		// check logic for full lines
		int check_rows = (_core.n_current_y + 4 < 21) ? _core.n_current_y + 4 : 20;
		for (int rows = _core.n_current_y; rows < check_rows; rows++)
		{
			bool b_check_full_line = true;
			// stay true if all block full
			for (int cols = 1; cols < 11; cols++)
			{
				b_check_full_line &= (_core.field[rows] >> (cols * 4) & 0b1111) != 0;
			}
			// if true preper to remove
			if (b_check_full_line)
			{
				_core.field[rows] = 0x0A9999999999A;
				_core.v_full_lines.push_back(rows);
				_core.n_cleaned_lines_count++;
			}
		}

		// give player new piece to play with
		get_new_piece(_core);

		// but check if new piec have place for new round
		_core.n_score += 25;
		if (!_core.v_full_lines.empty()) 
		{
			// give score
			_core.n_score += (1 << _core.v_full_lines.size()) * 100;

			// check level
			int  level_check = _core.n_cleaned_lines_count / DEFAULT_SPEED_LEVEL_UP;
			// adjust level
			if(_core.n_level_count == level_check)
			{
				_core.n_level_count++;
				if (_core.n_speed_game > DEFAULT_GAME_MIN_SPEED) _core.n_speed_game= DEFAULT_GAME_SPEED - level_check;
			}
		};

		_core.b_gameover = !does_piece_fit_in(_core, 0, 0, 0);
	}
}

void delete_fulllines(Game_session& _core)
{
	int move_line = 0;
	for (int row = _core.v_full_lines[_core.v_full_lines.size() - 1]; row >= 0; row--)
	{
		if (row == (!_core.v_full_lines.empty() ? _core.v_full_lines[_core.v_full_lines.size() - 1] : -1))
		{
			move_line++;
			if (!_core.v_full_lines.empty()) _core.v_full_lines.pop_back();
		}
		else
		{
			_core.field[row + move_line] = _core.field[row];
			_core.field[row] = 0x0A0000000000A;
		}
	}

	_core.n_count_tick_show_full_line = 0;
}
