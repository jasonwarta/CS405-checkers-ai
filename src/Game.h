// Game.h

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <math.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <memory>
#include <ostream>
#include <queue>

#include "Player.h"
#include "threadUtils.h"
#include "consts.h"
#include "BoardEval.h"

#include "defs.h"
#include "MinimaxWithAlphaBeta.h"

class Game
{
public:
	Game(Player *red, Player *black, std::ostream *os) : 
		red_(red), 
		black_(black), 
		os_(os)
	{};

	char run(std::string theBoard = START_BOARD_STRING);

private:
	Player * red_;
	Player * black_;
	std::ostream *os_;
	
	std::queue<std::string> moveTracker_;
};

#endif