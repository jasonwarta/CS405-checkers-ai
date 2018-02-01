
// update later



#include "checkers.h"
#include <vector>
#include <iostream>
#include <string>
#include <random>
using std::mt19937;





void printTempBoard(std::string &v)
{
	std::string spa = ".";

	std::cout << spa << v.at(0) << spa << v.at(1) << spa << v.at(2)<< spa << v.at(3)	<< std::endl <<
				 v.at(4)<< spa  << v.at(5)<< spa << v.at(6)<< spa << v.at(7) <<spa      << std::endl <<
				 spa <<v.at(8) << spa << v.at(9) << spa <<v.at(10)<< spa <<v.at(11)		<< std::endl <<
                 v.at(12)<< spa  <<v.at(13)<< spa <<v.at(14)<< spa << v.at(15)<< spa	<< std::endl <<
                 spa  <<v.at(16)<< spa << v.at(17)<< spa <<v.at(18)<< spa <<v.at(19)	<< std::endl <<
				 v.at(20)<< spa  <<v.at(21)<< spa <<v.at(22)<< spa << v.at(23) <<spa	<< std::endl <<
				 spa <<v.at(24)<< spa << v.at(25)<< spa <<v.at(26)<< spa <<v.at(27)		<< std::endl <<
				 v.at(28)<< spa << v.at(29)<< spa <<v.at(30)<< spa << v.at(31) <<spa	<< std::endl;
}

// Clean up later
void makeStartBoard(std::string &v1)
{
	//basic start board:
	/*
	std::string v2 { 'r','r','r','r',
					 'r','r','r','r',
					 'r','r','r','r',
					 '_','_','_','_',
					 '_','_','_','_',
					 'b','b','b','b',
			  	     'b','b','b','b',
					 'b','b','b','b' };
	*/

	//testing board:
	std::string v2 { '_','_','_','_',
					 '_','R','_','_',
					 '_','b','b','_',
					 '_','_','_','_',
					 '_','b','b','_',
					 '_','_','_','_',
					 '_','_','b','_',
					 '_','_','_','_' };
	/*
				X_X_X_X_	
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
	*/

	v1=v2;
}

std::string tempTester()
{
	std::string boardVec;

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


	std::vector<std::string> possibleMoves;

	makeStartBoard(boardVec);
	printTempBoard(boardVec);

	bool redPlayerTurn = true;
	CheckerBoard theCheckerBoard(boardVec, redPlayerTurn, redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard); // bestNameEver....
	possibleMoves = theCheckerBoard.getPossibleMoves();

    std::random_device rd;
    mt19937 rando(rd());
    if(possibleMoves.size() == 0) {
        return "";
    }
    else {
        std::uniform_int_distribution<> movePicked(0, (possibleMoves.size()-1));
        int temp = movePicked(rando);
        std::cout << possibleMoves[temp] << std::endl;
        return possibleMoves[temp];
    }
	std::cout << "NUMBER OF POSSIBLE MOVES: " << possibleMoves.size() << std::endl;
	for(int i=0; i<possibleMoves.size(); ++i)
	{
		std::cout << "-----------------" << " possibleMoves #" << i << " -----------------" << std::endl;
		printTempBoard(possibleMoves[i]);
	}



}

/*
 
	..00..01..02..03
	04..05..06..07..
	..08..09..10..11
	12..13..14..15..
	..16..17..18..19
	20..21..22..23..
	..24..25..26..27
	28..29..30..31..

temp key:
	r = red
	R = redKing
	b = black
	B = blackKing
	spa = nothingThere
*/

