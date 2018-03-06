// Game.cpp

#include "Game.h"

void Game::run() {
	size_t turnCounter = 0;
	bool redTeamTurn = true;
	std::string theBoard = START_BOARD_STRING;

	// main loop, only exit when done
	while (true && turnCounter < 200) {
		std::cout << std::setfill('0') << std::setw(3) << turnCounter << "," << theBoard << ",";

		*clock_ = std::chrono::system_clock::now();

		if( redTeamTurn ) {
			if (totalPieceCount(theBoard) <= 3 )
				theBoard = red_->getMove(theBoard, 10);
			else
				theBoard = red_->getMove(theBoard,8);
		}

		else {
			theBoard = black_->getMove(theBoard);
		}

		auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - *clock_).count();
		std::cout << "," << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << duration << "s" << std::endl;
		// std::cout << (redTeamTurn ? "Red" : "Black") << " took " << duration << " seconds" << std::endl;

		if ( theBoard.size() < 32) {
			std::cout << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !rPieceCount(theBoard) ) {
			std::cout << "Red team lost the game" << std::endl;
			break;
		}
		else if ( !bPieceCount(theBoard) ) {
			std::cout << "Black team lost the game" << std::endl;
			break;
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;
	}
}