#include "BasicBoardEval.h"

int basicBoardEval(std::string &currBoard, bool redTeamTurn)
{
	std::tuple<int,int> count = pieceCount(currBoard);
	int redC = std::get<0>(count);
	int blackC = std::get<1>(count);

	int currTeamCheckers = redTeamTurn ? redC : blackC;
	int otherTeamCheckers = redTeamTurn ? blackC : redC;

	if(currTeamCheckers == 0)
	{
		return -1;
	}
	else if(otherTeamCheckers == 0)
	{
		return 1;
	}

	// tanh((float)currTeamCheckers - (float)otherTeamCheckers)
	return currTeamCheckers - otherTeamCheckers;
}
int totalPieceCount(std::string &currBoard) {
	std::tuple<int,int> count = pieceCount(currBoard);
	return std::get<0>(count) + std::get<1>(count);
}

std::tuple<int,int> pieceCount(std::string &currBoard) {
	size_t rc = 0, bc = 0;
	for(char & c : currBoard) {
		if (c == 'R' || c == 'r')
			rc++;
		if (c == 'B' || c == 'b')
			bc++;
	}
	return std::make_tuple(rc,bc);
}

int rPieceCount(std::string &currBoard) {
	size_t counter = 0;
	for(char & c : currBoard) {
		if (c == 'R' || c == 'r')
			counter++;
	}
	return counter;
}

int bPieceCount(std::string &currBoard) {
	size_t counter = 0;
	for(char & c : currBoard) {
		if (c == 'B' || c == 'b')
			counter++;
	}
	return counter;
}
