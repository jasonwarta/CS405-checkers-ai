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
#include <utility>

#include "checkers.h"
#include "defs.h"
#include "BoardEval.h"

class MinimaxWithAlphaBeta {
public:
  MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, NeuralNet *net);
  MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, NeuralNet *net, uint threads);
  MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer);
  MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, bool usingIDS);
  MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, NeuralNet *net, bool usingIDS);

  std::string getBestBoard(std::ostream *os = &std::cout);
  void printABStats(std::ostream *os = &std::cout);

private:
	MinimaxWithAlphaBeta(bool redPlayer, NeuralNet *net, bool usingPieceCount, bool usingIDS = USING_ITERATIVE_DEEPENING) : 
		redPlayerTurn_(redPlayer),
		breakAlpha_(0),
		breakBeta_(0),
		boardExpansions_(0),
		net_(net),
		usingPieceCount_(usingPieceCount),
		usingIterativeDeepening_(usingIDS)
	{};

	void init(std::string &theBoard, uint depth, bool redPlayer);
	void threadedInit(std::string &theBoard, uint depth, bool redPlayer, uint threads);

	float minimaxWithAlphaBetaRecursive(std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);
	float threadedMinimaxWithAlphaBetaRecursive(NeuralNet *net, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);

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