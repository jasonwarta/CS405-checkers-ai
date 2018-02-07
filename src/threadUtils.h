#ifndef THREAD_UTILS_INCLUDED
#define THREAD_UTILS_INCLUDED 

#include <string>
#include <mutex>
#include <iostream>

#include "consts.h"

struct Communicator {
	std::string currentBoard;
	std::mutex * mtx;

	void setBoard(std::string board);
	std::string getBoard();
};

#endif // THREAD_UTILS_INCLUDED