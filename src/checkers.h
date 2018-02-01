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
	CheckerBoard(std::string startBoard, bool redPlayerTurn) {
		redTeamTurn_ = redPlayerTurn;
		firstJumpFound_ = false;

		if(redTeamTurn_) {
			currTeamMoveBoard_ = redMoveBoard;
			currTeamJumpBoard_ = redJumpBoard;
			oppTeamMoveBoard_ = blackMoveBoard;
			oppTeamJumpBoard_ = blackJumpBoard;
		}
		else {
			currTeamMoveBoard_ = blackMoveBoard;
			currTeamJumpBoard_ = blackJumpBoard;
			oppTeamMoveBoard_ = redMoveBoard;
			oppTeamJumpBoard_ = redJumpBoard;
		}


		checkers_.resize(32);
		for(int i=0; i<startBoard.size(); ++i) {
			// later, break at space first for speedup?
			if(startBoard.at(i) == 'r') {
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, false));
			}
			else if(startBoard.at(i) == 'R') {
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, true));
			}
			else if(startBoard.at(i) == 'b') {
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, false));
			}
			else if(startBoard.at(i) == 'B') {
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, true));
			}
			else if(startBoard.at(i) == '_') { // nothing there
				checkers_[i] = nullptr; //NULL or *NULL? test later....
			}
			else {
				std::cout << "FINALLY FOUND THAT FUCKKKING BUGGGGG!!!!!!!" << std::endl; // nope....
			}
		}
		// Start figuring out what can go where...
		updatePossibleMoves();
	}

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

	//Ill put a better way to do this later, but this is fast and easy
    std::vector<std::vector<int>> redMoveBoard { {4, 5},     //0
												{5, 6},
											    {6, 7},
                                                {7, -1},
											    {-1, 8},
                                                {8, 9},     //5
											    {9, 10},
											    {10, 11},
											    {12, 13},
											    {13, 14},
											    {14, 15},   //10
                                                {15, -1},
                                                {-1, 16},
											    {16, 17},
											    {17, 18},
											    {18, 19},   //15
											    {20, 21},
											    {21, 22},
											    {22, 23},
                                                {23, -1},
                                                {-1, 24},   //20
											    {24, 25},
											    {25, 26},
											    {26, 27},
											    {28, 29},
											    {29, 30},   //25
											    {30, 31},
                                                {31, -1},
											    {-1, -1},
											    {-1, -1},
											    {-1, -1},	//30
											    {-1, -1}	};

	std::vector<std::vector<int>> redJumpBoard { {-1, 9},	//0
											   {8, 10},
											   {9, 11},
											   {10, -1},
											   {-1, 13},
											   {12, 14},	//5
											   {13, 15},
											   {14, -1},
											   {-1, 17},
											   {16, 18},
											   {17, 19},	//10
											   {18, -1},
											   {-1, 21},
											   {20, 22},
											   {21, 23},
											   {22, -1},	//15
											   {-1, 25},
											   {24, 26},
											   {25, 27},
											   {26, -1},
											   {-1, 29},	//20
											   {28, 30},
											   {29, 31},
											   {30, -1},
											   {-1, -1},
											   {-1, -1},	//25
											   {-1, -1},
											   {-1, -1},
											   {-1, -1},
											   {-1, -1},
											   {-1, -1},	//30
											   {-1, -1} };

	std::vector<std::vector<int>> blackMoveBoard { {-1, -1},	//0
											    {-1, -1},
											    {-1, -1},
											    {-1, -1},
                                                {-1, 0},
											    {0, 1},//5
											    {1, 2},
											    {2, 3},
											    {4, 5},
											    {5, 6},
											    {6, 7},//10
                                                {7, -1},
                                                {-1, 8},
											    {8, 9},
											    {9, 10},
											    {10, 11}, //15
											    {12, 13},
											    {13, 14},
											    {14, 15},
                                                {15, -1},
                                                {-1, 16},//20
											    {16, 17},
											    {17, 18},
											    {18, 19},
											    {20, 21},
											    {21, 22},//25
											    {22, 23},
                                                {23, -1},
                                                {-1, 24},
											    {24, 25},
											    {25, 26},//30
											    {26, 27}};

	std::vector<std::vector<int>> blackJumpBoard { {-1, -1},	//0
											   {1, -1},
											   {-1, -1},
											   {-1, -1},
											   {-1, -1},
											   {-1, -1},	//5
											   {-1, -1},
											   {-1, -1},
											   {-1, 1},
											   {0, 2},
											   {1, 3},	//10
											   {2, -1},
											   {-1, 5},
											   {4, 6},
											   {5, 7},
											   {6, -1},	//15
											   {-1, 9},
											   {8, 10},
											   {9, 11},
											   {10, -1},
											   {-1, 13},	//20
											   {12, 14},
											   {13, 15},
											   {14, -1},
											   {-1, 17},
											   {16, 18},	//25
											   {17, 19},
											   {18, -1},
											   {-1, 21},
											   {20, 22},
											   {21, 23},	//30
											   {22, -1} };

};
#endif // CHECKERS_H_INLCUDED
