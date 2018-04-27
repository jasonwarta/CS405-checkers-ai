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
#include <memory>
#include <mutex>
#include <thread>
#include <queue>
#include <functional>
#include <utility>

#include "checkers.h"
#include "defs.h"
#include "BoardEval.h"

class MinimaxWithAlphaBeta {
public:
	MinimaxWithAlphaBeta(std::string &theBoard, bool redPlayer, uint minDepth, uint maxDepth, NeuralNet *net = nullptr);

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
		usingIterativeDeepening_(usingIDS),
		maxDepthReached_(0)
	{};

	void init(std::string &theBoard, uint minDepth, uint maxDepth, bool redPlayer);
	void init(std::string &theBoard, uint depth, bool redPlayer);
	void threadedInit(std::string &theBoard, uint depth, bool redPlayer, uint numThreads);

	friend void threadManager(std::queue<std::string> & boards, std::mutex & mtx, MinimaxWithAlphaBeta *self, NeuralNet *net, std::vector<std::pair<float,std::string>> &results, uint depth);
	friend float minimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);
	friend float threadedMinimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, NeuralNet *net, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);

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
	uint maxDepthReached_;
};
extern void threadManager(std::queue<std::string> & boards, std::mutex & mtx, MinimaxWithAlphaBeta *self, NeuralNet *net, std::vector<std::pair<float,std::string>> &results, uint depth);
extern float minimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);
extern float threadedMinimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, NeuralNet *net, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer);

#endif

