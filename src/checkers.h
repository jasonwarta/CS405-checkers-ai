#ifndef CHECKERS_H_INCLUDED
#define CHECKERS_H_INCLUDED

#include <vector>
#include <iostream>
#include <memory>
#include <string>
using std::string;

void makeStartBoard(std::vector<char> &v);
void printTempBoard(std::vector<char> &v);

string tempTester();

// May change from class to something else later...
class TheChecker {
public:
	TheChecker(bool onRedTeam, bool pieceKing) {
		isOnRed_ = onRedTeam;
		isKing_ = pieceKing;
	}
	bool isTeamRed() {
		return isOnRed_;
	}
	bool isKing() {
		return isKing_;
	}
	void setKing(bool b) {
		isKing_ = b;
	}
private:
	bool isOnRed_;
	bool isKing_;
};

class CheckerBoard {
public:
	// startBoard is the 32 vector
	CheckerBoard(std::string startBoard, bool redPlayerTurn);

	string turnBoardtoVec();
	void JumpingRecursion(int i, int j, bool currTeamDirection);
	// You can also not include the teamMoveBoard and teamJumpBoard, and just say if goingRightWay == true, use currTeamBoard, else use oppTeamBoard.
	void updateBoardWithDirection(int &i, int &j, bool goingRightWay);
	void updatePossibleMoves();

	std::vector<string> getPossibleMoves();
private:
	// pointers to all the checkers
	std::vector<std::shared_ptr<TheChecker>> checkers_;

	// will have all possible moves, each vector is a different board
	std::vector<string> possibleMoves_;
	// which turn
	bool redTeamTurn_;
	// should look for moves or not
	bool firstJumpFound_;

	// May think of a better way of putting these here later... too tired. It works.
	std::vector<std::vector<int>> currTeamMoveBoard_;
	std::vector<std::vector<int>> oppTeamMoveBoard_;
	std::vector<std::vector<int>> currTeamJumpBoard_;
	std::vector<std::vector<int>> oppTeamJumpBoard_;
};
#endif // CHECKERS_H_INLCUDED
