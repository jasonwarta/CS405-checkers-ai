// MinimaxWithAlphaBeta.cpp

#include "MinimaxWithAlphaBeta.h"

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer) : MinimaxWithAlphaBeta(redPlayer) 
{
	kingWeight_ = NAN;
	init(theBoard, depth);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, float kingWeight) : MinimaxWithAlphaBeta(redPlayer) 
{
	kingWeight_ = kingWeight;
	init(theBoard, depth);
}

std::string MinimaxWithAlphaBeta::getBestBoard() {
	if( breakAlpha_ > 0 || breakBeta_ > 0 )
		std::cout << "alpha: " << breakAlpha_ << ", beta: " << breakBeta_ << std::endl;
	return bestBoard_;
}

void MinimaxWithAlphaBeta::init(std::string &theBoard, int depth) {
	CheckerBoard tempBoard(theBoard, redPlayerTurn_);
	std::vector< std::string > possBoards = std::move(tempBoard.getAllRandoMoves());

	if(possBoards.size() == 0) {
		bestBoard_ = theBoard;
		return;
	}

	clock_ = std::chrono::system_clock::now();

	if(kingWeight_ != kingWeight_) {
		// kingWeight_ is NAN. NAN == NAN always evaluates as false
		// using ints
				
		int alpha = -10000;
		int beta = 10000;

		int bestVal = minimaxWithAlphaBetaRecursive(possBoards[0], depth-1, alpha, beta, true);
		bestBoard_ = possBoards[0];

		for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
			int val = minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, true);
			if ( val > bestVal ) {
				bestVal = val;
				bestBoard_ = *it;
			}
		}

	}
	else { // using floats
		float alpha = -10000.0;
		float beta = 10000.0;

		float bestVal = minimaxWithAlphaBetaRecursive(possBoards[0], depth-1, alpha, beta, true);
		bestBoard_ = possBoards[0];

		for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
			float val = minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, true);
			if ( val > bestVal ) {
				bestVal = val;
				bestBoard_ = *it;
			}
		}
	}
}

template<typename NUM_TYPE>
NUM_TYPE MinimaxWithAlphaBeta::minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, NUM_TYPE alpha, NUM_TYPE beta, bool maximizingPlayer) {
	
	if(depth == 0) {
		if(typeid(NUM_TYPE) == typeid(int))
			return basicBoardEval(theBoard, redPlayerTurn_);
		else
			return weightedBoardEval(theBoard, kingWeight_, redPlayerTurn_);
	}

	if ( std::chrono::duration<double>(std::chrono::system_clock::now() - clock_).count() >= 14.0) {
		std::cout << "out of time" << std::endl;
	}

	if (maximizingPlayer) {
		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

		NUM_TYPE bestVal = -10000;

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
		CheckerBoard tempBoard(theBoard, !redPlayerTurn_);
		std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

		NUM_TYPE worstVal = 10000;

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