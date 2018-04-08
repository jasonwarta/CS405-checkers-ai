#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <random>
#include <algorithm>
#include <mutex>

#include "../../src/defs.h"
#include "../../src/consts.h"
#include "../../src/MatchHandling.h"

//error in doubles
const double ERROR = 0.0000001;

TEST_CASE("Testing Score Struct", "[MatchHandling]")
{
    Score score;

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

TEST_CASE("Testing NetTracker struct","[MatchHandling]")
{
    NeuralNet nn(NET_SIZE);
    std::mutex mtx;
    NetTracker nt;
    Score score;

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