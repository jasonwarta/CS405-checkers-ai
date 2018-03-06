// Player.h

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <math.h>
#include <memory>

#include "MinMaxTree.h"
#include "MinimaxWithAlphaBeta.h"

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class Player
{
public:
	Player( bool redTeam, std::shared_ptr<Clock> clock ) : redTeam_(redTeam), kingWeight_(NAN), clock_(clock) {};
	Player( bool redTeam, float kingWeight, std::shared_ptr<Clock> clock) : 
		redTeam_(redTeam), 
		kingWeight_(kingWeight), 
		clock_(clock) {};

	std::string getMove(std::string &theBoard, int depth = 4);
private:
	float kingWeight_;
	bool redTeam_;
	std::shared_ptr<Clock> clock_;
};

#endif