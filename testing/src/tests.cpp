#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <random>
#include <algorithm>
#include <mutex>

#include "../../src/defs.h"
#include "../../src/consts.h"
#include "../../src/MatchHandling.h"
#include "../../src/BasicNN.h"
#include "../../src/NN91_Basic.h"
#include "../../src/MinimaxWithAlphaBeta.h"

//error in doubles
const double ERROR = 0.0000001;

TEST_CASE("MatchHandling")
{
	SECTION("Testing Score Struct", "[MatchHandling]")
	{
	    Score score;

	    score = {0, 0, 0};
	    REQUIRE(score == score);

	    score = {0, 0, 0};
	    score.assignScore('D', true);
	    REQUIRE(Score({0, 0, 1}) == score);

	    score = {0, 0, 0};
	    score.assignScore('D', false);
	    REQUIRE(Score({0, 0, 1}) == score);

	    score = {0, 0, 0};
	    score.assignScore('R', true);
	    REQUIRE(Score({1, 0, 0}) == score);

	    score = {0, 0, 0};
	    score.assignScore('R', false);
	    REQUIRE(Score({0, 1, 0}) == score);

	    score = {0, 0, 0};
	    score.assignScore('B', true);
	    REQUIRE(Score({0, 1, 0}) == score);

	    score = {0, 0, 0};
	    score.assignScore('B', false);
	    REQUIRE(Score({1, 0, 0}) == score);
	}

	SECTION("Testing NetTracker struct","[MatchHandling]")
	{
	    NeuralNet nn(NET_SIZE);
	    std::mutex mtx;
	    NetTracker nt;
	    Score score;

	    nt = {&mtx, &nn, 0};
	    REQUIRE(nt == nt);

	    score = {0, 0, 1};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * DRAW_VAL}) == nt);

	    score = {0, 0, 1};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * DRAW_VAL}) == nt);

	    score = {1, 0, 0};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * WIN_VAL}) == nt);

	    score = {1, 0, 0};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * LOSS_VAL}) == nt);

	    score = {0, 1, 0};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * LOSS_VAL}) == nt);

	    score = {0, 1, 0};
	    nt = {&mtx, &nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&mtx, &nn, 1 * WIN_VAL}) == nt);
	}
}

TEST_CASE("TIMING")
{
	bool redPlayerTurn = true;
	std::string theBoard = START_BOARD_STRING;
	int depth = 8;

	SECTION("Checkers constructor timing")
	{
		uint numTiming = 1000000;
		auto CheckersConstructorStart = std::chrono::system_clock::now();
		for(uint i=0; i<numTiming; ++i)
	    {
	    	CheckerBoard(theBoard, redPlayerTurn);
	    }
	    auto CheckersConstructorEnd = std::chrono::system_clock::now();


		std::chrono::duration<double> elapsed_Checkers_constructor = CheckersConstructorEnd - CheckersConstructorStart;
	    std::cout << "Checkers constructor timing: " << elapsed_Checkers_constructor.count() << "s total, or " << elapsed_Checkers_constructor.count() /numTiming << "s/run" << " for " << numTiming << " runs" << std::endl;
	}

	SECTION("Minimax Alpha-Beta timing")
	{
		uint numTiming = 200;
		std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
		NeuralNet net(NET_SIZE);

		auto minimaxABTimerStart = std::chrono::system_clock::now();
		for(uint i = 0; i < numTiming; ++i)
		{
			*clock = std::chrono::system_clock::now();
			MinimaxWithAlphaBeta(theBoard, depth, redPlayerTurn, clock, &net, false);
		}
		auto minimaxABTimerEnd = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsedMinimaxABTimer = minimaxABTimerEnd - minimaxABTimerStart;
		std::cout << "Minimax Alpha-Beta constructor timing: " << elapsedMinimaxABTimer.count() << "s total, or " << elapsedMinimaxABTimer.count() / numTiming << "s/run"
				  << " for " << numTiming << " runs" << std::endl;
	}


	
}