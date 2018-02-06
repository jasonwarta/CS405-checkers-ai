#include <uWS/uWS.h>

#include <iostream>

#include "server.h"

int main(int argc, char const *argv[]) {
	std::cout << argc << std::endl;

	// Set up and start server
	uWS::Hub h;
	createServerInstance(h);
	h.run();
}