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
#include <vector>

#include "checkers.h"
#include "defs.h"
#include "BoardEval.h"

class MinimaxWithAlphaBeta {
public:
    MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net, Clock *theClock);
    MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net);
    MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer);

    std::string getBestBoard(std::ostream *os = &std::cout);
    std::vector<std::string> getBestVector();
    void printABStats(std::ostream *os = &std::cout);

private:
	MinimaxWithAlphaBeta(bool redPlayer, NeuralNet *net, bool usingPieceCount) : 
		redPlayerTurn_(redPlayer),
		breakAlpha_(0),
		breakBeta_(0),
		boardExpansions_(0),
		net_(net),
		usingPieceCount_(usingPieceCount)
	{};

	void init(std::string &theBoard, int depth, bool redPlayer, Clock *TheClock = nullptr);

	float minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, float alpha, float beta, bool maximizingPlayer);

private:
	std::string bestBoard_;

	bool redPlayerTurn_;
	uint64_t breakAlpha_;
	uint64_t breakBeta_;
	uint64_t boardExpansions_;
	NeuralNet *net_;
	bool usingPieceCount_;
	bool usingIterativeDeepening_;
	Clock timer_;
	std::ostream *os_;
	std::vector<std::string> bestVector_;
};

#endif