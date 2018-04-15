#include "MinimaxTree.h"

MinimaxTree::MinimaxTree(std::string &theBoard, int depth, bool redPlayer) :
	MinimaxTree(redPlayer, nullptr, true)
{
	init(theBoard, depth, redPlayer);
}

MinimaxTree::MinimaxTree(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net) :
	MinimaxTree(redPlayer, net, false)
{
	init(theBoard, depth, redPlayer);	
}

void MinimaxTree::init(std::string &theBoard, int depth, bool redPlayer)
{	
	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	timer_ = std::chrono::system_clock::now();

	if(possBoards.size() == 0)
	{
		bestBoard_ = "";
		return;
	}

	float bestVal = minimaxRecurse(possBoards[0], depth-1, false);
	bestBoard_ = possBoards[0];

	for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it)
	{
		float val = minimaxRecurse(*it, depth-1, false);
		if(val > bestVal)
		{
			bestVal = val;
			bestBoard_ = *it;
		}
	}
}

float MinimaxTree::minimaxRecurse(std::string &theBoard, int depth, bool maximizingPlayer)
{
	if (depth == 0) // maybe other checks here later?...
	{
		if(!usingPieceCount_)
			return net_->evaluateNN(theBoard, redPlayerTurn_);
		else
			return basicBoardEval(theBoard, redPlayerTurn_);
	}

	if(std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0)
	{
		if (!usingPieceCount_)
			return net_->evaluateNN(theBoard, redPlayerTurn_);
		else
			return basicBoardEval(theBoard, redPlayerTurn_);
	}

	if (maximizingPlayer)
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayerTurn_).getAllRandoMoves());

		float bestVal = -10000;

		if(possBoards.size() == 0)
			return bestVal;

		for(auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			bestVal = std::max(bestVal, minimaxRecurse(*it, depth-1, false));
		}
		return bestVal;
	}
	else // NOT maximizingPlayer
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, !redPlayerTurn_).getAllRandoMoves());

		float worstValue = 10000;

		if(possBoards.size() == 0)
			return worstValue;

		for (auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			worstValue = std::min(worstValue, minimaxRecurse(*it, depth - 1, true));
		}
		return worstValue;
	}
}