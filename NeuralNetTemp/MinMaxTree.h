

#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED


#include <string>
#include <algorithm>
#include "../src/checkers.h"
#include "BasicBoardEval.h"


int minMaxTreeRecurse(std::string theBoard, int depth, bool redTeamTurn)
{
	if(depth == 0) // Maybe add aditional break if board gets too bad later?
					// also break if no moves possible
	{
		return basicBoardEval(theBoard, redTeamTurn);
	}

	CheckerBoard tempBoard(theBoard, redTeamTurn);
	std::vector<std::string> possBoards;
	possBoards = std::move(tempBoard.getAllRandoMoves());
	// Delete tempBoard Here

	// Have at least one "best value" stored, i starts at 1
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, !redTeamTurn);
	for(int i=1; i<possBoards.size(); ++i)
	{
		int v = minMaxTreeRecurse(possBoards[i], depth-1, !redTeamTurn);
		bestValue = std::max(bestValue, v);
	}
	// Because redTeamTurn flips, this returns your best move, then opps best move, etc...
	return bestValue;
}


std::string minMaxTreeBase(std::string theBoard, int depth, bool redTeamTurn)
{

	CheckerBoard tempBoard(theBoard, redTeamTurn);
	std::vector<std::string> possBoards;
	possBoards = std::move(tempBoard.getAllRandoMoves());
	// Delete tempBoard here


	int indexBestBoard = 0;
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, redTeamTurn);
	for(int i=1; i<possBoards.size(); ++i)
	{
		int v = minMaxTreeRecurse(possBoards[i], depth-1, redTeamTurn);

		if(v>bestValue)
		{
			bestValue = v;
			indexBestBoard = i;
		}
	}

	return possBoards[indexBestBoard];
}

#endif


/* ----------TEMP BACKUP! MAY DELETE LATER. TESTING POSSIBLE SHORTCUT ABOVE---------


int minMaxTreeRecurse(std::string theBoard, int depth, bool redTeamTurn, bool maximizingPlayer)
{
	if(depth == 0) // Maybe add aditional break if board gets too bad later?
					// also break if no moves possible
	{
		return basicBoardEval(theBoard);
	}

	CheckerBoard tempBoard(theBoard, redTeamTurn);
	const std::vector<std::string>> possBoards = std::move(tempBoard.getAllRandoMoves());

	if(maximizingPlayer)
	{

		int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, !redTeamTurn, false);
		for(int i=1; i<possBoards.size(); ++i)
		{
			int v = minMaxTreeRecurse(possBoards[i], depth-1, !redTeamTurn, false);
			bestValue = std::max(bestValue, v);
		}
		return bestValue;
	}
	else // NOT maximizingPlayer
	{
		int worstValue = minMaxTreeRecurse(possBoards[0], depth-1, !redTeamTurn, true);
		for(int i=1; i<possBoards.size(); ++i)
		{
			int v = minMaxTreeRecurse(possBoards[i], depth-1, !redTeamTurn, true);
			worstValue = std::min(worstValue, v);
		}
		return worstValue;
	}
}


std::string minMaxTreeBase(std::string theBoard, int depth, bool redTeamTurn)
{

	CheckerBoard tempBoard(theBoard, redTeamTurn);
	const std::vector<std::string>> possBoards = std::move(tempBoard.getAllRandoMoves());

	int indexBestBoard = 0;
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, redTeamTurn, true)

	for(int i=1; i<possBoards.size(); ++i)
	{
		int v = minMaxTreeRecurse(possBoards[i], depth-1, redTeamTurn, true);
		if(v>bestValue)
		{
			bestValue = v;
			indexBestBoard = i;
		}
	}

	return possBoards[indexBestBoard];
}
*/




