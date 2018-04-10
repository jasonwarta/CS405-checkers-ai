#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "checkers.h"
#include "BoardEval.h"

#include "BasicNN.h"

class MinMaxTree
{
public:

	MinMaxTree(std::string &theBoard, int depth, bool redPlayer, BasicNN *net)
	{
		redPlayerTurn_ = redPlayer;
		net_ = net;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());
		// Delete tempBoard here

		if(possBoards.size() == 0) 
		{
			bestBoard_ = theBoard;
			return;
		}

		int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, false, -10000);
		bestBoard_ = possBoards[0];

		for(int i=1; i<possBoards.size(); ++i) 
		{
			int value = minMaxTreeRecurse(possBoards[i], depth-1, false, -10000);
			if(value > bestValue) 
			{
				bestValue = value;
				bestBoard_ = possBoards[i];
			}
		}
		//std::cout << theBoard <<  " - Best Board: " << bestValue << std::endl;
	}

	std::string getBestBoard()
	{
		return bestBoard_;
	}

private:
	// int because basicBoardEval just returns an int
	// change to float when we hook up NN
	int minMaxTreeRecurse(std::string &theBoard, int depth, bool maximizingPlayer, int currentBest)
	{
		// std::cout << depth << std::endl;

		if(depth == 0) // maybe other checks here later?...
		{
			// net_->printData();
			net_->evaluateNN(theBoard);
			// std::cout << net_->getLastNode() << " " << theBoard << std::endl;
			return net_->getLastNode();
			// return basicBoardEval(theBoard, redPlayerTurn_);
		}

		if(maximizingPlayer)
		{
			CheckerBoard tempBoard(theBoard, redPlayerTurn_);
			std::vector<std::string> possBoards;
			possBoards = std::move(tempBoard.getAllRandoMoves());
			// delete class here later

			if(possBoards.size() == 0)
			{
				return -10000;
			}

			int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, false, -10000);

			for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
				bestValue = std::max(bestValue, minMaxTreeRecurse(*it,depth-1,false,bestValue));
			}
			return bestValue;
		}
		else // NOT maximizingPlayer
		{
			CheckerBoard tempBoard(theBoard, !redPlayerTurn_);
			std::vector<std::string> possBoards;
			possBoards = std::move(tempBoard.getAllRandoMoves());
			// delete class here later

			if(possBoards.size() == 0)
			{
				return 10000;
			}

			int worstValue = minMaxTreeRecurse(possBoards[0], depth-1, true, 10000);

			for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
				worstValue = std::min(worstValue, minMaxTreeRecurse(*it,depth-1,true,worstValue));
			}
			return worstValue;
		}
	}

private:
	std::string bestBoard_;
	bool redPlayerTurn_;

	BasicNN *net_;
};

#endif


