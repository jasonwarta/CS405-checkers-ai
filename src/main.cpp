#include <uWS/uWS.h>

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <string>
#include <mutex>
#include <iomanip>

// #include "server.h"
#include "threadUtils.h"
#include "consts.h"
// #include "checkers.h"
#include "MinMaxTree.h"
#include "MinimaxWithAlphaBeta.h"

// void launchServer(Communicator * comm) {
// 	uWS::Hub h;
// 	createServerInstance(h,(*comm));
// 	h.run();
// }

// Linux sleep command takes microseconds, most other things think in milliseconds
int milli_to_micro(int milliseconds) {
	return 1000*milliseconds;
}

int main(int argc, char const *argv[]) {
	// std::cout << argc << std::endl;

	std::mutex mtx;
	// Communicator comm = {START_BOARD_STRING,&mtx};

	// launch server in separate thread
	// server currently has no logic for terminating
	// std::thread serverThread(launchServer,&comm);

	size_t turnCounter = 0;
	bool redTeamTurn = true;
	std::string boardString = START_BOARD_STRING;
	// main loop, only exit when done
	while (true && turnCounter < 200) {
		std::cout << std::setw(3) << turnCounter << ": " << boardString << std::endl;

		// red team adjusts search depth depending on game state
		// values assignments are not backed up by math or statistics, and could
		// probably be refined to some extent, especially once we improve the BEF
		if( redTeamTurn ) {
			// if(turnCounter > 120 ) {
			// 	MinMaxTree board(boardString,8,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			// else if(turnCounter > 80 && totalPieceCount(boardString) < 8 ) {
			// 	MinMaxTree board(boardString,8,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			// else if( totalPieceCount(boardString) < 4 ) {
			// 	MinMaxTree board(boardString,8,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			// else if ( totalPieceCount(boardString) < 6 ) {
			// 	MinMaxTree board(boardString,7,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			// else if ( totalPieceCount(boardString) < 8 ) {
			// 	MinMaxTree board(boardString,6,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			// else if ( totalPieceCount(boardString) < 12 ) {
			// 	MinMaxTree board(boardString,5,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// } 
			// else {
			// 	MinMaxTree board(boardString,4,redTeamTurn);
			// 	boardString = board.getBestBoard();
			// }
			
			// MinMaxTree board(boardString,4,redTeamTurn);
			// boardString = board.getBestBoard();

			MinimaxWithAlphaBeta board(boardString,6,redTeamTurn);
			boardString = board.getBestBoard();
		}

		// black team uses random move picking... should always lose
		else {
			// MinMaxTree board(boardString,4,redTeamTurn);
			// boardString = board.getBestBoard();

			CheckerBoard board(boardString,redTeamTurn);
			boardString = board.getRandoMove();
		}

		if ( boardString.size() < 32) {
			std::cout << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !rPieceCount(boardString) ) {
			std::cout << "Red team lost the game" << std::endl;
			break;
		}
		else if ( !bPieceCount(boardString) ) {
			std::cout << "Black team lost the game" << std::endl;
			break;
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;

		// comm.setBoard(boardString);

		// std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
		// std::cout << "in main thread" << endl;
		// std::cout << comm.getBoard() << endl;
	}


	std::cout << "reached end of program" << std::endl;
	exit(0);
}