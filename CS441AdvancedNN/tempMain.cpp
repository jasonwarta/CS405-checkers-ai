


#include "BasicNN.h"
#include "BasicNN_Ref.h"
#include "NN91_Basic.h"

#include "../src/BasicBoardEval.h"
#include "../src/MinMaxTree.h"
#include "../src/checkers.h"
#include "../src/consts.h"

#include <vector>
#include <chrono>


int main() {
    /*
    // NN test
    std::vector<int> y {1000,1000,10001,420,420,420,1};
    NeuralNet myNodes(y);
    std::cout << "LastNode of NN: " << myNodes.getLastNode() << std::endl;
    */

    /*
    // BasicBoardEval test
	std::string board0 = "__________brrrr_________________";
	std::string board1 = "Br____________________________bR";
	std::string board2 = "________bb______________________";
	std::string board3 = "________rr______________________";
	bool redTeamTurn = true;
    std::cout << "BoardEval: " << basicBoardEval(board0, redTeamTurn) << std::endl;
    std::cout << "BoardEval: " << basicBoardEval(board1, redTeamTurn) << std::endl;
    std::cout << "BoardEval: " << basicBoardEval(board2, redTeamTurn) << std::endl;
    std::cout << "BoardEval: " << basicBoardEval(board3, redTeamTurn) << std::endl;
    */

    /*
    // MinMaxTree test    12345678901234567890123456789012
    std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    MinMaxTree searchTree(board0, 4, false);
    std::cout << "StartBoard Eval: " << searchTree.getBestBoard() << std::endl;
    */


    std::vector<int> networkSize{4, 32, 40, 10, 1};
    const std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    //NN91_Basic tempBoard(board0, networkSize);
    //std::cout << "Returned: " << tempBoard.getLastNode() << std::endl;

    
    vector<vector<float>> layers;
    vector<vector<float>> edges;

    BasicNN basicNet(networkSize);
    basicNet.randomizeWeights();
    //BasicNN_Ref basicNet(networkSize, layers, edges);

    auto startTime = std::chrono::system_clock::now();
    for(int i=0; i<100000; i++)
    {
        basicNet.evaluateNN();
        //basicNet.evaluateNN(layers, edges);
    }
    auto endTime = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_time = endTime - startTime;
    std::cout << "Elapsed Time: " << elapsed_time.count() << "s" << std::endl;
    



    /*
    int count =0;
    for(int i=0; i<NN91_NODE_LOCATIONS.size(); ++i)
    {
        for(int j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
        {
            count++;
        }
    }
    std::cout << "Count is: " << count << std::endl;
    */
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