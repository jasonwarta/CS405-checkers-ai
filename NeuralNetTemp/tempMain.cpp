


#include "NeuralNet.h"
#include "BasicBoardEval.h"
#include "MinMaxTree.h"
#include "../src/checkers.h"
#include <vector>

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

    // MinMaxTree test    12345678901234567890123456789012
    std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    MinMaxTree searchTree(board0, 4, true);
    std::cout << "StartBoard Eval: " << searchTree.getBestBoard() << std::endl;

    return 0;
}
