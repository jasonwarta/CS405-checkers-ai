// Game.cpp

#include "Game.h"

char Game::run(std::string theBoard) {
	size_t turnCounter = 0;
	bool redTeamTurn = true;
	bool stalemate1 = false, stalemate2 = false;

	// main loop, only exit when done
	while (true && turnCounter < 200) {
		(*os_) << "{\"move\":\"" << std::setfill('0') << std::setw(3) << turnCounter << "\",\"board\":\"" << theBoard << "\",";

		*clock_ = std::chrono::system_clock::now();

		if( redTeamTurn ) {
			// if (stalemate1 && stalemate2)
			// 	theBoard = red_->getMove(theBoard, os_, 10);
			if (stalemate1)
				theBoard = red_->getMove(theBoard, os_, 8);
			else 
				theBoard = red_->getMove(theBoard, os_, 6);
		}

		else {
			// if (stalemate1 && stalemate2)
			// 	theBoard = black_->getMove(theBoard, os_, 10);
			if (stalemate1)
				theBoard = black_->getMove(theBoard, os_, 8);
			else
				theBoard = black_->getMove(theBoard, os_, 6);
		}

		auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - *clock_).count();
		(*os_) << "\"time\":\"" << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << duration << "s\"}" << std::endl;

		if ( theBoard.size() < 32) {
			return redTeamTurn ? 'B' : 'R';
			(*os_) << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !rPieceCount(theBoard) ) {
			(*os_) << "Red team lost the game" << std::endl;
			return 'B';
		}
		else if ( !bPieceCount(theBoard) ) {
			(*os_) << "Black team lost the game" << std::endl;
			return 'R';
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;

		if(theBoard.compare(moveTracker_.front()) == 0) {
			// if (stalemate1 && stalemate2) {
			// 	return 'D';
			// }
			// else if (stalemate1 && !stalemate2) {
			// 	stalemate2 = true;
			// 	moveTracker_ = {};
			// }
			// else if (!stalemate1 && !stalemate2) {
			// 	stalemate1 = true;
			// 	moveTracker_ = {};
			// }
			if (stalemate1) {
				return 'D';
			}
			else {
				stalemate1 = true;
				std::queue<std::string> empty;
				std::swap(moveTracker_, empty);	
				moveTracker_ = {};
			}
		}

		moveTracker_.push(theBoard);
		if (moveTracker_.size() > 4)
			moveTracker_.pop();

	}
	(*os_) << "Draw" << std::endl;
	return 'D';
}