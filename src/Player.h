// Player.h

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <math.h>
#include <memory>

#include "MinMaxTree.h"
#include "MinimaxWithAlphaBeta.h"
#include "../CS441AdvancedNN/BasicNN.h"

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class Player
{
public:
	Player( bool redTeam, std::shared_ptr<Clock> clock ) : redTeam_(redTeam), clock_(clock) {};
	Player( bool redTeam, std::shared_ptr<Clock> clock, BasicNN *net) : 
		redTeam_(redTeam), 
		clock_(clock),
		net_(net) {};

	std::string getMove(std::string &theBoard, std::ostream *os, int depth = 4);
private:
	bool redTeam_;
	std::shared_ptr<Clock> clock_;
	BasicNN *net_;
};

#endif