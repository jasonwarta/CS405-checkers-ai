/*
    Cameron Showalter
    CS 405 && CS 441
    2-22-2018
    Version: alot
*/



#define BASIC_NN 0
#define BASIC_NN_SIMD 1
#define NN91_BASIC_NET 0
#define NN91_SIMD_NET 0

#if BASIC_NN
    #include "BasicNN.h"
#endif
#if BASIC_NN_SIMD
    #include "BasicNN_SIMD.h"
#endif
#if NN91_BASIC_NET
    #include "NN91_Basic.h"
#endif
#if NN91_SIMD_NET
    #include "NN91_SIMD"
#endif


#include <vector>
#include <chrono>
#include <iostream>

int main() 
{

    // config options:
    std::vector<int> networkSize{32, 40, 10, 1};
    const std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    const int NumBoards = 2000;


    // Different boards:
    #if BASIC_NN
        BasicNN basicNet(networkSize);
        basicNet.randomizeWeights();
    #endif
    #if BASIC_NN_SIMD
        BasicNN_SIMD basicNetSIMD(networkSize);
        basicNetSIMD.randomizeWeights();
    #endif
    #if NN91_BASIC_NET
        NN91_Basic NN91_BasicNet(networkSize);
        // NN91_BasicNet.randomizeWeights();
    #endif
    #if NN91_SIMD_NET
        NN91_SIMD NN91_SIMDnetwork(networkSize);
        NN91_SIMDnetwork.randomizeWeights();
    #endif


    // Timing Math:
    #if BASIC_NN
        auto evalBasicNetStart = std::chrono::system_clock::now();
        for(int i=0; i<NumBoards; ++i)
        {
            basicNet.evaluateNN(board0);
        }
        auto evalBasicNetEnd = std::chrono::system_clock::now();
    #endif

    #if BASIC_NN_SIMD
        auto evalBasicSIMDstart = std::chrono::system_clock::now();
        for(int i=0; i<NumBoards; ++i)
        {
            basicNetSIMD.evaluateNN(board0);
        }
        auto evalBasicSIMDend = std::chrono::system_clock::now();
    #endif

    #if NN91_BASIC_NET
        auto eval91BasicStart = std::chrono::system_clock::now();
        for(int i=0; i<NumBoards; ++i)
        {
            NN91_BasicNet.evaluateNN(board0);
        }
        auto eval91BasicEnd = std::chrono::system_clock::now();
    #endif

    #if NN91_SIMD_NET
        auto eval91SIMDstart = std::chrono::system_clock::now();
        for(int i=0; i<NumBoards; ++i)
        {
            NN91_SIMDnetwork.evaluateNN(board0);
        }
        auto eval91SIMDend = std::chrono::system_clock::now();
    #endif



    std::cout << "TIMES: " << std::endl;

    #if BASIC_NN
        std::chrono::duration<double> elapsed_seconds_1 = evalBasicNetEnd - evalBasicNetStart;
        std::cout << "Time of BasicNN: " << elapsed_seconds_1.count() << "s total, or " << elapsed_seconds_1.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;
    #endif
    #if BASIC_NN_SIMD
        std::chrono::duration<double> elapsed_seconds_2 = evalBasicSIMDend - evalBasicSIMDstart;
        std::cout << "Time of BasicNN with SIMD: " << elapsed_seconds_2.count() << "s total, or " << elapsed_seconds_2.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;
    #endif
    #if NN91_BASIC_NET
        std::chrono::duration<double> elapsed_seconds_3 = eval91BasicEnd - eval91BasicStart;
        std::cout << "Time of NN91: " << elapsed_seconds_3.count() << "s total, or " << elapsed_seconds_3.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;
    #endif
    #if NN91_SIMD_NET
        std::chrono::duration<double> elapsed_seconds_4 = eval91SIMDend - eval91SIMDstartt;
        std::cout << "Time of NN91 with SIMD: " << elapsed_seconds_4.count() << "s total, or " << elapsed_seconds_4.count() / NumBoards << "s/board" << " for " << NumBoards << " boards" << std::endl;
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