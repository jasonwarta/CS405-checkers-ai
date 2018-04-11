// MinimaxWithAlphaBeta.cpp

#include "MinimaxWithAlphaBeta.h"

#ifdef CUDA
	#include <cuda_minmaxFunctions.h>
#endif //CUDA

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer, std::shared_ptr<Clock> clock, NeuralNet *net, bool usingPieceCount) : MinimaxWithAlphaBeta(redPlayer, clock, net, usingPieceCount)
{
	init(theBoard, depth, redPlayer);
}

std::string MinimaxWithAlphaBeta::getBestBoard(std::ostream *os) {
	printABStats(os);
	return bestBoard_;
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


void MinimaxWithAlphaBeta::init(std::string &theBoard, int depth, bool redPlayer) {
	CheckerBoard tempBoard(theBoard, redPlayer);
	std::vector< std::string > possBoards = std::move(tempBoard.getAllRandoMoves());

	if(possBoards.size() == 0) {
		bestBoard_ = "";
		return;
	}

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

float MinimaxWithAlphaBeta::minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, float alpha, float beta, bool maximizingPlayer) {

	if(depth == 0) {
		if(!usingPieceCount_) {
			net_->evaluateNN(theBoard, redPlayerTurn_);
			return net_->getLastNode();
		} else {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
	}

	if ( std::chrono::duration<double>(std::chrono::system_clock::now() - *clock_).count() >= 14.0) {
		if(!usingPieceCount_) {
			net_->evaluateNN(theBoard, redPlayerTurn_);
			return net_->getLastNode();
		} else {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
	}

	if (maximizingPlayer) {
		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

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
		CheckerBoard tempBoard(theBoard, !redPlayerTurn_);
		std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

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
