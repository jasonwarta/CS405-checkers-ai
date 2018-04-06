#ifndef MATCHHANDLING_H
#define MATCHHANDLING_H

#include <vector>
#include <mutex>
#include <random>

#include "consts.h"
#include "defs.h"
#include "Player.h"
#include "Game.h"

struct Score {
	int8_t draw = 0;
	int8_t p1 = 0;
	int8_t p2 = 0;

	// given Players 1 and 2, the games should be played as following:
	// game 1: 1v2. call with toggle = true
	// game 2: 2v1. call with toggle = false
	void assignScore(char result, bool toggle, std::ostream * os);
};

struct NetTracker {
	std::mutex * mtx;
	NeuralNet* net;
	int64_t score = 0;
	std::vector<uint8_t> opponents = {};

	void assignScore(Score * gameScore, bool p1);
};


struct Match {
	std::shared_ptr<NetTracker> lhs = nullptr;
	std::shared_ptr<NetTracker> rhs = nullptr;
	std::string startBoard = "";
	std::string fname = "";

	void playMatch();
	void operator=(Match &match);
};

std::string playGame(Player *p1, Player *p2, std::string startBoard, Score *score, bool toggle, std::ostream *os);

void play(std::mutex &mtx, std::queue<std::unique_ptr<Match>> &matches);

int getRandomIndex(int index, std::mt19937 &rand, std::uniform_int_distribution<> &randIndex, std::vector<std::shared_ptr<NetTracker>> &nets);

#endif