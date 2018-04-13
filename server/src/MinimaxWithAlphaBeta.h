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
#include "defs.h"
#include "BoardEval.h"

// typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class MinimaxWithAlphaBeta {
public:
	MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, std::shared_ptr<Clock> clock, NeuralNet *net, bool usingPieceCount);

	std::string getBestBoard(std::ostream *os = &std::cout);
	void printABStats(std::ostream *os = &std::cout);

private:
	MinimaxWithAlphaBeta(bool redPlayer, std::shared_ptr<Clock> clock, NeuralNet *net, bool usingPieceCount) : 
		redPlayerTurn_(redPlayer),
		clock_(clock),
		breakAlpha_(0),
		breakBeta_(0),
		boardExpansions_(0),
		net_(net),
		usingPieceCount_(usingPieceCount)
	{};

	void init(std::string &theBoard, int depth, bool redPlayer);

	float minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, float alpha, float beta, bool maximizingPlayer);

private:
	std::string bestBoard_;

	bool redPlayerTurn_;
	std::shared_ptr<Clock> clock_;
	uint64_t breakAlpha_;
	uint64_t breakBeta_;
	uint64_t boardExpansions_;
	NeuralNet *net_;
	bool usingPieceCount_;

	std::ostream *os_;
};

#endif