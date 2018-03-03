/*
    Cameron Showalter
    CS 405 && CS 441
    2-22-2018
    Version: alot
*/


#include "BasicNN.h"


#include <vector>
#include <chrono>
#include <iostream>
#include <string>


int main() 
{
	std::string board0 = "rrrrrrrrrrrr________bbbbbbbbbbbb";
    std::vector<int> startBoard{3, 4, 2, 1};

	BasicNN tempNet(startBoard);
	//tempNet.printAll();
	tempNet.evaluateNN(board0);
	tempNet.printAll();
	tempNet.evolve();
	tempNet.evaluateNN(board0);
	//tempNet.printAll();

    return 0;
}
