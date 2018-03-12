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

#include "Player.h"
#include "threadUtils.h"
#include "consts.h"
#include "MinMaxTree.h"
#include "MinimaxWithAlphaBeta.h"

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class Game
{
public:
	Game(Player *red, Player *black, std::shared_ptr<Clock> clock, std::ostream *os) : 
		red_(red), 
		black_(black), 
		clock_(clock),
		os_(os)
	{};

	char run();

private:
	Player * red_;
	Player * black_;
	std::ostream *os_;

	std::shared_ptr<Clock> clock_;
};

#endif