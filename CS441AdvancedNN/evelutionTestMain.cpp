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
    std::vector<int> startBoard{40,10};

    int counter = 0;
    for(int i=0; i<NN91_NODE_LOCATIONS.size(); ++i)
    {
    	for(int j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
    	{
    		counter++;
    	}
    }
    std::cout << "COUNTER IS: " << counter << std::endl;

	NN91_Basic tempNet(startBoard);
	// tempNet.printAll();
	tempNet.evaluateNN(board0);
	tempNet.printAll();
	// tempNet.evolve();
	// tempNet.evaluateNN(board0);
	//tempNet.printAll();

    return 0;
}
