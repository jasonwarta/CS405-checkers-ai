#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <random>
#include <algorithm>

#include "../../src/MatchHandling.h"

//error in doubles
const double ERROR = 0.0000001;

TEST_CASE("Testing Score Struct", "[Match Handling]")
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