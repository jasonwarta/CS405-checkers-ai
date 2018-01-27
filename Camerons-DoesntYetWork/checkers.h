
// update later




#ifndef CHECKERS_H_INCLUDED
#define CHECKERS_H_INCLUDED

#include <vector>
#include <iostream>
#include <memory>

void makeStartBoard(std::vector<char> &v);


void printTempBoard(std::vector<char> &v);

std::vector<std::vector<char>> generatePossibleMoves(std::vector<char> currentBoard, std::string TeamName);

void tempTester();


// boardPosition may not be needed. Look into later. (Just use vector element)
class TheChecker
{
public:
	TheChecker(bool onRedTeam, int ID, bool pieceKing)
	{
		isOnRed_ = onRedTeam;
		isKing_ = pieceKing;
		checkerID_ = ID;
		//spotOnBoard_ = boardPosition;
	}
	bool isTeamRed()
	{
		return isOnRed_;
	}
	bool isKing()
	{
		return isKing_;
	}
private:
	bool isOnRed_;
	bool isKing_;
	int checkerID_;
	//int spotOnBoard_;
};

class CheckerBoard
{
public:
	// startBoard is the 32 vector
	CheckerBoard(std::vector<char> startBoard, bool redPlayerTurn, std::vector<std::vector<int>> redMoveBoard, std::vector<std::vector<int>> redJumpBoard)
	{
		piecesOnRedTeam_ = 0;
		piecesOnBlackTeam_ = 0;
		redTeamTurn = redPlayerTurn;
		redMoveBoard_ = redMoveBoard; // Ill fix this soon... promise
		redJumpBoard_ = redJumpBoard;
		// that weird vector resize command here later


		for(int i=0; i<startBoard.size(); ++i)
		{
			// later, break at space first for speedup?
			// also, change pointers to sharedpointers later
			// might condence the if tree later.... lazy atm
			if(startBoard[i] == 'r')
			{
				checkers_.push_back(std::make_shared<TheChecker>(TheChecker(true, piecesOnRedTeam_, false)) );
				piecesOnRedTeam_++;
			}
			else if(startBoard[i] == 'R')
			{
				checkers_.push_back(std::make_shared<TheChecker>(TheChecker(true, piecesOnRedTeam_, true)) );
				piecesOnRedTeam_++;
			}
			else if(startBoard[i] == 'b')
			{
				checkers_.push_back(std::make_shared<TheChecker>(TheChecker(false, piecesOnBlackTeam_, false)) );
				piecesOnBlackTeam_++;
			}
			else if(startBoard[i] == 'B')
			{
				checkers_.push_back(std::make_shared<TheChecker>(TheChecker(false, piecesOnBlackTeam_, true)) );
				piecesOnBlackTeam_++;
			}
			else if(startBoard[i] == ' ')// nothing there
			{
				checkers_.push_back(nullptr); //NULL or *NULL? test later....
			}
			else
			{
				std::cout << "FINALLY FOUND THAT FUCKKKING BUGGGGG!!!!!!!" << std::endl; // nope....
			}
		}
	}

	std::vector<char> turnBoardtoVec()
	{
		std::vector<char> returnMe;
		for(int i=0; i<checkers_.size(); i++)
		{
			if(checkers_[i] == nullptr)
			{
				returnMe.push_back(' ');
			}
			else if(checkers_[i]->isTeamRed() == true)
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe.push_back('R');
				}
				else
				{
					returnMe.push_back('r');
				}
			}
			else
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe.push_back('B');
				}
				else
				{
					returnMe.push_back('b');
				}
			}
		}

		return returnMe;
	}


/*
	returnish updatePossibleMovesRecurse()
	{

	}
*/
	/*
		I might cut the length of updatePosibleMoves in half later by switching the tables back
		and forth as variables, but for now im too tired and this is easier....
	*/
	void updatePossibleMoves()
	{
		
		std::cout << "test";
		bool firstJumpFound = false;
		for(int i=0; i<checkers_.size(); ++i)
		{
			// If no checker on that square, or wrong teams checker, do nothing....
			if(checkers_[i] == nullptr || checkers_[i]->isTeamRed() != redTeamTurn)
			{
				continue;
			}
			// Kings can do whatever they want. Otherwise, stick to your own tables
			if(checkers_[i]->isKing() == true || redTeamTurn == true)
			{
				//for loop because 0 deals with left, and 1 deals with right
				//check both sides independently
				for(int j=0; j<2; j++)
				{					
					if(redMoveBoard_[i][j] != -1)
					{
						
						if(checkers_[redMoveBoard_[i][j]] == nullptr) 
						{
							if(firstJumpFound == false)
							{
								possibleMoves_.push_back(turnBoardtoVec());
							}
						}
						else if(checkers_[redMoveBoard_[i][j]]->isTeamRed() != redTeamTurn)
						{
							if(redJumpBoard_[i][j] != -1 && checkers_[redJumpBoard_[i][j]] == nullptr)
							{
								// Jump found
								if(firstJumpFound == false)
								{
									//everything in possibleMoves are not jumps, so wipe it
									possibleMoves_.clear();
									firstJumpFound = true;
								}
								possibleMoves_.push_back(turnBoardtoVec());

								//----
								// recurse on Jump here
								// Note to self: Recurse WILL have to take into account both tables at all times for kings
								// since a king can jump up, down, and up again.
								//----
							}
						}
						
					}
					

				}

			}
			if(checkers_[i]->isKing() == true || redTeamTurn == false)
			{

			}

		}
		
	}

	std::vector<std::vector<char>> getPossibleMoves()
	{
		return possibleMoves_;
	}
private:
	// pointers to all the checkers
	std::vector<std::shared_ptr<TheChecker>> checkers_;
	// to set ID of each piece. (0-11 on each side)
	int piecesOnRedTeam_;
	int piecesOnBlackTeam_;
	// will have all possible moves, each vector is a different board
	std::vector<std::vector<char>> possibleMoves_;
	// which turn
	bool redTeamTurn;
	// May think of a better way of putting these here later... too tired. It works.
	std::vector<std::vector<int>> redMoveBoard_;
	std::vector<std::vector<int>> redJumpBoard_;


	//std::vector<*TheChecker> redCheckers_;
	//std::vector<*TheChecker> blackCheckers_;
};


#endif // CHECKERS_H_INLCUDED