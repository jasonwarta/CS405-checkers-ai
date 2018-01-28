
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
	TheChecker(bool onRedTeam, bool pieceKing)
	{
		isOnRed_ = onRedTeam;
		isKing_ = pieceKing;
	}
	bool isTeamRed()
	{
		return isOnRed_;
	}
	bool isKing()
	{
		return isKing_;
	}
	void setKing(bool b)
	{
		isKing_ = b;
	}
private:
	bool isOnRed_;
	bool isKing_;
};

class CheckerBoard
{
public:
	// startBoard is the 32 vector
	CheckerBoard(std::vector<char> startBoard, bool redPlayerTurn, std::vector<std::vector<int>> &redMove, std::vector<std::vector<int>> &redJump, std::vector<std::vector<int>> &blackMove, std::vector<std::vector<int>> &blackJump)
	{
		redTeamTurn_ = redPlayerTurn;
		if(redTeamTurn_)
		{
			currTeamMoveBoard_ = redMove;
			currTeamJumpBoard_ = redJump;
			oppTeamMoveBoard_ = blackMove;
			oppTeamJumpBoard_ = blackJump;
		}
		else
		{
			currTeamMoveBoard_ = blackMove;
			currTeamJumpBoard_ = blackJump;
			oppTeamMoveBoard_ = redMove;
			oppTeamJumpBoard_ = redJump;
		}


		checkers_.resize(32);
		for(int i=0; i<startBoard.size(); ++i)
		{
			// later, break at space first for speedup?
			// also, change pointers to sharedpointers later
			// might condence the if tree later.... lazy atm
			if(startBoard[i] == 'r')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, false));
			}
			else if(startBoard[i] == 'R')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, true));
			}
			else if(startBoard[i] == 'b')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, false));
			}
			else if(startBoard[i] == 'B')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, true));
			}
			else if(startBoard[i] == ' ')// nothing there
			{
				checkers_[i] = nullptr; //NULL or *NULL? test later....
			}
			else
			{
				std::cout << "FINALLY FOUND THAT FUCKKKING BUGGGGG!!!!!!!" << std::endl; // nope....
			}
		}
	}

	std::vector<char> turnBoardtoVec()
	{
		std::vector<char> returnMe(32);
		for(int i=0; i<checkers_.size(); i++)
		{
			if(checkers_[i] == nullptr)
			{
				returnMe[i] = ' ';
			}
			else if(checkers_[i]->isTeamRed() == true)
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe[i] = 'R';
				}
				else
				{
					returnMe[i] = 'r';
				}
			}
			else
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe[i] = 'B';
				}
				else
				{
					returnMe[i] = 'b';
				}
			}
		}

		return returnMe;
	}

	void updatePossibleMovesRecurse(int i, int j, bool currTeamDirection)
	{
		std::shared_ptr<TheChecker> pieceJumped;
		int k;
		bool jumpNeverFound = true;

		if(currTeamDirection == true)
		{
			pieceJumped = checkers_[currTeamMoveBoard_[i][j]];
			checkers_[currTeamMoveBoard_[i][j]] = nullptr;
			checkers_[currTeamJumpBoard_[i][j]] = checkers_[i];
			checkers_[i] = nullptr;
			k = currTeamJumpBoard_[i][j];
		}
		else // currTeamDirection == false
		{
			pieceJumped = checkers_[oppTeamMoveBoard_[i][j]];
			checkers_[oppTeamMoveBoard_[i][j]] = nullptr;
			checkers_[oppTeamJumpBoard_[i][j]] = checkers_[i];
			checkers_[i] = nullptr;
			k = oppTeamJumpBoard_[i][j];
		}

		for(int l=0; l<2; ++l)
		{
			// if "Can move that way" && "Theres a checker that way" && "That checker is not yours" && "The space to jump it is empty"
			if(currTeamMoveBoard_[k][l] != -1 && checkers_[currTeamMoveBoard_[k][l]] != nullptr && 
				checkers_[currTeamMoveBoard_[k][l]]->isTeamRed() != redTeamTurn_ && checkers_[currTeamJumpBoard_[k][l]] == nullptr)
			{
				updatePossibleMovesRecurse(k, l, true);
				jumpNeverFound = false;
			}

			// if king, check other direction too
			if(checkers_[k]->isKing())
			{
				if(oppTeamMoveBoard_[k][l] != -1 && checkers_[oppTeamMoveBoard_[k][l]] != nullptr && 
					checkers_[oppTeamMoveBoard_[k][l]]->isTeamRed() != redTeamTurn_ && checkers_[oppTeamJumpBoard_[k][l]] == nullptr)
				{
					updatePossibleMovesRecurse(k, l, false);
					jumpNeverFound = false;
				}
			}
		}
		// if at tail of jump sequence, board is a possible answer
		if(jumpNeverFound == true)
		{

			//isAlreadyKing default to true so that it bypasses checks if it doesn't get changed
			bool isAlreadyKing = true;
			if(currTeamDirection)
			{
				isAlreadyKing = checkers_[currTeamJumpBoard_[i][j]]->isKing();
				if(!isAlreadyKing && (redTeamTurn_ && currTeamJumpBoard_[i][j]>27) || (!redTeamTurn_ && currTeamJumpBoard_[i][j]<4))
				{
					checkers_[currTeamJumpBoard_[i][j]]->setKing(true);

				}

			}

			possibleMoves_.push_back(turnBoardtoVec());

			if(currTeamDirection && !isAlreadyKing && (redTeamTurn_ && currTeamJumpBoard_[i][j]>27) || (!redTeamTurn_ && currTeamJumpBoard_[i][j]<4))
			{
				checkers_[currTeamJumpBoard_[i][j]]->setKing(false);

			}
		}


		// undo the beginning for previous recurse call
		if(currTeamDirection == true)
		{
			checkers_[i] = checkers_[currTeamJumpBoard_[i][j]];
			checkers_[currTeamJumpBoard_[i][j]] = nullptr;
			checkers_[currTeamMoveBoard_[i][j]] = pieceJumped;
		}
		else
		{
			checkers_[i] = checkers_[oppTeamJumpBoard_[i][j]];
			checkers_[oppTeamJumpBoard_[i][j]] = nullptr;
			checkers_[oppTeamMoveBoard_[i][j]] = pieceJumped;
		}
		pieceJumped = nullptr; //necessary?
	}


	void updateBoardWithDirection(int &i, int &j, bool &firstJumpFound, bool goingRightWay, std::vector<std::vector<int>> &teamMoveBoard, std::vector<std::vector<int>> &teamJumpBoard)
	{
		if(teamMoveBoard[i][j] != -1)
		{
			
			if(checkers_[teamMoveBoard[i][j]] == nullptr) 
			{
				if(firstJumpFound == false)
				{
					//move the checker to the new spot
					checkers_[teamMoveBoard[i][j]] = checkers_[i];
					checkers_[i] = nullptr;

					//If already a king, dont change it. If not, change it and change it back
					bool isAlreadyKing = checkers_[teamMoveBoard[i][j]]->isKing();
					if(!isAlreadyKing && (redTeamTurn_ && teamMoveBoard[i][j]>27) || (!redTeamTurn_ && teamMoveBoard[i][j]<4))
					{
						checkers_[teamMoveBoard[i][j]]->setKing(true);
					}

					possibleMoves_.push_back(turnBoardtoVec());

					if(!isAlreadyKing && (redTeamTurn_ && teamMoveBoard[i][j]>27) || (!redTeamTurn_ && teamMoveBoard[i][j]<4))
					{
						checkers_[teamMoveBoard[i][j]]->setKing(false);
					}

					//put the checker back
					checkers_[i] = checkers_[teamMoveBoard[i][j]];
					checkers_[teamMoveBoard[i][j]] = nullptr;


				}
			}
			else if(checkers_[teamMoveBoard[i][j]]->isTeamRed() != redTeamTurn_)
			{
				// if it wont jump off the board, and the spot it can jump to is empty
				if(teamJumpBoard[i][j] != -1 && checkers_[teamJumpBoard[i][j]] == nullptr)
				{
					// Jump found
					if(firstJumpFound == false)
					{
						//everything in possibleMoves are not jumps, so wipe it
						possibleMoves_.clear();
						firstJumpFound = true;
					}
					//possibleMoves_.push_back(turnBoardtoVec());
					updatePossibleMovesRecurse(i, j, goingRightWay);

				}
			}
			
		}
	}


	/*
		I might cut the length of updatePosibleMoves in half later by switching the tables back
		and forth as variables, but for now im too tired and this is easier....
	*/
	void updatePossibleMoves()
	{
		bool firstJumpFound = false;
		for(int i=0; i<checkers_.size(); ++i)
		{
			// If no checker on that square, or wrong teams checker, do nothing....
			if(checkers_[i] == nullptr || checkers_[i]->isTeamRed() != redTeamTurn_)
			{
				continue;
			}

			//for loop because 0 deals with left, and 1 deals with right
			//check both sides independently
			for(int j=0; j<2; j++)
			{	

				updateBoardWithDirection(i, j, firstJumpFound, true, currTeamMoveBoard_, currTeamJumpBoard_);

				if(checkers_[i]->isKing())
				{
					updateBoardWithDirection(i, j, firstJumpFound, false, oppTeamMoveBoard_, oppTeamJumpBoard_);
				}

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

	// will have all possible moves, each vector is a different board
	std::vector<std::vector<char>> possibleMoves_;
	// which turn
	bool redTeamTurn_;

	// May think of a better way of putting these here later... too tired. It works.
	std::vector<std::vector<int>> currTeamMoveBoard_;
	std::vector<std::vector<int>> oppTeamMoveBoard_;
	std::vector<std::vector<int>> currTeamJumpBoard_;
	std::vector<std::vector<int>> oppTeamJumpBoard_;

};


#endif // CHECKERS_H_INLCUDED