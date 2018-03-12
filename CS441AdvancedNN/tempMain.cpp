/*
    Cameron Showalter
    CS 405 && CS 441
    2-22-2018
    Version: alot
*/




#include "BasicNN.h"
#include "NN91_Basic.h"


#include <vector>
#include <chrono>
#include <iostream>

int main() 
{

    // config options:
    std::vector<int> networkSize{32, 40, 10, 1};
    const std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    const int NumBoards = 100000;


    // Different boards:
        BasicNN basicNet(networkSize);
        NN91_Basic NN91_BasicNet(networkSize);

        BasicNN basicNetCopy = basicNet;
        NN91_Basic NN91_BasicNetCopy = NN91_BasicNet;



    // Timing Math:

    auto evalBasicNetStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        basicNet.evaluateNN(board0);
    }
    auto evalBasicNetEnd = std::chrono::system_clock::now();

    auto evalSIMDNetStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        basicNet.SIMDevaluateNN(board0);
    }
    auto evalSIMDNetEnd = std::chrono::system_clock::now();


    auto eval91BasicStart = std::chrono::system_clock::now();
    for(int i=0; i<NumBoards; ++i)
    {
        NN91_BasicNet.evaluateNN(board0);
    }
    auto eval91BasicEnd = std::chrono::system_clock::now();




    std::cout << "TIMES: " << std::endl;

    std::chrono::duration<double> elapsed_seconds_1 = evalBasicNetEnd - evalBasicNetStart;
    std::cout << "Time of BasicNN: " << elapsed_seconds_1.count() << "s total, or " << elapsed_seconds_1.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_2 = evalSIMDNetEnd - evalSIMDNetStart;
    std::cout << "Time of 32 SIMD-NN: " << elapsed_seconds_2.count() << "s total, or " << elapsed_seconds_2.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::chrono::duration<double> elapsed_seconds_3 = eval91BasicEnd - eval91BasicStart;
    std::cout << "Time of NN91: " << elapsed_seconds_3.count() << "s total, or " << elapsed_seconds_3.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;

    std::cout << "Time of NN91-SIMD: COMMING SOON(ish)" << std::endl << std::endl;

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