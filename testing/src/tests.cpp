#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <random>
#include <algorithm>
#include <mutex>
#include <cmath> //abs

#include "../../src/defs.h"
#include "../../src/consts.h"
#include "../../src/MatchHandling.h"
#include "../../src/BasicNN.h"
#include "../../src/NN91_Basic.h"
#include "../../src/MinimaxWithAlphaBeta.h"
#include "../../src/MinimaxTree.h"

//error in doubles
const double ERROR = 0.0000001;

TEST_CASE("MatchHandling")
{
	bool redPlayerTurn = true;
	std::string theBoard = START_BOARD_STRING;
	int depth = 8;

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

	    nt = {&nn, 0};
	    REQUIRE(nt == nt);

	    score = {0, 0, 1};
	    nt = {&nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&nn, 0, 1 * DRAW_VAL}) == nt);

	    score = {0, 0, 1};
	    nt = {&nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&nn, 0, 1 * DRAW_VAL}) == nt);

	    score = {1, 0, 0};
	    nt = {&nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&nn, 0, 1 * WIN_VAL}) == nt);

	    score = {1, 0, 0};
	    nt = {&nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&nn, 0, 1 * LOSS_VAL}) == nt);

	    score = {0, 1, 0};
	    nt = {&nn, 0};
	    nt.assignScore(&score, true);
	    REQUIRE(NetTracker({&nn, 0, 1 * LOSS_VAL}) == nt);

	    score = {0, 1, 0};
	    nt = {&nn, 0};
	    nt.assignScore(&score, false);
	    REQUIRE(NetTracker({&nn, 0, 1 * WIN_VAL}) == nt);
	   
	}

	SECTION("Minimax equivalence testing")
	{
		NeuralNet nn(NET_SIZE);
		bool redTeam = true;
		std::string theBoard = START_BOARD_STRING;
		int depth;

		// using piece count
		depth = 2;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam).getBestBoard());

		depth = 4;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam).getBestBoard());

		depth = 6;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam).getBestBoard());

		depth = 8;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam).getBestBoard());

		// using NN
		depth = 2;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam, &nn).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam, &nn).getBestBoard());

		depth = 4;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam, &nn).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam, &nn).getBestBoard());

		depth = 6;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam, &nn).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam, &nn).getBestBoard());

		depth = 8;
		REQUIRE(MinimaxTree(theBoard, depth, redTeam, &nn).getBestBoard() == MinimaxWithAlphaBeta(theBoard, depth, redTeam, &nn).getBestBoard());
	}

	SECTION("BasicNN correctness tests")
	{
		
		std::vector<uint> size{32, 40, 10, 1};
		BasicNN Net0(size);
		BasicNN Net1(size);


		INFO("Two different eval's called on same Class. Should be same. (used to not reset)");
		REQUIRE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0.evaluateNN(theBoard, redPlayerTurn)) < ERROR);

		INFO("Same board and NN, different teams")
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0.evaluateNN(theBoard, !redPlayerTurn)) < ERROR);

		INFO("Two different NN's won't have the same network");
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);

		// make sure evolving changes the NN, and that evolving the same NN twice gives two different NN's
		BasicNN Net0copy0 = Net0;
		BasicNN Net0copy1 = Net0;

		INFO("Copy constructor works");
		REQUIRE(fabs(Net0copy0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);

		Net0copy0.evolve();
		Net0copy1.evolve();
		INFO("evolving changes the NN");
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0copy0.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
		REQUIRE_FALSE(fabs(Net0copy0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
		
	}
	SECTION("NN91_Basic correctness tests")
	{
		
		std::vector<uint> size{5,4,3};
		NN91_Basic Net0(size);
		NN91_Basic Net1(size);


		INFO("Two different eval's called on same Class. Should be same. (one is faster than the other)");
		REQUIRE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0.SLOWevaluateNN(theBoard, redPlayerTurn)) < ERROR);

		INFO("Same board and NN, different teams")
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0.evaluateNN(theBoard, !redPlayerTurn)) < ERROR);

		INFO("Two different NN's won't have the same network");
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);

		// make sure evolving changes the NN, and that evolving the same NN twice gives two different NN's
		NN91_Basic Net0copy0 = Net0;
		NN91_Basic Net0copy1 = Net0;

		INFO("Copy constructor works");
		REQUIRE(fabs(Net0copy0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);

		Net0copy0.evolve();
		Net0copy1.evolve();
		INFO("evolving changes the NN");
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0copy0.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
		REQUIRE_FALSE(fabs(Net0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
		REQUIRE_FALSE(fabs(Net0copy0.evaluateNN(theBoard, redPlayerTurn) - Net0copy1.evaluateNN(theBoard, redPlayerTurn)) < ERROR);
	}
}

TEST_CASE("TIMING")
{
	bool redPlayerTurn = true;
	std::string theBoard = START_BOARD_STRING;

	SECTION("Checkers constructor timing")
	{
		uint numTiming = 100000;
		auto CheckersConstructorStart = std::chrono::system_clock::now();
		for(uint i=0; i<numTiming; ++i)
	    {
	    	CheckerBoard(theBoard, redPlayerTurn);
	    }
	    auto CheckersConstructorEnd = std::chrono::system_clock::now();


		std::chrono::duration<double> elapsed_Checkers_constructor = CheckersConstructorEnd - CheckersConstructorStart;
	    std::cout << "Checkers constructor timing: " << elapsed_Checkers_constructor.count() << "s total, or " << elapsed_Checkers_constructor.count() /numTiming << "s/run" << " for " << numTiming << " runs" << std::endl;
	}
	SECTION("BasicNN evaluation timing")
	{
		uint numTiming = 500000;
		std::vector<uint> size{32,40,10,1};
		BasicNN Net0(size);
		float startVal = Net0.evaluateNN(theBoard, redPlayerTurn);
		float trash = 0;

		auto BasicNNStart = std::chrono::system_clock::now();
		for(uint i=0; i<numTiming; ++i)
		{
			trash += Net0.evaluateNN(theBoard, redPlayerTurn);
		}
		auto BasicNNEnd = std::chrono::system_clock::now();

		float endVal = Net0.evaluateNN(theBoard, redPlayerTurn);

		INFO("If different, evaluateNN somehow modfies the edges.");
		REQUIRE(fabs(endVal - startVal) < ERROR);

		std::chrono::duration<double> elapsedBasicNNTimer = BasicNNEnd - BasicNNStart;
	    std::cout << "BasicNN timing: " << elapsedBasicNNTimer.count() << "s total, or " << elapsedBasicNNTimer.count() /numTiming << "s/run" << " for " << numTiming << " runs" << " trash: " << trash << std::endl;
	}
	SECTION("NN91 evaluation timing")
	{
		uint numTiming = 500000;
		std::vector<uint> size{15,8,2};
		NN91_Basic Net0(size);
		float startVal = Net0.evaluateNN(theBoard, redPlayerTurn);
		float trash = 0;

		auto NN91Start = std::chrono::system_clock::now();
		for(uint i=0; i<numTiming; ++i)
		{
			trash += Net0.evaluateNN(theBoard, redPlayerTurn);
		}
		auto NN91End = std::chrono::system_clock::now();

		float endVal = Net0.evaluateNN(theBoard, redPlayerTurn);

		INFO("If different, evaluateNN somehow modfies the edges.");
		REQUIRE(fabs(endVal - startVal) < ERROR);

		std::chrono::duration<double> elapsedNN91Timer = NN91End - NN91Start;
	    std::cout << "NN91 timing: " << elapsedNN91Timer.count() << "s total, or " << elapsedNN91Timer.count() /numTiming << "s/run" << " for " << numTiming << " runs" << " trash: " << trash << std::endl;
	}
	SECTION("Minimax Alpha-Beta timing")
	{
		uint numTiming = 10000;
		std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
		NeuralNet net(NET_SIZE);

		for(uint depth = 4; depth <= 10; depth+=2) {
			auto minimaxABTimerStart = std::chrono::system_clock::now();
			for(uint i = 0; i < numTiming; ++i)
			{
				MinimaxWithAlphaBeta(theBoard, depth, redPlayerTurn, &net);
			}
			auto minimaxABTimerEnd = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsedMinimaxABTimer = minimaxABTimerEnd - minimaxABTimerStart;
			std::cout << "Minimax Alpha-Beta constructor timing, depth " << depth << ": " << elapsedMinimaxABTimer.count() << "s total, or " << elapsedMinimaxABTimer.count() / numTiming << "s/run"
					<< " for " << numTiming << " runs" << std::endl;

			numTiming /= 10;
		}
		
	}


	
}