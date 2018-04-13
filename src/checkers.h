
// checkers.h
// Created By:
//	Cameron Showalter
// 	Rachel Crosley
//	Jason Warta
//	Jim Samson
// Version 4.20
// Date February 2, 2018



#ifndef CHECKERS_H_INCLUDED
#define CHECKERS_H_INCLUDED

#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <random>


#include "consts.h"
#include "defs.h"


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
	CheckerBoard(std::string &startBoard, bool redPlayerTurn); //std::vector<std::vector<int>> &redMove, std::vector<std::vector<int>> &redJump, std::vector<std::vector<int>> &blackMove, std::vector<std::vector<int>> &blackJump)

	std::string getRandoMove();

	std::vector<std::string> getAllRandoMoves();

	bool isValidBoard(std::string &newBoard);

private:
	std::string turnBoardToString();

	void JumpingRecursion(uint i, uint j, bool currTeamDirection); // currTeamDirection gets passed going right way

	void moveJumpManager(uint &i, uint &j, bool goingRightWay);

	void updatePossibleMoves();


private:
	// pointers to all the checkers
	std::vector<std::unique_ptr<TheChecker>> checkers_;

	// will have all possible moves, each vector is a different board
	std::vector<std::string> possibleMoves_;
	// which turn
	bool redTeamTurn_;
	// should look for moves or not
	bool firstJumpFound_;

	std::shared_ptr<MoveTable> currTeamMoveBoard_;
	std::shared_ptr<MoveTable> oppTeamMoveBoard_;
	std::shared_ptr<MoveTable> currTeamJumpBoard_;
	std::shared_ptr<MoveTable> oppTeamJumpBoard_;
};

std::string getRandomStartBoard();


class weights {
public:



private:



};



#endif // CHECKERS_H_INLCUDED
