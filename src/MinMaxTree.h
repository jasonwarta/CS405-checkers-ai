#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "checkers.h"
#include "BasicBoardEval.h"


class MinMaxTree
{
public:
	// will crash if there is no possible board to go down
	// add fix later
	MinMaxTree(std::string theBoard, int searchDepth, bool redPlayer)
	{
		depth_ = searchDepth;
		redPlayerTurn_ = redPlayer;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards;
		possBoards = std::move(tempBoard.getAllRandoMoves());
		// Delete tempBoard here

		depth_--;

		bestBoard_ = possBoards[0];
		int bestValue = minMaxTreeRecurse(possBoards[0], false);
		for(int index = 1; index < possBoards.size(); ++index) 
		{
			int value = minMaxTreeRecurse(possBoards[index], false);
			if(value > bestValue) 
			{
				bestValue = value;
				bestBoard_ = possBoards[index];
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
	int minMaxTreeRecurse(std::string theBoard, bool maximizingPlayer)
	{
		if(depth_ == 0) // maybe other checks here later?...
		{
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
		depth_--;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards;
		possBoards = std::move(tempBoard.getAllRandoMoves());
		// delete class here later


		if(maximizingPlayer)
		{
			int bestValue = minMaxTreeRecurse(possBoards[0], false);
			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], false);
				bestValue = std::max(bestValue, v);
			}
			return bestValue;
		}
		else // NOT maximizingPlayer
		{
			int worstValue = minMaxTreeRecurse(possBoards[0], true);
			for(int i=1; i<possBoards.size(); ++i)
			{
				int v = minMaxTreeRecurse(possBoards[i], true);
				worstValue = std::min(worstValue, v);
			}
			return worstValue;
		}
}


private:
	std::string bestBoard_;
	int depth_;
	bool redPlayerTurn_;

};




#endif


