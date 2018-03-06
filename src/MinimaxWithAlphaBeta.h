// MinimaxWithAlphaBeta.h

#ifndef MINIMAX_WITH_ALPHA_BETA_H_INCLUDED
#define MINIMAX_WITH_ALPHA_BETA_H_INCLUDED

#include <string>
#include <algorithm>
#include <chrono>
#include <math.h>
#include <sstream>
#include <memory>
#include <iomanip>

#include "checkers.h"
#include "BoardEval.h"

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class MinimaxWithAlphaBeta {
public:
	MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, std::shared_ptr<Clock> clock);
	MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, float kingWeight, std::shared_ptr<Clock> clock);

	std::string getBestBoard();
	void printABStats();

private:
	MinimaxWithAlphaBeta(bool redPlayer, std::shared_ptr<Clock> clock) : 
		redPlayerTurn_(redPlayer),
		clock_(clock),
		breakAlpha_(0),
		breakBeta_(0) 
	{};

	void init(std::string &theBoard, int depth);

	template<typename NUM_TYPE>
	NUM_TYPE minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, NUM_TYPE alpha, NUM_TYPE beta, bool maximizingPlayer);

private:
	std::string bestBoard_;
	bool redPlayerTurn_;
	int breakBeta_;
	int breakAlpha_;
	float kingWeight_;

	std::shared_ptr<Clock> clock_;
};

#endif
