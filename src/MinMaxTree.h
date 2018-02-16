#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "checkers.h"
#include "BasicBoardEval.h"


class MinMaxTree
{
public:

	MinMaxTree(std::string theBoard, int depth, bool redPlayer)
	{
		redPlayerTurn_ = redPlayer;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards;
		possBoards = std::move(tempBoard.getAllRandoMoves());
		// Delete tempBoard here

		if(possBoards.size() == 0) 
		{
			//std::cout << "MinMaxTree.h: No possible moves. Setting bestBoard_ = theBoard" << std::endl;
			bestBoard_ = theBoard;
			return;
		}

		int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, true);
		std::cout << possBoards[0] << " - Value: " << bestValue << " D: " << depth-1 << " true"<< std::endl;

		bestBoard_ = possBoards[0];

		for(int i=1; i<possBoards.size(); ++i) 
		{
			int value = minMaxTreeRecurse(possBoards[i], depth-1, true);
			std::cout << possBoards[i] << " - Value: " << value << " D: " << depth-1 << " true"<< std::endl;
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
	int minMaxTreeRecurse(std::string theBoard, int depth, bool maximizingPlayer)
	{

		if(depth == 0) // maybe other checks here later?...
		{
			return basicBoardEval(theBoard, redPlayerTurn_);
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

			int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, false);
			std::cout << possBoards[0] << " - Value: " << bestBalue << " D: " << depth-1 << " false"<< std::endl;

			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], depth-1, false);
				std::cout << possBoards[i] << " - Value: " << v << " D: " << depth-1 << " false"<< std::endl;
				bestValue = std::max(bestValue, v);
			}
			//std::cout << "MP: true, depth: " << depth << ", BestValue: " << bestValue << std::endl;
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

			int worstValue = minMaxTreeRecurse(possBoards[0], depth-1, true);
			std::cout << possBoards[0] << " - Value: " << worstValue << " D: " << depth-1 << " true"<< std::endl;

			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], depth-1, true);
				std::cout << possBoards[i] << " - Value: " << v << " D: " << depth-1 << " true"<< std::endl;

				worstValue = std::min(worstValue, v);
			}
			//std::cout << "MP: false, depth: " << depth << ", WorstValue: " << worstValue << std::endl;
			return worstValue;
		}
	}


private:
	std::string bestBoard_;
	bool redPlayerTurn_;

};




#endif


