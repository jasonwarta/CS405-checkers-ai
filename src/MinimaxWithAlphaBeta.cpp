// MinimaxWithAlphaBeta.cpp

#include "MinimaxWithAlphaBeta.h"

#ifdef CUDA
	#include <cuda_minmaxFunctions.h>
#endif //CUDA


MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net, Clock *theClock) : MinimaxWithAlphaBeta(redPlayer, net, false)
{
	init(theBoard, depth, redPlayer, theClock);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, NeuralNet *net) : MinimaxWithAlphaBeta(redPlayer, net, false)
{
	init(theBoard, depth, redPlayer);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer) : MinimaxWithAlphaBeta(redPlayer, nullptr, true)
{
	init(theBoard, depth, redPlayer);
}

std::string MinimaxWithAlphaBeta::getBestBoard(std::ostream *os) {
	printABStats(os);
	return bestBoard_;
}

std::vector<std::string> MinimaxWithAlphaBeta::getBestVector(){
	return bestVector_;
}

void MinimaxWithAlphaBeta::printABStats(std::ostream *os) {
	(*os) << "\"expansions\":\"";
	(*os) << std::setfill('0') << std::setw(10) << boardExpansions_;
	(*os) << "\",\"alpha\":\"";
	(*os) << std::setfill('0') << std::setw(6) << breakAlpha_;
	(*os) << "\",\"beta\":\"";
	(*os) << std::setfill('0') << std::setw(6) << breakBeta_;
	(*os) << "\",";
}

void MinimaxWithAlphaBeta::init(std::string &theBoard, int depth, bool redPlayer, Clock *TheClock) {

	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	if(TheClock == nullptr)
	{
		timer_ = std::chrono::system_clock::now();
		usingIterativeDeepening_ = false;
	}
	else
	{
		timer_ = *TheClock;
		usingIterativeDeepening_ = true;
	}

	if(possBoards.size() == 0) {
		bestBoard_ = "";
		return;
	}

	float alpha = -10000.0;
	float beta = 10000.0;

	float bestVal = minimaxWithAlphaBetaRecursive(possBoards[0], depth-1, alpha, beta, false);
	bestBoard_ = possBoards[0];

	for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
		float val = minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, false);
		if ( val > bestVal ) {
			bestVal = val;
			bestBoard_ = *it;
		}
	}
}

float MinimaxWithAlphaBeta::minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, float alpha, float beta, bool maximizingPlayer) {

	if(depth == 0) 
	{
		bestVector_.push_back(theBoard);
		if(!usingPieceCount_) {
			return net_->evaluateNN(theBoard, redPlayerTurn_);
		} else {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0)
	{
		if(!usingPieceCount_) {
			return net_->evaluateNN(theBoard, redPlayerTurn_);
		} else {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
	}

	if (maximizingPlayer) {
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayerTurn_).getAllRandoMoves());

		boardExpansions_ += possBoards.size();

		float bestVal = -10000;

		if(possBoards.size() == 0)
			return bestVal;

		for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
			bestVal = std::max(bestVal, minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, false));
			alpha = std::max(alpha,bestVal);
			if(beta <= alpha) {
				breakBeta_++;
				break;
			}
		}
		return bestVal;
	}
	else {
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, !redPlayerTurn_).getAllRandoMoves());

		boardExpansions_ += possBoards.size();

		float worstVal = 10000;

		if(possBoards.size() == 0)
			return worstVal;

		for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
			worstVal = std::min(worstVal, minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, true));
			beta = std::min(beta,worstVal);
			if(beta <= alpha) {
				breakAlpha_++;
				break;
			}
		}
		return worstVal;
	}
}
