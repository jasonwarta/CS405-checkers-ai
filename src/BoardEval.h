#ifndef BOARDEVAL_H_INCLUDED
#define BOARDEVAL_H_INCLUDED


#include <iostream>
#include <string>
#include <math.h>
#include <tuple>
#include <functional>

struct PieceMap {
	PieceMap(std::string &currBoard);

	int rKing = 0;
	int rChip = 0;

	int bKing = 0;
	int bChip = 0;

	int totalPieces() { return rKing + rChip + bKing + bChip; }
	int redPieces() { return rKing + rChip; }
	int blackPieces() { return bKing + bChip; }

	float weightedRedPieces(float weight) { return (rKing*weight) + rChip; }
	float weightedBlackPieces(float weight) { return (bKing*weight) + bChip; }
};

int basicBoardEval(std::string &currBoard, bool redTeamTurn);
float weightedBoardEval(std::string &currBoard, float kingVal, bool redTeamTurn);

int totalPieceCount(std::string &currBoard);
int rPieceCount(std::string &currBoard);
int bPieceCount(std::string &currBoard);

#endif
