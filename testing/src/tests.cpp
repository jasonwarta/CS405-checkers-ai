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
    Score score = {0, 0, 0};

    // std::cout << sore << std::endl;
    score.assignScore('D', true);
    Score expectedScore {0, 0, 0};
    REQUIRE(expectedScore == score);

    // const int NUM = 5;
    // random_device rndDev;
    // mt19937 randomNum(rndDev());
    // uniform_real_distribution<> randomLen(0, 842);
    // std::vector<double> lenCollection;
    // for (int i = 0; i < NUM; i++)
    // {
    //     lenCollection.push_back(randomLen(rndDev));
    // }
    // //Testing Triangles
    // for (auto v : lenCollection)
    // {
    //     REQUIRE(testCalcX(1, 3, v) == calcX(1, 3, v));
    //     REQUIRE(testCalcX(2, 3, v) == calcX(2, 3, v));
    //     REQUIRE(testCalcY(1, 3, v) == calcY(1, 3, v));
    //     REQUIRE(testCalcY(2, 3, v) == calcY(2, 3, v));
    // }
    // //Testing Squares
    // for (auto v : lenCollection)
    // {
    //     REQUIRE(testCalcX(1, 4, v) == calcX(1, 4, v));
    //     REQUIRE(testCalcX(2, 4, v) == calcX(2, 4, v));
    //     REQUIRE(testCalcX(3, 4, v) == calcX(3, 4, v));
    //     REQUIRE(testCalcY(1, 4, v) == calcY(1, 4, v));
    //     REQUIRE(testCalcY(2, 4, v) == calcY(2, 4, v));
    //     REQUIRE(testCalcY(3, 4, v) == calcY(3, 4, v));
    // }

    // //Testing Pentagon
    // for (auto v : lenCollection)
    // {
    //     REQUIRE(testCalcX(1, 5, v) == calcX(1, 5, v));
    //     REQUIRE(testCalcX(2, 5, v) == calcX(2, 5, v));
    //     REQUIRE(testCalcX(3, 5, v) == calcX(3, 5, v));
    //     REQUIRE(testCalcX(4, 5, v) == calcX(4, 5, v));
    //     REQUIRE(testCalcY(1, 5, v) == calcY(1, 5, v));
    //     REQUIRE(testCalcY(2, 5, v) == calcY(2, 5, v));
    //     REQUIRE(testCalcY(3, 5, v) == calcY(3, 5, v));
    //     REQUIRE(testCalcY(4, 5, v) == calcY(4, 5, v));
    // }
}