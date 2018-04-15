// Game.cpp

#include "Game.h"

char Game::run(std::string theBoard) {
	size_t turnCounter = 0;
	bool redTeamTurn = true;
	bool stalemate1 = false, stalemate2 = false;

	(*os_) << "[" << std::endl;
	// main loop, only exit when done
	while (turnCounter < TOTAL_MOVES)
	{
		(*os_) << "{\"move\":\"" << std::setfill('0') << std::setw(3) << turnCounter << "\",\"board\":\"" << theBoard << "\",";

		Clock clock = std::chrono::system_clock::now();

		if( redTeamTurn ) {
			theBoard = red_->getMove(theBoard, os_, 8);
		}

		else {
			theBoard = black_->getMove(theBoard, os_, 8);
		}

		auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - clock).count();
		(*os_) << "\"time\":\"" << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << duration << "s\"}," << std::endl;

		PieceMap pm(theBoard);

		if ( theBoard.size() < 32) {
			(*os_) << "]" << std::endl;
			return redTeamTurn ? 'B' : 'R';
			(*os_) << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !pm.redPieces() ) {
			(*os_) << "]" << std::endl;
			(*os_) << "Red team lost the game" << std::endl;
			return 'B';
		}
		else if ( !pm.blackPieces() ) {
			(*os_) << "]" << std::endl;
			(*os_) << "Black team lost the game" << std::endl;
			return 'R';
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;

		if (!moveTracker_.empty() && theBoard.compare(moveTracker_.front()) == 0)
		{
			if (stalemate1) {
				(*os_) << "]" << std::endl;
				return 'D';
			}
			else {
				stalemate1 = true;
				std::queue<std::string> empty;
				std::swap(moveTracker_, empty);	
			}
		}

		moveTracker_.push(theBoard);
		if (moveTracker_.size() > 4)
			moveTracker_.pop();

	}
	(*os_) << "]" << std::endl;
	(*os_) << "Draw" << std::endl;
	return 'D';
}