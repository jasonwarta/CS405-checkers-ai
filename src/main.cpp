#include <uWS/uWS.h>

#include "server.h"
#include "board.h"

int main(int argc, char const *argv[]) {

	// Set up and start server
	uWS::Hub h;
	createServerInstance(h);
	h.run();


}