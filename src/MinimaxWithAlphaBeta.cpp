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
}

void MinimaxWithAlphaBeta::init(std::string &theBoard, uint minDepth, uint maxDepth, bool redPlayer) 
{
	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	timer_ = std::chrono::system_clock::now();
	if(possBoards.size() == 0) {
		bestBoard_ = "";
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
				boardScores[i].first = minimaxWithAlphaBetaRecursive(this, boardScores[i].second, minDepth - 1, alpha, beta, false);
				// std::cout << "valid state: " << bool(boardScores[i].second.compare(possBoards[i])==0) << std::endl;
			}

			if (std::chrono::duration<double>(std::chrono::system_clock::now() - timer_).count() >= 14.0)
			{
				std::cout << "hit \"unreachable\" state" << std::endl;
				return;
			}

			std::sort(boardScores.begin(), boardScores.end(), [](auto &a, auto &b) {
				return a.first > b.first;
			});

			bestBoard_ = boardScores[0].second;
			
			if (minDepth == maxDepth)
			{
				for(auto &item : boardScores)
				{
					std::cout << item.first << " " << item.second << std::endl;
				}
				std::cout << "depth" << minDepth << std::endl;
				return;
			}

			minDepth += 2;
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

void MinimaxWithAlphaBeta::threadedInit(std::string &theBoard, uint depth, bool redPlayer, uint numThreads)
{
	std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, redPlayer).getAllRandoMoves());
	timer_ = std::chrono::system_clock::now();
	if (possBoards.size() == 0)
	{
		bestBoard_ = "";
		return;
	}

	float alpha = -10000.0f;
	float beta = 10000.0f;

	std::vector<std::thread> threads;
	std::vector<std::pair<float,std::string>> results;
	std::vector<NeuralNet> nets;

	for(uint i = 0; i < possBoards.size(); ++i)
	{
		// results.push_back(std::make_pair(0.0f,possBoards[i]));
		nets.push_back(*net_);
	}

	std::cout << "going into threads" << std::endl;
	std::queue<std::string> boards;
	for (uint i = 0; i < possBoards.size(); ++i)
	{
		boards.push(possBoards[i]);
	}

	std::mutex mtx;
	for(uint i = 0; i < numThreads; ++i)
	{
		threads.push_back(std::thread(
			threadManager,
			std::ref(boards),
			std::ref(mtx),
			this,
			net_,
			std::ref(results),
			depth));
	}

	for(std::thread &t : threads)
		t.join();

	std::sort(results.begin(), results.end(), [](auto &a, auto &b) {
		// std::cout << a.first << " " << a.second << std::endl;
		// std::cout << b.first << " " << b.second << std::endl;
		// std::cout << std::endl;
		return a.first > b.first;
	});
	// std::sort(results.begin(), results.end(), [](auto &a, auto &b) {
	// 	a.first > b.first;
	// });

	for(auto &res : results)
		std::cout << res.first << " " << res.second << std::endl;

	bestBoard_ = results[0].second;
}

void threadManager(std::queue<std::string> & boards, std::mutex & mtx, MinimaxWithAlphaBeta *self, NeuralNet *net, std::vector<std::pair<float,std::string>> &results, uint depth)
{
	while(boards.size() > 0)
	{
		std::string myBoard;
		mtx.lock();
		if(boards.size() > 0)
		{
			myBoard = boards.front();
			boards.pop();
		}
		else
		{
			mtx.unlock();
			return;
		}
		mtx.unlock();

		float temp = threadedMinimaxWithAlphaBetaRecursive(self, net, myBoard, depth, -10000, 10000, false);
		
		mtx.lock();
		results.push_back(std::make_pair(temp,myBoard));
		mtx.unlock();
	}
}

float threadedMinimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, NeuralNet *net, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer)
{
	// std::cout << retVal << std::endl;
	// net->printData();
	if(depth == 0) 
	{
		// std::cout << "got to bottom" << std::endl;
		// self->bestVector_.self->push_back(theBoard);
		if(!self->usingPieceCount_) {
			return self->net_->evaluateNN(theBoard, self->redPlayerTurn_);
		} else {
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - self->timer_).count() >= 14.0 && !TESTING)
	{
		if(!self->usingPieceCount_) {
			return self->net_->evaluateNN(theBoard, self->redPlayerTurn_);
		} else {
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (maximizingPlayer) {
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, self->redPlayerTurn_).getAllRandoMoves());

		// self->boardExpansions_ += possBoards.size();

		float bestVal = -10000;

		if(possBoards.size() == 0)
			return bestVal;

		for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
			bestVal = std::max(bestVal, threadedMinimaxWithAlphaBetaRecursive(self, net, *it, depth-1, alpha, beta, false));
			alpha = std::max(alpha,bestVal);
			if(beta <= alpha) {
				// self->breakBeta_++;
				break;
			}
		}
		return bestVal;
	}
	else {
		std::vector<std::string> possBoards = std::move(CheckerBoard(theBoard, !self->redPlayerTurn_).getAllRandoMoves());

		// self->boardExpansions_ += possBoards.size();

		float worstVal = 10000;

		if(possBoards.size() == 0)
			return worstVal;

		for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
			worstVal = std::min(worstVal, threadedMinimaxWithAlphaBetaRecursive(self, net, *it, depth-1, alpha, beta, true));
			beta = std::min(beta,worstVal);
			if(beta <= alpha) {
				// self->breakAlpha_++;
				break;
			}
		}
		return worstVal;
	}
}

float minimaxWithAlphaBetaRecursive(MinimaxWithAlphaBeta *self, std::string &theBoard, uint depth, float alpha, float beta, bool maximizingPlayer)
{

	if (depth == 0)
	{

		// self->bestVector_.self->push_back(theBoard);
		if (!self->usingPieceCount_)
		{
			return self->net_->evaluateNN(theBoard, self->redPlayerTurn_);
		}
		else
		{
			return basicBoardEval(theBoard, self->redPlayerTurn_);
		}
	}

	if (std::chrono::duration<double>(std::chrono::system_clock::now() - self->timer_).count() >= 14.0 && !TESTING)
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