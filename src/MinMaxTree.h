#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "../src/checkers.h"
#include "../src/BasicBoardEval.h"


class MinMaxTree
{
public:
	// will crash if there is no possible board to go down
	// add fix later
	MinMaxTree(std::string theBoard, int depth, bool redPlayer)
	{
		redPlayerTurn_ = redPlayer;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards;
		possBoards = std::move(tempBoard.getAllRandoMoves());
		// Delete tempBoard here

		if(possBoards.size() == 0) 
		{
			std::cout << "MinMaxTree.h: No possible moves. bestBoard_ = theBoard" << std::endl;
			bestBoard_ = theBoard;
			return;
		}

		int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, true);
		bestBoard_ = possBoards[0];

		for(int i=1; i<possBoards.size(); ++i) 
		{
			int value = minMaxTreeRecurse(possBoards[i], depth-1, true);
			//std::cout << index << "th value is: " << value << std::endl;
			if(value > bestValue) 
			{
				bestValue = value;
				bestBoard_ = possBoards[i];
			}
		}
	
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
			//std::cout << "BasicBoardEval: " << basicBoardEval(theBoard, redPlayerTurn_) << std::endl;
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
			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], depth-1, false);
				bestValue = std::max(bestValue, v);
			}
			//std::cout << "Recursing level: " << depth_ << " MaxPlayer: True bestValue: " << bestValue << std::endl;
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
			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], depth-1, true);
				worstValue = std::min(worstValue, v);
			}
			//std::cout << "Recursing level: " << depth_ << " MaxPlayer: False worstValue: " << worstValue << std::endl;
			return worstValue;
		}
	}


private:
	std::string bestBoard_;
	bool redPlayerTurn_;

};




#endif


