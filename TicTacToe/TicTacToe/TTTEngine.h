#pragma once


#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <conio.h>

class TTTEngine
{
public:
	TTTEngine();
	~TTTEngine();

	void run();

private:

	// checking if is request to turn off engine
	bool closeEngine();
	void terminateEngine();

	// render the screen game
	void render();
	void update_mapString();

	// get and process keys for game
	void processingInput();
	unsigned int get_key();
	bool set_vKey(unsigned int vKey);

	// reseting engine game
	void restartEngine();

	// additional function for keys processing, screen updating
	void enter_action();
	bool check_condition();
	bool positionCheck_b(unsigned int pos);
	std::string positionCheck(unsigned int pos);
	std::string playerCheck(unsigned int pos);
	void markPosition(unsigned int pos);


protected:
	std::bitset<7> game_system;
	int positionMenu = 4;
	std::bitset<9> pool;

	std::vector<std::bitset<9>> players_pools{ 0,0 };

	std::string poolMarks;

	std::vector<std::string> lines; // 17x11
	std::map<unsigned int, std::string*> mapString;


private:
	// 8 condition to win
	std::vector<unsigned long> winCondition{ 7,56,73,84,146,273,292,448 };

};


enum
{
	TTT_INPUT_KEY_SELECT = 13,
	TTT_INPUT_KEY_ESC = 27,
	TTT_INPUT_KEY_UP = 72,
	TTT_INPUT_KEY_LEFT = 75,
	TTT_INPUT_KEY_RIGHT = 77,
	TTT_INPUT_KEY_DOWN = 80,
	TTT_INPUT_KEY_R = 82,
	TTT_INPUT_KEY_r = 114,

};

enum
{
	TTT_SYSTEM_PLAYER_TURN = 0,
	TTT_SYSTEM_BLOCK_OCCUPIED = 1, // for message info if player try take occupied block
	TTT_SYSTEM_RESTRICTED_KEYS = 2, // accept only ESC, restart
	TTT_SYSTEM_WIN_PLAYER = 3, //  which player won
	TTT_SYSTEM_SHUTDOWN = 4, // turn off
	TTT_SYSTEM_START = 5, // only for one jump over for keys processing
	TTT_SYSTEM_RESET_GAME = 6,
	TTT_SYSTEM_R = 7, // not restricted keys
	TTT_SYSTEM_ESC = 8, // 
	TTT_SYSTEM_SELECT = 9, // restricted keys
	TTT_SYSTEM_UP = 10, // 
	TTT_SYSTEM_DOWN = 11, // 
	TTT_SYSTEM_LEFT = 12, // 
	TTT_SYSTEM_RIGHT = 13, // 
};
