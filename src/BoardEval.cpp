#include "BoardEval.h"

PieceMap::PieceMap(std::string &currBoard) {
	for (char &c : currBoard) {
		switch(c) {
			case 'R':
				rKing++;
				break;
			case 'r':
				rChip++;
				break;
			case 'B':
				bKing++;
				break;
			case 'b':
				bChip++;
				break;
			default:
				break;
		}
	}
}

int basicBoardEval(std::string &currBoard, bool redTeamTurn)
{
	PieceMap board(currBoard);

	int currTeamCheckers = redTeamTurn ? board.redPieces() : board.blackPieces();
	int otherTeamCheckers = redTeamTurn ? board.blackPieces() : board.redPieces();

	if(currTeamCheckers == 0)
	{
		return -10000;
	}
	else if(otherTeamCheckers == 0)
	{
		return 10000;
	}
	return currTeamCheckers - otherTeamCheckers;
}

float weightedBoardEval(std::string &currBoard, float kingVal, bool redTeamTurn){
	// std::cout << "weighted board eval" << std::endl;
	PieceMap board(currBoard);

	float currTeamCheckers = redTeamTurn ? board.weightedRedPieces(kingVal) : board.weightedBlackPieces(kingVal);
	float otherTeamCheckers = redTeamTurn ? board.weightedBlackPieces(kingVal) : board.weightedRedPieces(kingVal);

	if(currTeamCheckers == 0)
		return -10000.0f;
	else if (otherTeamCheckers == 0)
		return 10000.0f;

	return currTeamCheckers - otherTeamCheckers;
}

int totalPieceCount(std::string &currBoard) {
	PieceMap board(currBoard);
	return board.totalPieces();
}

int rPieceCount(std::string &currBoard) {
	PieceMap board(currBoard);
	return board.redPieces();
}

int bPieceCount(std::string &currBoard) {
	PieceMap board(currBoard);
	return board.blackPieces();
}
