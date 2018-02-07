#include <uWS/uWS.h>

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <string>
#include <mutex>

#include "server.h"
#include "threadUtils.h"

void launchServer(Communicator * comm) {
	uWS::Hub h;
	createServerInstance(h,(*comm));
	h.run();
}

// Linux sleep command takes microseconds, most other things think in milliseconds
int milli_to_micro(int milliseconds) {
	return 1000*milliseconds;
}

int main(int argc, char const *argv[]) {
	std::cout << argc << std::endl;

	std::mutex mtx;
	Communicator comm = {START_BOARD_STRING,&mtx};

	// launch server in separate thread
	// server currently has no logic for terminating
	std::thread serverThread(launchServer,&comm);



	// main loop, only exit when done
	while (true) {
		std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
		std::cout << "in main thread" << endl;
		std::cout << comm.getBoard() << endl;
	}


	std::cout << "Time to Die" << endl;
	exit(0);
}