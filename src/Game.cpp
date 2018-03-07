// Game.cpp

#include "Game.h"

void Game::run() {
	size_t turnCounter = 0;
	bool redTeamTurn = true;
	std::string theBoard = START_BOARD_STRING;

	// main loop, only exit when done
	while (true && turnCounter < 200) {
		(*os_) << "{'move':'" << std::setfill('0') << std::setw(3) << turnCounter << "','board':'" << theBoard << "',";

		*clock_ = std::chrono::system_clock::now();

		if( redTeamTurn ) {
			if (totalPieceCount(theBoard) <= 3 )
				theBoard = red_->getMove(theBoard, os_, 10);
			else
				theBoard = red_->getMove(theBoard, os_, 8);
		}

		else {
			theBoard = black_->getMove(theBoard, os_);
		}

		auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - *clock_).count();
		(*os_) << ",'time':'" << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << duration << "s'}" << std::endl;

		if ( theBoard.size() < 32) {
			(*os_) << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !rPieceCount(theBoard) ) {
			(*os_) << "Red team lost the game" << std::endl;
			break;
		}
		else if ( !bPieceCount(theBoard) ) {
			(*os_) << "Black team lost the game" << std::endl;
			break;
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;
	}
}