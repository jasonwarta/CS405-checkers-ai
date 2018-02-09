#include "threadUtils.h"

void Communicator::setBoard(std::string board) {
	std::lock_guard<std::mutex> guard(*(this->mtx));
	this->currentBoard = board;
}

std::string Communicator::getBoard() {
	std::lock_guard<std::mutex> guard(*(this->mtx));
	return this->currentBoard;
}