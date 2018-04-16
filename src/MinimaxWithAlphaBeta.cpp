// MinimaxWithAlphaBeta.cpp

#include "MinimaxWithAlphaBeta.h"

#ifdef CUDA
	#include <cuda_minmaxFunctions.h>
#endif //CUDA


MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, NeuralNet *net) : MinimaxWithAlphaBeta(redPlayer, net, false)
{
	init(theBoard, depth, redPlayer);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer) : MinimaxWithAlphaBeta(redPlayer, nullptr, true)
{
	init(theBoard, depth, redPlayer);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, bool usingIDS) : MinimaxWithAlphaBeta(redPlayer, nullptr, false, usingIDS)
{
	init(theBoard, depth, redPlayer);
}

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, uint depth, bool redPlayer, NeuralNet *net, bool usingIDS) : MinimaxWithAlphaBeta(redPlayer, net, false, usingIDS)
{
	init(theBoard, depth, redPlayer);
}

std::string MinimaxWithAlphaBeta::getBestBoard(std::ostream *os) {
	if(os != nullptr)
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

void MinimaxWithAlphaBeta::init(std::string &theBoard, uint depth, bool redPlayer) {

	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	timer_ = std::chrono::system_clock::now();
	if(possBoards.size() == 0) {
		bestBoard_ = "";
		return;
	}
	if (usingIterativeDeepening_)
	{
		std::vector<std::pair<float,std::string>> boardScores(possBoards.size());
		for(uint i=0; i<boardScores.size(); ++i)
		{
			boardScores[i] = std::make_pair(-10000.0f, possBoards[i]);
		}
		for (auto &item : boardScores)
		{
			std::cout << item.first << " " << item.second << std::endl;
		}
		while(true)
		{
			float alpha = -10000.0;
			float beta = 10000.0;
			for(uint i=0; i<boardScores.size(); ++i)
			{
				boardScores[i].first = minimaxWithAlphaBetaRecursive(boardScores[i].second, depth - 1, alpha, beta, false);
				// std::cout << "valid state: " << bool(boardScores[i].second.compare(possBoards[i])==0) << std::endl;
			}

			std::sort(boardScores.begin(), boardScores.end(), [](auto &a, auto &b) {
				// std::cout << a.first << " " << a.second << std::endl;
				// std::cout << b.first << " " << b.second << std::endl;
				// std::cout << std::endl;
				return a.first > b.first;
			});

			if(!TESTING)
				if( std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0)
				{
					std::cout << "hit \"unreachable\" state" << std::endl;
					return;
				}

			bestBoard_ = boardScores[0].second;
			
			if (TESTING && depth == 12)
			{
				for(auto &item : boardScores)
				{
					std::cout << item.first << " " << item.second << std::endl;
				}
				std::cout << "depth" << depth << std::endl;
				return;
			}

			depth += 2;
		}
	}
	else
	{
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
}

float MinimaxWithAlphaBeta::minimaxWithAlphaBetaRecursive(std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer) {

	if(depth == 0) 
	{
		
		// bestVector_.push_back(theBoard);
		if(!usingPieceCount_) {
			return net_->evaluateNN(theBoard, redPlayerTurn_);
		} else {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0 && !TESTING)
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
