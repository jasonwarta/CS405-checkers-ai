// MinimaxWithAlphaBeta.cpp

#include "MinimaxWithAlphaBeta.h"

#ifdef CUDA
	#include <cuda_minmaxFunctions.h>
#endif //CUDA

MinimaxWithAlphaBeta::MinimaxWithAlphaBeta(std::string &theBoard, bool redPlayer, uint minDepth, uint maxDepth, NeuralNet *net) :
	MinimaxWithAlphaBeta(redPlayer, net, net != nullptr ? false : true, minDepth == maxDepth ? false : true)
{
	init(theBoard, minDepth, maxDepth, redPlayer);
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

	if (maxDepthReached_ != 0) {
		(*os) << "\"depth_reached\":\"";
		(*os) << std::setfill('0') << std::setw(2) << maxDepthReached_;
		(*os) << "\",";
	}
}

void MinimaxWithAlphaBeta::init(std::string &theBoard, uint minDepth, uint maxDepth, bool redPlayer) 
{
	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	timer_ = std::chrono::system_clock::now();
	if(possBoards.size() == 0) {
		bestBoard_ = "";
		return;
	}

	if (possBoards.size() == 1)
	{
		bestBoard_ = possBoards[0];
		return;
	}

	if(minDepth%2 != 0 || maxDepth%2 != 0)
	{
		std::cout << "Don't touch things you don't know how to use!" << std::endl;
		minDepth = 8, maxDepth = 8, usingIterativeDeepening_ = false;
	}

	if (usingIterativeDeepening_)
	{
		std::vector<std::pair<float,std::string>> boardScores(possBoards.size());
		for(uint i=0; i<boardScores.size(); ++i)
		{
			boardScores[i] = std::make_pair(-10000.0f, possBoards[i]);
		}

		std::vector<std::thread> threads;
		std::vector<NeuralNet> nets;
		std::cout << possBoards.size() << std::endl;

		for(uint i = 0; i < possBoards.size(); ++i)
			nets.push_back(*net_);

		while(true)
		{
			float alpha = -10000.0f;
			float beta = 10000.0f;
			for (uint i = 0; i < possBoards.size(); ++i)
			{	
				threads.push_back(std::thread(
					threadManager,
					std::ref(boardScores[i].second),
					this,
					std::ref(nets[i]),
					std::ref(boardScores[i].first),
					minDepth - 1
				));
			}

			for(std::thread &t : threads)
				t.join();

			if (std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0)
			{
				maxDepthReached_ = minDepth;
				return;
			}

			std::sort(boardScores.begin(), boardScores.end(), [](auto &a, auto &b) {
				return a.first > b.first;
			});

			bestBoard_ = boardScores[0].second;
			
			if (minDepth == maxDepth)
			{
				return;
			}

			minDepth += 2;
			threads.clear();
		}
	}
	else
	{
		float alpha = -10000.0;
		float beta = 10000.0;
		float bestVal = minimaxWithAlphaBetaRecursive(this, possBoards[0], minDepth-1, alpha, beta, false);
		bestBoard_ = possBoards[0];

		for(auto it = possBoards.begin()+1; it != possBoards.end(); ++it) {
			// std::cout << alpha << " " << beta << std::endl;
			float val = minimaxWithAlphaBetaRecursive(this, *it, minDepth-1, bestVal, beta, false);
			if ( val > bestVal ) {
				bestVal = val;
				bestBoard_ = *it;
			}
		}
	}
}

void threadManager(std::string &board, MinimaxWithAlphaBeta *self, NeuralNet &net, float &result, uint depth)
{	
	std::cout << "launching thread" << std::endl;
	result = threadedMinimaxWithAlphaBetaRecursive(self, &net, board, depth, -10000, 10000, false);
}

float minimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer)
{
	if (depth == 0)
	{
		if (!self->usingPieceCount_)
		{
			return self->net_->evaluateNN(theBoard, self->redPlayerTurn_);
		}
		else
		{
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - self->timer_).count() >= 14.0)
	{
		if (!self->usingPieceCount_)
		{
			return self->net_->evaluateNN(theBoard, self->redPlayerTurn_);
		}
		else
		{
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (maximizingPlayer)
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, self->redPlayerTurn_).getAllRandoMoves());

		self->boardExpansions_ += possBoards.size();

		float bestVal = -10000;

		if (possBoards.size() == 0)
			return bestVal;

		for (auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			bestVal = std::max(bestVal, minimaxWithAlphaBetaRecursive(self, *it, depth - 1, alpha, beta, false));
			alpha = std::max(alpha, bestVal);
			if (beta <= alpha)
			{
				self->breakBeta_++;
				break;
			}
		}
		return bestVal;
	}
	else
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, !self->redPlayerTurn_).getAllRandoMoves());

		self->boardExpansions_ += possBoards.size();

		float worstVal = 10000;

		if (possBoards.size() == 0)
			return worstVal;

		for (auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			worstVal = std::min(worstVal, minimaxWithAlphaBetaRecursive(self, *it, depth - 1, alpha, beta, true));
			beta = std::min(beta, worstVal);
			if (beta <= alpha)
			{
				self->breakAlpha_++;
				break;
			}
		}
		return worstVal;
	}
}

float threadedMinimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, NeuralNet *net, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer)
{
	if (depth == 0)
	{
		if (!self->usingPieceCount_)
		{
			return net->evaluateNN(theBoard, self->redPlayerTurn_);
		}
		else
		{
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - self->timer_).count() >= 14.0)
	{
		if (!self->usingPieceCount_)
		{
			return net->evaluateNN(theBoard, self->redPlayerTurn_);
		}
		else
		{
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (maximizingPlayer)
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, self->redPlayerTurn_).getAllRandoMoves());

		// self->boardExpansions_ += possBoards.size();

		float bestVal = -10000;

		if (possBoards.size() == 0)
			return bestVal;

		for (auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			bestVal = std::max(bestVal, threadedMinimaxWithAlphaBetaRecursive(self, net, *it, depth - 1, alpha, beta, false));
			alpha = std::max(alpha, bestVal);
			if (beta <= alpha)
			{
				// self->breakBeta_++;
				break;
			}
		}
		return bestVal;
	}
	else
	{
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, !self->redPlayerTurn_).getAllRandoMoves());

		// self->boardExpansions_ += possBoards.size();

		float worstVal = 10000;

		if (possBoards.size() == 0)
			return worstVal;

		for (auto it = possBoards.begin(); it != possBoards.end(); ++it)
		{
			worstVal = std::min(worstVal, threadedMinimaxWithAlphaBetaRecursive(self, net, *it, depth - 1, alpha, beta, true));
			beta = std::min(beta, worstVal);
			if (beta <= alpha)
			{
				// self->breakAlpha_++;
				break;
			}
		}
		return worstVal;
	}
}