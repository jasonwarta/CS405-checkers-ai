#ifndef BASICBOARDEVAL_H_INCLUDED
#define BASICBOARDEVAL_H_INCLUDED


#include <iostream>
#include <string>
#include <math.h>
#include <tuple>
#include <functional>

int basicBoardEval(std::string &currBoard, bool redTeamTurn);
int totalPieceCount(std::string &currBoard);
std::tuple<int,int> pieceCount(std::string &currBoard);
int rPieceCount(std::string &currBoard);
int bPieceCount(std::string &currBoard);

#endif
