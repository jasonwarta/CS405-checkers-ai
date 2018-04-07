#include "testRun.h"

char gameLoop(Player *red, Player *black, int blackTeamDepth, std::string theBoard) {
    std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());

    size_t turnCounter = 0;
	bool redTeamTurn = true;
	bool stalemate1 = false, stalemate2 = false;
    std::queue<std::string> moveTracker;

    while(true && turnCounter < 200) {
		std::cout << "{\"move\":\"" << std::setfill('0') << std::setw(3) << turnCounter << "\",\"board\":\"" << theBoard << "\",";

        *clock = std::chrono::system_clock::now();

        if( redTeamTurn ) {
			if (stalemate1) 
				theBoard = red->getMove(theBoard, &std::cout, 8);
			else 
				theBoard = red->getMove(theBoard, &std::cout, 6);
		}

		else {
            theBoard = black->getMove(theBoard, &std::cout, blackTeamDepth);
		}

        auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - *clock).count();
		std::cout << "\"time\":\"" << std::setw(9) << std::setfill('0') << std::fixed << std::setprecision(6) << duration << "s\"}" << std::endl;

		if ( theBoard.size() < 32) {
			return redTeamTurn ? 'B' : 'R';
			std::cout << (redTeamTurn ? "Red team" : "Black team") << " lost the game" << std::endl;
			break;
		} 
		else if ( !rPieceCount(theBoard) ) {
			std::cout << "Red team lost the game" << std::endl;
			return 'B';
		}
		else if ( !bPieceCount(theBoard) ) {
			std::cout << "Black team lost the game" << std::endl;
			return 'R';
		}

		redTeamTurn = !redTeamTurn;
		turnCounter++;

		if(theBoard.compare(moveTracker.front()) == 0) {
			if (stalemate1) {
				return 'D';
			}
			else {
				stalemate1 = true;
				std::queue<std::string> empty;
				std::swap(moveTracker, empty);	
			}
		}

		moveTracker.push(theBoard);
		if (moveTracker.size() > 4)
			moveTracker.pop();
    }
    std::cout << "Draw" << std::endl;
    return 'D';

}

void testRun(Player *red, Player *black) {
    std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());

	std::string startBoard = getRandomStartBoard();

	size_t turnCounter = 0;
	bool redTeamTurn = true;
	bool stalemate1 = false, stalemate2 = false;

    int blackTeamDepth = 2;
    gameLoop(red, black, blackTeamDepth, startBoard);

    blackTeamDepth = 4;
    gameLoop(red, black, blackTeamDepth, startBoard);

    blackTeamDepth = 6;
    gameLoop(red, black, blackTeamDepth, startBoard);

    blackTeamDepth = 8;
    gameLoop(red, black, blackTeamDepth, startBoard);
}