

#include <iostream>
#include <string>
#include <math.h>

float basicBoardEval(std::string currBoard, bool redTeamTurn)
{
	int redC = 0;
	int blackC = 0;

	for(int i=0; i<currBoard.size(); ++i)
	{
		if(currBoard.at(i) == 'r' || currBoard.at(i) == 'R')
		{
			redC++;
		}
		else if(currBoard.at(i) == 'b' || currBoard.at(i) == 'B')
		{
			blackC++;
		}
	}

	int currTeamCheckers;
	int otherTeamCheckers;

	if(redTeamTurn)
	{
		currTeamCheckers = redC;
		otherTeamCheckers = blackC;
	}
	else
	{
		currTeamCheckers = blackC;
		otherTeamCheckers = redC;
	}

	if(currTeamCheckers == 0)
	{
		return 1.0f;
	}
	else if(otherTeamCheckers == 0)
	{
		return -1.0f;
	}


	return tanh((float)currTeamCheckers - (float)otherTeamCheckers);


}