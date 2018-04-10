#include <uWS/uWS.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <string>
#include <mutex>
#include <iomanip>
#include <memory>
#include <fstream>
#include <sstream>

#include "Player.h"
#include "Game.h"

#include "server.h"
#include "defs.h"

int main(int argc, char const *argv[]) {
	if(argc != 2) {
		std::cout << "you must pass a filename for the net to load" << std::endl;
		return 0;
	}

	std::ifstream ifs;
	ifs.open(argv[1]);
	NeuralNet * ai = new NeuralNet(ifs);
	ifs.close();

	uWS::Hub h;
	createServerInstance(h,ai);
	h.run();

	return 0;
}