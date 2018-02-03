





#include "consts.h"
#include "checkers.h"



	/*
		==================================================== THE CHECKER CLASS ====================================================
	*/
	TheChecker::TheChecker(bool onRedTeam, bool pieceKing)
	{
		isOnRed_ = onRedTeam;
		isKing_ = pieceKing;
	}
	bool TheChecker::isTeamRed()
	{
		return isOnRed_;
	}
	bool TheChecker::isKing()
	{
		return isKing_;
	}
	void TheChecker::setKing(bool b)
	{
		isKing_ = b;
	}

	/*
		==================================================== CHECKER BOARD CLASS ====================================================
	*/

	CheckerBoard::CheckerBoard(std::string startBoard, bool redPlayerTurn)
	{
		redTeamTurn_ = redPlayerTurn;
		firstJumpFound_ = false;

		if(redTeamTurn_)
		{
			currTeamMoveBoard_ = RED_MOVE_BOARD;
			currTeamJumpBoard_ = RED_JUMP_BOARD;
			oppTeamMoveBoard_ = BLACK_MOVE_BOARD;
			oppTeamJumpBoard_ = BLACK_JUMP_BOARD;
		}
		else
		{
			currTeamMoveBoard_ = BLACK_MOVE_BOARD;
			currTeamJumpBoard_ = BLACK_JUMP_BOARD;
			oppTeamMoveBoard_ = RED_MOVE_BOARD;
			oppTeamJumpBoard_ = RED_JUMP_BOARD;
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

		}
		// Start figuring out what can go where...
		updatePossibleMoves();
	}

	std::string CheckerBoard::getRandoMove()
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

	std::vector<std::string> CheckerBoard::getAllRandoMoves()
	{
		return possibleMoves_;
	}

	std::string CheckerBoard::turnBoardToString()
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

		return returnMe;
	}

	void CheckerBoard::JumpingRecursion(int i, int j, bool currTeamDirection) // currTeamDirection gets passed going right way
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
			if(currTeamMoveBoard_[k][l] != -1 && checkers_[currTeamMoveBoard_[k][l]] != nullptr && checkers_[currTeamMoveBoard_[k][l]]->isTeamRed() != redTeamTurn_ && currTeamJumpBoard_[k][l] != -1 && checkers_[currTeamJumpBoard_[k][l]] == nullptr)
			{
				jumpNeverFound = false;
				JumpingRecursion(k, l, true);
			}
			// if king, check other direction too
			if(checkers_[k]->isKing())
			{
				if(oppTeamMoveBoard_[k][l] != -1 && checkers_[oppTeamMoveBoard_[k][l]] != nullptr && checkers_[oppTeamMoveBoard_[k][l]]->isTeamRed() != redTeamTurn_ && oppTeamJumpBoard_[k][l] != -1 && checkers_[oppTeamJumpBoard_[k][l]] == nullptr)
				{
					jumpNeverFound = false;
					JumpingRecursion(k, l, false);
				}
			}
		}

		// if at tail of jump sequence, board is a possible answer
		if(jumpNeverFound)
		{
			//isAlreadyKing default to true so that it bypasses checks if it doesn't get changed
			bool isAlreadyKing = true;
			
			if(currTeamDirection)
			{
				isAlreadyKing = checkers_[currTeamJumpBoard_[i][j]]->isKing();
				if(!isAlreadyKing && ((redTeamTurn_ && currTeamJumpBoard_[i][j]>27) || (!redTeamTurn_ && currTeamJumpBoard_[i][j]<4)))
				{
					checkers_[currTeamJumpBoard_[i][j]]->setKing(true);

				}
			}
			
			possibleMoves_.push_back(turnBoardToString());
			
			if(currTeamDirection && !isAlreadyKing && ((redTeamTurn_ && currTeamJumpBoard_[i][j]>27) || (!redTeamTurn_ && currTeamJumpBoard_[i][j]<4)))
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
	void CheckerBoard::moveJumpManager(int &i, int &j, bool goingRightWay)
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
				if(!isAlreadyKing && ((redTeamTurn_ && teamMoveBoard[i][j]>27) || (!redTeamTurn_ && teamMoveBoard[i][j]<4)))
				{
					checkers_[teamMoveBoard[i][j]]->setKing(true);
				}

				possibleMoves_.push_back(turnBoardToString());

				if(!isAlreadyKing && ((redTeamTurn_ && teamMoveBoard[i][j]>27) || (!redTeamTurn_ && teamMoveBoard[i][j]<4)))
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

				JumpingRecursion(i, j, goingRightWay);
			}
		}
	}

	void CheckerBoard::updatePossibleMoves()
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






