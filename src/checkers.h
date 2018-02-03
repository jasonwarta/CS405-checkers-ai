
// update later




#ifndef CHECKERS_H_INCLUDED
#define CHECKERS_H_INCLUDED

#include <vector>
#include <iostream>
#include <memory>
#include <string>

#include <random>
#include "consts.h"
/*
	TODO:
		Split into CPP and H
		change shared to unique pointers
		Update comment at top
*/

class TheChecker
{
public:
	TheChecker(bool onRedTeam, bool pieceKing);

	bool isTeamRed();

	bool isKing();

	void setKing(bool b);

private:
	bool isOnRed_;
	bool isKing_;
};


class CheckerBoard
{
public:
	// startBoard is the 32 vector
	// CheckerBoard(std::string startBoard, bool redPlayerTurn, std::vector<std::vector<int>> &redMove, std::vector<std::vector<int>> &redJump, std::vector<std::vector<int>> &blackMove, std::vector<std::vector<int>> &blackJump)
	CheckerBoard(std::string startBoard, bool redPlayerTurn); //std::vector<std::vector<int>> &redMove, std::vector<std::vector<int>> &redJump, std::vector<std::vector<int>> &blackMove, std::vector<std::vector<int>> &blackJump)
	
	std::string getRandoMove();

	std::vector<std::string> getAllRandoMoves();

	bool isValidBoard(std::string newBoard);

private:
	std::string turnBoardToString();

	void JumpingRecursion(int i, int j, bool currTeamDirection); // currTeamDirection gets passed going right way
	
	void moveJumpManager(int &i, int &j, bool goingRightWay);
	
	void updatePossibleMoves();



private:
	// pointers to all the checkers
	std::vector<std::shared_ptr<TheChecker>> checkers_;

	// will have all possible moves, each vector is a different board
	std::vector<std::string> possibleMoves_;
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
