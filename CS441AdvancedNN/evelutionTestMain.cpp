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
#include <string>


int main() 
{
	std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    std::vector<int> startBoard{32, 40, 10, 1};



	BasicNN tempNetOne(startBoard);
    BasicNN tempNetTwo = tempNetOne;
	// tempNet.printAll();
	tempNetOne.evaluateNN(board0);
    tempNetTwo.SIMDevaluateNN(board0);

    std::cout << std::endl << "BOARD ONE" << std::endl;
	tempNetOne.printAll();
    std::cout << std::endl << "BOARD TWO" << std::endl;
    tempNetTwo.printAll();
	// tempNet.evolve();
	// tempNet.evaluateNN(board0);
	//tempNet.printAll();

    return 0;
}
