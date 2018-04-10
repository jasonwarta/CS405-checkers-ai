/*
    Cameron Showalter
    CS 405 && CS 441
    2-22-2018
    Version: alot
*/

#define ALIGNOCATOR_PRINT_TEST 0


#include "BasicNN.h"
#include "NN91_Basic.h"

#include <vector>
#include <chrono>
#include <iostream>

int main() 
{

    // config options:

    std::vector<int> networkSizeBasic {32, 40, 16, 1};

    // NN91 class automatically adds input and output nodes. 
    // Also multiplies all your numbers by 8
    std::vector<int> networkSizeNN91 {8, 7, 6}; // actually is {91, 64, 56, 48, 1}
    const std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    const int NumBoards = 500000;
    bool isRedTeam = true;

    // Different boards:
        BasicNN basicNet(networkSizeBasic);
        NN91_Basic NN91_BasicNet(networkSizeNN91);



    // **************************** Timing Math ****************************
    // basic 32 network, no SIMD
    auto evalBasicNetStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        basicNet.evaluateNN(board0, isRedTeam);
    }
    auto evalBasicNetEnd = std::chrono::system_clock::now();

    // basic 32 network, unaligned SIMD
    auto evalSIMDNetStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        basicNet.SIMDevaluateNN(board0, isRedTeam);
        // basicNet.evaluateNN(board0);
    }
    auto evalSIMDNetEnd = std::chrono::system_clock::now();

    // basic 32 network, aligned SIMD
    auto evalAlignedBasicStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        basicNet.AlignedEvaluateNN(board0, isRedTeam);
    }
    auto evalAlignedBasicEnd = std::chrono::system_clock::now();

    // NN91, no SIMD
    auto eval91BasicStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        NN91_BasicNet.evaluateNN(board0, isRedTeam);
    }
    auto eval91BasicEnd = std::chrono::system_clock::now();

    // NN91, aligned SIMD
    auto eval91SIMDStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        NN91_BasicNet.SIMDevaluateNN(board0, isRedTeam);
    }
    auto eval91SIMDEnd = std::chrono::system_clock::now();



    // **************************** Timing Math Results ****************************
    std::cout << "TIMES: " << std::endl;

    std::chrono::duration<double> elapsed_seconds_1 = evalBasicNetEnd - evalBasicNetStart;
    std::cout << "Time of BasicNN: " << elapsed_seconds_1.count() << "s total, or " << elapsed_seconds_1.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_2 = evalSIMDNetEnd - evalSIMDNetStart;
    std::cout << "Time of 32 SIMD-NN: " << elapsed_seconds_2.count() << "s total, or " << elapsed_seconds_2.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_3 = evalAlignedBasicEnd - evalAlignedBasicStart;
    std::cout << "Time of 32 Aligned SIMD-NN: " << elapsed_seconds_3.count() << "s total, or " << elapsed_seconds_3.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_4 = eval91BasicEnd - eval91BasicStart;
    std::cout << "Time of NN91: " << elapsed_seconds_4.count() << "s total, or " << elapsed_seconds_4.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_5 = eval91SIMDEnd - eval91SIMDStart;
    std::cout << "Time of NN91: " << elapsed_seconds_5.count() << "s total, or " << elapsed_seconds_5.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;
    std::cout << std::endl << std::endl;

    // **************************** Correctness Tests ****************************
    // (Not the best testing method in the world, but better than nothing)
    const std::string board1 = "rbrbRBRB__rb_BRRRR___RRrrr______";
    const std::string board2 = "________________________________";
    const std::string board3 = "RRRR________________________BBBB";



    BasicNN basicNet1 = basicNet;
    BasicNN basicNet2 = basicNet;

    basicNet.evaluateNN(board1, isRedTeam);
    basicNet1.evaluateNN(board1, isRedTeam);
    basicNet2.AlignedEvaluateNN(board1, isRedTeam);

    std::cout << "tempMain.cpp - test 1 - Should be almost the same: " << std::endl;
    std::cout << basicNet.getLastNode() << std::endl;
    std::cout << basicNet1.getLastNode() << std::endl;
    std::cout << basicNet2.getLastNode() << std::endl;
    std::cout << std::endl;


    basicNet.evaluateNN(board2, isRedTeam);
    basicNet1.evaluateNN(board2, isRedTeam);
    basicNet2.AlignedEvaluateNN(board2, isRedTeam);

    std::cout << "tempMain.cpp - test 2 - Should be almost the same: " << std::endl;
    std::cout << basicNet.getLastNode() << std::endl;
    std::cout << basicNet1.getLastNode() << std::endl;
    std::cout << basicNet2.getLastNode() << std::endl;
    std::cout << std::endl;


    basicNet.evaluateNN(board3, isRedTeam);
    basicNet1.evaluateNN(board3, isRedTeam);
    basicNet2.AlignedEvaluateNN(board3, isRedTeam);

    std::cout << "tempMain.cpp - test 3 - Should be almost the same: " << std::endl;
    std::cout << basicNet.getLastNode() << std::endl;
    std::cout << basicNet1.getLastNode() << std::endl;
    std::cout << basicNet2.getLastNode() << std::endl;
    std::cout << std::endl;



    NN91_Basic NN91_BasicNet1 = NN91_BasicNet;

    NN91_BasicNet.evaluateNN(board1, isRedTeam);
    NN91_BasicNet1.SIMDevaluateNN(board1, isRedTeam);

    std::cout << "tempMain.cpp - test 4 - Should be almost the same: " << std::endl;
    std::cout << NN91_BasicNet.getLastNode() << std::endl;
    std::cout << NN91_BasicNet1.getLastNode() << std::endl;
    std::cout << std::endl;

    NN91_BasicNet.evaluateNN(board2, isRedTeam);
    NN91_BasicNet1.SIMDevaluateNN(board2, isRedTeam);

    std::cout << "tempMain.cpp - test 5 - Should be almost the same: " << std::endl;
    std::cout << NN91_BasicNet.getLastNode() << std::endl;
    std::cout << NN91_BasicNet1.getLastNode() << std::endl;
    std::cout << std::endl << std::endl;

#if ALIGNOCATOR_PRINT_TEST

    // **************************** Alignocator ****************************
    // Never ended up using this class, but I figured I'd include it. I may end up
    // improving it later
    
    std::cout << "Alignocator Test" << std::endl;
    const int ARRAY_SIZE = 6;
    Aligninator<float, 32> firstTest(ARRAY_SIZE);
    firstTest[0] = 5.0;
    firstTest[1] = 4.6;
    firstTest[3] = 7;

    for(int i=0; i<ARRAY_SIZE; ++i)
    {
        std::cout << i << ": " << firstTest[i] << std::endl;
    }
    std::cout << "Begin: " << firstTest.begin() << " end: " << firstTest.end() << std::endl;
#endif


    return 0;
}

/*

..00..01..01..03
04..05..06..07..
..08..09..10..11
12..13..14..15..
..16..17..18..19
20..21..22..23..
..24..25..26..27
28..29..30..31..

*/