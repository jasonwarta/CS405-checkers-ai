#include "checkers.h"

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
					 'R','_','_','_',
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

int main()
{
	std::string boardVec;
	std::vector<std::string> possibleMoves;

	makeStartBoard(boardVec);
	printTempBoard(boardVec);

	bool redPlayerTurn = true;
	CheckerBoard theCheckerBoard(boardVec, redPlayerTurn); // bestNameEver....
	possibleMoves = theCheckerBoard.getPossibleMoves();


	std::cout << "NUMBER OF POSSIBLE MOVES: " << possibleMoves.size() << std::endl;
	for(int i=0; i<possibleMoves.size(); ++i) {
		std::cout << "-----------------" << " possibleMoves #" << i << " -----------------" << std::endl;
		printTempBoard(possibleMoves[i]);
	}
	return 0;
}