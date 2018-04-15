#ifndef MINIMAXTREE_H_INCLUDED
#define MINIMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "checkers.h"
#include "BoardEval.h"

#include "defs.h"

class MinimaxTree
{
public:
	MinimaxTree(std::string &theBoard, int depth, bool redPlayer);
	MinimaxTree(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net);

	void init(std::string &theBoard, int depth, bool redPlayer);

	std::string getBestBoard()
	{
		return bestBoard_;
	}

private:
	MinimaxTree(bool redPlayer, NeuralNet *net, bool usingPieceCount) :
		redPlayerTurn_(redPlayer),
		net_(net),
		usingPieceCount_(usingPieceCount) 
	{};

	// int because basicBoardEval just returns an int
	// change to float when we hook up NN
	float minimaxRecurse(std::string &theBoard, int depth, bool maximizingPlayer);

  private:
	std::string bestBoard_;

	bool redPlayerTurn_;
	NeuralNet *net_;
	bool usingPieceCount_;

	Clock timer_;
	std::ostream *os_;
};

#endif
