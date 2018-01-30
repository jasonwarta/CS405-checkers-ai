
// update later



#include "checkers.h"
#include <vector>
#include <iostream>
#include <string>

void printTempBoard(std::vector<char> &v)
{
	std::string spa = ".";
	std::cout << v[0] << spa << v[1] << spa << v[2]<< spa << v[3]<< spa 	<< std::endl <<
				 spa  << v[4]<< spa  << v[5]<< spa << v[6]<< spa << v[7] 	<< std::endl <<
				 v[8] << spa << v[9] << spa <<v[10]<< spa <<v[11]<< spa		<< std::endl <<
				 spa  <<v[12]<< spa  <<v[13]<< spa <<v[14]<< spa << v[15]	<< std::endl <<
				 v[16]<< spa << v[17]<< spa <<v[18]<< spa <<v[19]<< spa		<< std::endl <<
				 spa  <<v[20]<< spa  <<v[21]<< spa <<v[22]<< spa << v[23]	<< std::endl <<
				 v[24]<< spa << v[25]<< spa <<v[26]<< spa <<v[27]<< spa		<< std::endl <<
				 spa  <<v[28]<< spa  <<v[29]<< spa <<v[30]<< spa << v[31]	<< std::endl;
}

// Clean up later
void makeStartBoard(std::vector<char> &v1)
{
	//basic start board:
	/*
	std::vector<char> v2 { 'r','r','r','r',
						   'r','r','r','r',
						   'r','r','r','r',
						   ' ',' ',' ',' ',
						   ' ',' ',' ',' ',
						   'b','b','b','b',
						   'b','b','b','b',
						   'b','b','b','b' };
	*/

	//testing board:
	std::vector<char> v2 { ' ',' ',' ',' ',
						   ' ','R',' ',' ',
						   ' ','b','b',' ',
						   ' ',' ',' ',' ',
						   ' ','b','b',' ',
						   ' ',' ',' ',' ',
						   ' ',' ','b',' ',
						   ' ',' ',' ',' ' };
	/*
	Note to self: check for a bouncing king later
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
				X_X_X_X_
				_X_X_X_X
				X_X_X_X_
	*/

	v1=v2;
}

void tempTester()
{
	std::vector<char> boardVec;

	//Ill put a better way to do this later, but this is fast and easy
	std::vector<std::vector<int>> redMoveBoard { {-1, 4},	//0
								 				{4, 5},
												{5, 6},
											    {6, 7},
											    {8, 9},
											    {9, 10},	//5
											    {10, 11},
											    {11, -1},
											    {-1, 12},
											    {12, 13},
											    {13, 14},	//10
											    {14, 15},
											    {16, 17},
											    {17, 18},
											    {18, 19},
											    {19, -1},	//15
											    {-1, 20},
											    {20, 21},
											    {21, 22},
											    {22, 23},
											    {24, 25},	//20
											    {25, 26},
											    {26, 27},
											    {27, -1},
											    {-1, 28},
											    {28, 29},	//25
											    {29, 30},
											    {30, 31},
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
											   {0, 1},
											   {1, 2},	//5
											   {2, 3},
											   {3, -1},
											   {-1, 4},
											   {4, 5},
											   {5, 6},	//10
											   {6, 7},
											   {8, 9},
											   {9, 10},
											   {10, 11},
											   {11, -1},	//15
											   {-1, 12},
											   {12, 13},
											   {13, 14},
											   {14, 15},
											   {16, 17},	//20
											   {17, 18},
											   {18, 19},
											   {19, -1},
											   {-1, 20},
											   {20, 21},	//25
											   {21, 22},
											   {22, 23},
											   {24, 25},
											   {25, 26},
											   {26, 27},	//30
											   {27, -1} };

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


	std::vector<std::vector<char>> possibleMoves;

	makeStartBoard(boardVec);
	printTempBoard(boardVec);

	bool redPlayerTurn = true;
	CheckerBoard theCheckerBoard(boardVec, redPlayerTurn, redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard); // bestNameEver....
	theCheckerBoard.updatePossibleMoves(); // note to self: make it so you don't have to update. Put this line in the constructor maybe
	possibleMoves = theCheckerBoard.getPossibleMoves();


	std::cout << "NUMBER OF POSSIBLE MOVES: " << possibleMoves.size() << std::endl;
	for(int i=0; i<possibleMoves.size(); ++i)
	{
		std::cout << "-----------------" << " possibleMoves #" << i << " -----------------" << std::endl;
		printTempBoard(possibleMoves[i]);
	}



}

/*

	00..01..02..03..
	..04..05..06..07
	08..09..10..11..
	..12..13..14..15
	16..17..18..19..
	..20..21..22..23
	24..25..26..27..
	..28..29..30..31

temp key:
	r = red
	R = redKing
	b = black
	B = blackKing
	spa = nothingThere
*/















/*












std::vector<std::vector<char>> generatePossibleMoves(std::vector<char> currentBoard, std::string teamName)
{
	std::vector<std::vector<char>> returnMe;
	int teamCheckerCount = 0;
	// it's going to add checkers every recursive call. move outside recursion later
	for(int i=0; i<currentBoard.size(); ++i)
	{
		if((teamName == "Red" && (currentBoard[i] == 'r' || currentBoard[i] == 'R')) ||
		   (teamName == "Black" && (currentBoard[i] == 'b' || currentBoard[i] == 'B'))  )
		{
			teamCheckerCount++;
		}
	}
	
		bool firstJumpFound = false;
		For each checker that matches teamName
		{
			if(red || king)
			{
				if(firstJumpFound == false && checker movable)
				{
					add this to moves list
				}
				if(firstJumpFound == true && checker movable)
				{
					do nothing
				}
				if(firstJumpFound == false && checker jumpable)
				{
					delete all moves before this
					firstJumpFound = true
					add this to moves list
					recurse on new board, but have firstJumpFound == true
				}
				if(firstJumpFound == true && checker jumpable)
				{
					add this to moves list
					recurse on new board, but have firstJumpFound == ture
				}
			}
			if(black || king)
			{
				Same thing as red, use different tables.
			}
		}
	
	return returnMe;
}


/*

	00..01..02..03..
	..04..05..06..07
	08..09..10..11..
	..12..13..14..15
	16..17..18..19..
	..20..21..22..23
	24..25..26..27..
	..28..29..30..31

temp key:
	r = red
	R = redKing
	b = black
	B = blackKing
	spa = nothingThere
// */

