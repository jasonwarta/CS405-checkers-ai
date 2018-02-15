#ifndef MINMAXTREE_H_INCLUDED
#define MINMAXTREE_H_INCLUDED

#include <string>
#include <algorithm>
#include "checkers.h"
#include "BasicBoardEval.h"

int minMaxTreeRecurse(std::string theBoard, int depth, bool playerTurn) {
	// Maybe add aditional break if board gets too bad later?
	// also break if no moves possible
	if(depth == 0) {
		return basicBoardEval(theBoard, playerTurn);
	}

	CheckerBoard tempBoard(theBoard, playerTurn);
	std::vector<std::string> possBoards;
	possBoards = std::move(tempBoard.getAllRandoMoves());
	// Delete tempBoard Here

	// Have at least one "best value" stored, i starts at 1
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, !playerTurn);
	for(int index = 1; index < possBoards.size(); ++index) {
		int value = minMaxTreeRecurse(possBoards[index], depth-1, !playerTurn);
		bestValue = std::max(bestValue, value);
	}
	// Because playerTurn flips, this returns your best move, then opps best move, etc...
	return bestValue;
}


std::string minMaxTreeBase(std::string theBoard, int depth, bool playerTurn) {

	CheckerBoard tempBoard(theBoard, playerTurn);
	std::vector<std::string> possBoards;
	possBoards = std::move(tempBoard.getAllRandoMoves());
	// Delete tempBoard here

	int indexBestBoard = 0;
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, playerTurn);
	for(int index = 1; index < possBoards.size(); ++index) {

		int value = minMaxTreeRecurse(possBoards[index], depth-1, playerTurn);
		if(bestValue > value) {
			bestValue = value;
			indexBestBoard = index;
		}
	}
	return possBoards[indexBestBoard];
}
#endif

/* ----------TEMP BACKUP! MAY DELETE LATER. TESTING POSSIBLE SHORTCUT ABOVE---------


int minMaxTreeRecurse(std::string theBoard, int depth, bool playerTurn, bool maximizingPlayer)
{
	if(depth == 0) // Maybe add aditional break if board gets too bad later?
					// also break if no moves possible
	{
		return basicBoardEval(theBoard);
	}

	CheckerBoard tempBoard(theBoard, playerTurn);
	const std::vector<std::string>> possBoards = std::move(tempBoard.getAllRandoMoves());

	if(maximizingPlayer)
	{

		int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, !playerTurn, false);
		for(int i=1; i<possBoards.size(); ++i)
		{
			int v = minMaxTreeRecurse(possBoards[i], depth-1, !playerTurn, false);
			bestValue = std::max(bestValue, v);
		}
		return bestValue;
	}
	else // NOT maximizingPlayer
	{
		int worstValue = minMaxTreeRecurse(possBoards[0], depth-1, !playerTurn, true);
		for(int i=1; i<possBoards.size(); ++i)
		{
			int v = minMaxTreeRecurse(possBoards[i], depth-1, !playerTurn, true);
			worstValue = std::min(worstValue, v);
		}
		return worstValue;
	}
}


std::string minMaxTreeBase(std::string theBoard, int depth, bool playerTurn)
{

	CheckerBoard tempBoard(theBoard, playerTurn);
	const std::vector<std::string>> possBoards = std::move(tempBoard.getAllRandoMoves());

	int indexBestBoard = 0;
	int bestValue = minMaxTreeRecurse(possBoards[0], depth-1, playerTurn, true)

	for(int i=1; i<possBoards.size(); ++i)
	{
		int v = minMaxTreeRecurse(possBoards[i], depth-1, playerTurn, true);
		if(v>bestValue)
		{
			bestValue = v;
			indexBestBoard = i;
		}
	}

	return possBoards[indexBestBoard];
}
*/
