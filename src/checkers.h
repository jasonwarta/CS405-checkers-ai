
// update later




#ifndef CHECKERS_H_INCLUDED
#define CHECKERS_H_INCLUDED

#include <vector>
#include <iostream>
#include <memory>
#include <string>

#include <random>




void makeStartBoard(std::vector<char> &v);

void printTempBoard(std::vector<char> &v);

std::string tempTester();




// May change from class to something else later...
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
	CheckerBoard(std::string startBoard, bool redPlayerTurn, std::vector<std::vector<int>> &redMove, std::vector<std::vector<int>> &redJump, std::vector<std::vector<int>> &blackMove, std::vector<std::vector<int>> &blackJump)
	{
		redTeamTurn_ = redPlayerTurn;
		firstJumpFound_ = false;

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
			if(startBoard.at(i) == 'r')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, false));
			}
			else if(startBoard.at(i) == 'R')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(true, true));
			}
			else if(startBoard.at(i) == 'b')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, false));
			}
			else if(startBoard.at(i) == 'B')
			{
				checkers_[i] = std::make_shared<TheChecker>(TheChecker(false, true));
			}
			else if(startBoard.at(i) == '_')// nothing there
			{
				checkers_[i] = nullptr; //NULL or *NULL? test later....
			}
			else
			{
				std::cout << "FINALLY FOUND THAT FUCKKKING BUGGGGG!!!!!!!" << std::endl; // nope....
			}
		}
		// Start figuring out what can go where...
		updatePossibleMoves();
	}

	std::string getRandoMove()
	{
	    std::random_device rd;
	    std::mt19937 rando(rd());
	    if(possibleMoves_.size() == 0)
	    {
	    	// if no moves, still return something so you wont crash
	    	std::cout << "NO MOVES FOUND. RETURNING BLANK BOARD. FIX LATER!!!!" << std::endl;
	        return "________________________________";
	    }
	    else if(possibleMoves_.size() == 1)
	    {
	    	// if only option, just return it
	    	return possibleMoves_[0];
	    }
	    else
	    {
	        std::uniform_int_distribution<> movePicked(0, (possibleMoves_.size()-1));
	        //int temp = movePicked(rando);
	        //std::cout << possibleMoves[temp] << std::endl;
	        return possibleMoves_[movePicked(rando)];
	    }
	}

	std::vector<std::string> getAllRandoMoves()
	{
		return possibleMoves_;
	}



	private:
	std::string turnBoardtoVec()
	{
		std::string returnMe = "";
		for(int i=0; i<checkers_.size(); i++)
		{
			if(checkers_[i] == nullptr)
			{
				returnMe += '_';
			}
			else if(checkers_[i]->isTeamRed() == true)
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe += 'R';
				}
				else
				{
					returnMe += 'r';
				}
			}
			else // black team
			{
				if(checkers_[i]->isKing() == true)
				{
					returnMe += 'B';
				}
				else
				{
					returnMe += 'b';
				}
			}
		}
		if(returnMe.size() != 32)
		{
			std::cout << "Look closer at this" << std::endl;
		}

		return returnMe;
	}

	void JumpingRecursion(int i, int j, bool currTeamDirection)
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
				JumpingRecursion(k, l, true);
				jumpNeverFound = false;
			}

			// if king, check other direction too
			if(checkers_[k]->isKing())
			{
				if(oppTeamMoveBoard_[k][l] != -1 && checkers_[oppTeamMoveBoard_[k][l]] != nullptr && 
					checkers_[oppTeamMoveBoard_[k][l]]->isTeamRed() != redTeamTurn_ && checkers_[oppTeamJumpBoard_[k][l]] == nullptr)
				{
					JumpingRecursion(k, l, false);
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

	// You can also not include the teamMoveBoard and teamJumpBoard, and just say if goingRightWay == true, use currTeamBoard, else use oppTeamBoard.
	// 
	void moveJumpManager(int &i, int &j, bool goingRightWay)
	{

		// Use the correct boards:
		std::vector<std::vector<int>> teamMoveBoard;
		std::vector<std::vector<int>> teamJumpBoard;

		if(goingRightWay == true)
		{
			teamMoveBoard = currTeamMoveBoard_;
			teamJumpBoard = currTeamJumpBoard_;
		}
		else // A king using the other board:
		{
			teamMoveBoard = oppTeamMoveBoard_;
			teamJumpBoard = oppTeamJumpBoard_;
		}
		
		// if you can't even move that direction, don't check for other options
		if(teamMoveBoard[i][j] == -1)
		{
			return;
		}
		if(checkers_[teamMoveBoard[i][j]] == nullptr) 
		{
			if(firstJumpFound_ == false)
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
				if(firstJumpFound_ == false)
				{
					//everything in possibleMoves are not jumps, so wipe it
					possibleMoves_.clear();
					firstJumpFound_ = true;
				}
				//possibleMoves_.push_back(turnBoardtoVec());
				JumpingRecursion(i, j, goingRightWay);

			}
		}
	}

	void updatePossibleMoves()
	{
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

				moveJumpManager(i, j, true);

				if(checkers_[i]->isKing())
				{
					moveJumpManager(i, j, false);
				}

			}
		}
		
	}




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
