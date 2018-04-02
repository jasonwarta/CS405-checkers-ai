
#include "src/BasicNN.h"
#include "src/NN91_Basic.h"

#include <vector>
#include <string>

int main()
{
	std::vector<int> v {3, 10, 1};
	std::string board = "b_r";
	bool isRedTeam = true;

	BasicNN basicTest(v);
	basicTest.evaluateNN(board, isRedTeam);
	// std::cout << "Eval 1: " << basicTest.getLastNode() << std::end;;
	basicTest.printAll();

	basicTest.evolve();
	basicTest.printAll();
	return 0;
}