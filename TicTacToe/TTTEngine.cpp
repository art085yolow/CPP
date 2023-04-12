#include "TTTEngine.h"
#include <iomanip>

TTTEngine::TTTEngine()
{

	std::string s1 = "     |     |     ";
	std::string s2 = "\n-----+-----+-----\n";
	std::string insertS;

	/*

			  |         |
	 - > x < -|- > x < -|- > x < -
			  |         |
	 - - - - -+- - - - -+- - - - -
			  |         |
	 - > x < -|- > x < -|- > x < -
			  |         |
	 - - - - -+- - - - -+- - - - -
			  |         |
	 - > x < -|- > x < -|- > x < -
			  |         |

	*/

	this->lines.push_back(s1);
	this->lines.push_back(insertS);
	this->lines.push_back(s1 + s2 + s1);
	this->lines.push_back(insertS);
	this->lines.push_back(s1 + s2 + s1);
	this->lines.push_back(insertS);
	this->lines.push_back(s1);


	this->mapString[0] = &(lines[1]);
	this->mapString[1] = &(lines[3]);
	this->mapString[2] = &(lines[5]);

	restartEngine();

	// bit set to jump over input_keys once.
	this->game_system[TTT_SYSTEM_START] = 1;
}

TTTEngine::~TTTEngine()
{
}

bool TTTEngine::closeEngine()
{
	return this->game_system[TTT_SYSTEM_SHUTDOWN];
}

void TTTEngine::run()
{

	while (!closeEngine())
	{
		if (!this->game_system[TTT_SYSTEM_START]) { processingInput(); }
		else
		{
			this->game_system[TTT_SYSTEM_START] = 0;
		}

		render();
	}

	terminateEngine();
}

void TTTEngine::render()
{
	// clearing console for another render
	system("cls");
	
	// Welcome text and instructions on start
	std::cout << "Welcome! To Exit press ESC. \n Arrows to move. Enter to select. \n Press R to restart game. \n" << std::endl;

	std::cout << "Player turn: " << (this->game_system[0] ? "O" : "X") << "\n\n";

	update_mapString();

	std::string s;

	for (int i = 0; i < 17; i++)
	{
		s += "-";
	}

	std::cout << std::setw(2) << s << std::endl;

	for (size_t i = 0; i < lines.size(); i++)
	{
		std::cout << std::setw(2) << lines[i] << std::endl;

	}

	std::cout << std::setw(2) << s << std::endl;


	// checking if player select occupied block

	if (this->game_system[TTT_SYSTEM_BLOCK_OCCUPIED] == 1)
	{
		std::cout << "This block is occupied! \n";
		this->game_system[TTT_SYSTEM_BLOCK_OCCUPIED] = 0;
	}

	// checking if and which player met win condition

	if (this->check_condition()) {
		std::cout << "Player won " << (this->game_system[TTT_SYSTEM_WIN_PLAYER] ? 'O' : 'X') << "\n\n Press R to restart game or ESC to exit.\n\n";
	}
	else if (this->pool.to_ulong() == 511) // if non met condiotion but all blocks are full
	{
		std::cout << "Draw" << "\n\n Press R to restart game or ESC to exit.\n\n";
		this->game_system[TTT_SYSTEM_RESTRICTED_KEYS] = 1;
	};

}

void TTTEngine::processingInput()
{

	

	bool correctKey = false;

	while (!correctKey)
	{

		switch (this->get_key())
		{
			// checking what keys recived and send further
		case TTT_INPUT_KEY_DOWN:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_DOWN);
		}
		break;
		case TTT_INPUT_KEY_UP:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_UP);
		}
		break;
		case TTT_INPUT_KEY_LEFT:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_LEFT);
		}
		break;
		case TTT_INPUT_KEY_RIGHT:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_RIGHT);
		}
		break;
		case TTT_INPUT_KEY_R:
		case TTT_INPUT_KEY_r:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_R);
		}
		break;
		case TTT_INPUT_KEY_ESC:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_ESC);
		}
		break;
		case TTT_INPUT_KEY_SELECT:
		{
			correctKey = this->set_vKey(TTT_SYSTEM_SELECT);
		}
		break;
		default:
			break;
		}
	}

}

void TTTEngine::terminateEngine()
{
	std::cout << "Bey bey!";
}

void TTTEngine::restartEngine()
{

	this->poolMarks = "         ";

	this->pool.reset();

	for (auto& player : this->players_pools) { player.reset(); };

	this->game_system.reset();
	this->positionMenu = 4;

	this->game_system[TTT_SYSTEM_RESTRICTED_KEYS] = 0;

}



void TTTEngine::update_mapString()
{
	for (size_t idy = 0; idy < 3; idy++)
	{
		*mapString[idy] = "";

		for (size_t idx = 0; idx < 3; idx++)
		{
			
			if ((positionMenu) == (idy * 3 + idx))
			{
				*mapString[idy] += "->" + positionCheck(idy * 3 + idx) + "<-";
			}
			else
			{
				*mapString[idy] += "  " + positionCheck(idy * 3 + idx) + "  ";
			}

			if (idx < 2) *mapString[idy] += "|";
		}

	}

}


bool TTTEngine::positionCheck_b(unsigned int pos)
{
	return pool[pos];
}

std::string TTTEngine::positionCheck(unsigned int pos)
{
	if (!positionCheck_b(pos))
	{
		return " ";
	}

	return playerCheck(pos);
}

std::string TTTEngine::playerCheck(unsigned int pos)
{
	std::string s;
	s = poolMarks[pos];

	return s;
}

void TTTEngine::markPosition(unsigned int pos)
{
	// locking access to block
	pool[pos] = 1;

	// assigned player character to rendered pool
	poolMarks[pos] = this->game_system[0] ? 'O' : 'X';

	// writing down selected block in player pool
	this->players_pools[this->game_system[0]][pos] = 1;

	// change turn
	this->game_system[0] = !this->game_system[0];
}

unsigned int TTTEngine::get_key()
{
	return _getch();;
}

bool TTTEngine::set_vKey(unsigned int v_key)
{
	// checking if game continue
	if (!this->game_system[TTT_SYSTEM_RESTRICTED_KEYS])
	{
		// processing v_key if thier are from restricted list of keys
		switch (v_key)
		{

		case TTT_SYSTEM_DOWN:
		{
			if ((this->positionMenu + 3) < 9)
			{
				this->positionMenu += 3;
			}
		}
		break;
		case TTT_SYSTEM_UP:
		{
			if ((this->positionMenu - 3) >= 0)
			{
				this->positionMenu -= 3;
			}
		}
		break;
		case TTT_SYSTEM_LEFT:
		{
			if ((this->positionMenu - 1) >= 0)
			{
				this->positionMenu -= 1;
			}
		}
		break;
		case TTT_SYSTEM_RIGHT:
		{
			if ((this->positionMenu + 1) < 9)
			{
				this->positionMenu += 1;
			}
		}
		break;
		case TTT_SYSTEM_SELECT:
		{
			this->enter_action();
		}
		break;
		};

	}

	// processing v_key from not restricted list and rest
	switch (v_key)
	{
	case TTT_SYSTEM_R:
	{
		// restarting game
		this->restartEngine();
	}
	break;

	case TTT_SYSTEM_ESC:
	{
		// turn off engine
		this->game_system[TTT_SYSTEM_SHUTDOWN] = 1;
	}
	break;
	};

	return true;
}

bool TTTEngine::check_condition()
{
	// return only proper won state otherwise nothing [0];
	for (unsigned int id = 0; id < 2; id++)
	{
		auto player_pool = this->players_pools[id];

		// writing down bits meting condition AND
		std::bitset<9> ret;

		for (size_t c = 0; c < this->winCondition.size(); c++)
		{
			ret.reset();

			std::bitset<9> wc = this->winCondition[c];

			for (size_t i = 0; i < player_pool.size(); i++)
			{
				if ((wc[i] == 1) && (player_pool[i] == 1))
				{
					ret[i] = 1;
				}
			};

			// checking whether the sifted bits meet the win condition
			if (ret == wc)
			{
				// putting restriction on keys input
				this->game_system[TTT_SYSTEM_RESTRICTED_KEYS] = 1;
				// putting information who won
				this->game_system[TTT_SYSTEM_WIN_PLAYER] = id;
				return true;
			}
		}
	}

	return false;
}

void TTTEngine::enter_action()
{
	if (!this->positionCheck_b(this->positionMenu))
	{
		markPosition(this->positionMenu);
	}
	else {
		this->game_system[TTT_SYSTEM_BLOCK_OCCUPIED] = 1;

	}
}
