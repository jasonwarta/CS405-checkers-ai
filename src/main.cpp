// #include <uWS/uWS.h>

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <chrono>
#include <string>
#include <mutex>
#include <iomanip>
#include <memory>

#include "threadUtils.h"
#include "Player.h"
#include "Game.h"

// void launchServer(Communicator * comm) {
// 	uWS::Hub h;
// 	createServerInstance(h,(*comm));
// 	h.run();
// }

// Linux sleep command takes microseconds, most other things think in milliseconds
int milli_to_micro(int milliseconds) {
	return 1000*milliseconds;
}

int main(int argc, char const *argv[]) {
	// std::cout << argc << std::endl;

	std::mutex mtx;
	// Communicator comm = {START_BOARD_STRING,&mtx};

	// launch server in separate thread
	// server currently has no logic for terminating
	// std::thread serverThread(launchServer,&comm);

	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	Player red( true, 2.0, clock );
	Player black( false, clock );

	Game game( &red, &black, clock, &std::cout );
	game.run();

	std::cout << "reached end of program" << std::endl;
	exit(0);
}


// NN genetics

// a(by)(cz)	abc, abz, ayc, ayz
// x(by)(cz)	xbc, xbz, xyc, xyz
// (ax)b(cz)	abc, abz, xbc, xbz
// (ax)y(cz)	ayc, ayz, xyc, xyz
// (ax)(by)c 	abc, ayc, xbc, xyc
// (ax)(by)z	abz, ayz, xbz, xyz

// abc, abz, ayc, ayz
// xbc, xbz, xyc, xyz