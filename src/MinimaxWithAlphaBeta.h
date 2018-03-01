// MinimaxWithAlphaBeta.h

#ifndef MINIMAX_WITH_ALPHA_BETA_H_INCLUDED
#define MINIMAX_WITH_ALPHA_BETA_H_INCLUDED

#include <string>
#include <algorithm>
#include <chrono>
#include "checkers.h"
#include "BasicBoardEval.h"

class MinimaxWithAlphaBeta {
public:
	MinimaxWithAlphaBeta(std::string &theBoard, int depth, bool redPlayer) {
		redPlayerTurn_ = redPlayer;
		breakAlpha_ = 0;
		breakBeta_ = 0;

		CheckerBoard tempBoard(theBoard, redPlayerTurn_);
		std::vector< std::string > possBoards = std::move(tempBoard.getAllRandoMoves());

		if(possBoards.size() == 0){
			bestBoard_ = theBoard;
			return;
		}

		int alpha = -10000;
		int beta = 10000;

		clock_ = std::chrono::system_clock::now();
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

	std::string getBestBoard() {
		if( breakAlpha_ > 0 || breakBeta_ > 0 )
			std::cout << "alpha: " << breakAlpha_ << ", beta: " << breakBeta_ << std::endl;
		return bestBoard_;
	}

private:
	int minimaxWithAlphaBetaRecursive(std::string &theBoard, int depth, int alpha, int beta, bool maximizingPlayer) {
		if(depth == 0) {
			return basicBoardEval(theBoard, redPlayerTurn_);
		}

		if ( std::chrono::duration<double>(std::chrono::system_clock::now() - clock_).count() >= 14.0) {
			std::cout << "out of time" << std::endl;
		}

		if (maximizingPlayer) {
			CheckerBoard tempBoard(theBoard, redPlayerTurn_);
			std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

			int bestVal = -10000;

			if(possBoards.size() == 0)
				return bestVal;

			for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
				bestVal = std::max(bestVal, minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, false));
				alpha = std::max(alpha,bestVal);
				if(beta <= alpha) {
					// std::cout << "beta" << std::endl;
					breakBeta_++;
					break;
				}
			}
			return bestVal;
		}
		else {
			CheckerBoard tempBoard(theBoard, !redPlayerTurn_);
			std::vector<std::string> possBoards = std::move(tempBoard.getAllRandoMoves());

			int worstVal = 10000;

			if(possBoards.size() == 0)
				return worstVal;

			for(auto it = possBoards.begin(); it != possBoards.end(); ++it) {
				worstVal = std::min(worstVal, minimaxWithAlphaBetaRecursive(*it, depth-1, alpha, beta, true));
				beta = std::min(beta,worstVal);
				if(beta <= alpha) {
					breakAlpha_++;
					// std::cout << "alpha" << std::endl;
					break;
				}
			}
			return worstVal;
		}
	}


// 01 function alphabeta(node, depth, α, β, maximizingPlayer)
// 02      if depth = 0 or node is a terminal node
// 03          return the heuristic value of node
// 04      if maximizingPlayer
// 05          v := -∞
// 06          for each child of node
// 07              v := max(v, alphabeta(child, depth – 1, α, β, FALSE))
// 08              α := max(α, v)
// 09              if β ≤ α
// 10                  break (* β cut-off *)
// 11          return v
// 12      else
// 13          v := +∞
// 14          for each child of node
// 15              v := min(v, alphabeta(child, depth – 1, α, β, TRUE))
// 16              β := min(β, v)
// 17              if β ≤ α
// 18                  break (* α cut-off *)
// 19          return v
// (* Initial call *)
// alphabeta(origin, depth, -∞, +∞, TRUE)
private:
	std::string bestBoard_;
	bool redPlayerTurn_;
	int breakBeta_;
	int breakAlpha_;

	std::chrono::time_point<std::chrono::system_clock> clock_;
};

#endif
