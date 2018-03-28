// #include <uWS/uWS.h>

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

#include "threadUtils.h"
#include "Player.h"
#include "Game.h"
#include "consts.h"
#include "defs.h"

const int8_t LOSS_VAL = -1;
const int8_t DRAW_VAL = 0;
const int8_t WIN_VAL = 2;

const std::vector<int> NET_SIZE {32,5,4,3};

struct NetTracker {
	NeuralNet* net;
	int64_t score = 0;
};

struct Score {
	std::mutex * mtx;

	int8_t draw = 0;
	int8_t p1 = 0;
	int8_t p2 = 0;

	// given Players 1 and 2, the games should be played as following:
	// game 1: 1v2. call with toggle = true
	// game 2: 2v1. call with toggle = false
	void assignScore(char result, bool toggle) {
		std::lock_guard<std::mutex> guard(*(this->mtx));
		std::cout << (toggle ? "1v2" : "2v1") << ": " << result << std::endl;
		switch(result) {
			case 'R':
				if (toggle)
					p1++;
				else 
					p2++;
				break;
			case 'B':
				if (toggle)
					p2++;
				else
					p1++;
				break;
			case 'D':
				draw++;
				break;
			default:
				break;
		}
	}
};

void playGame(Player * p1, Player * p2, std::string startBoard, Score * score, bool toggle, std::ostream * os) 
{
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	Game game( p1, p2, clock, os );
	score->assignScore(game.run(startBoard), toggle);
}


int main(int argc, char const *argv[]) {
	
	std::ofstream ofs;
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	
	std::vector<NetTracker*> nets;

	for(size_t i = 0; i < 30; ++i)
		nets.push_back(new NetTracker {new NeuralNet(NET_SIZE), 0});

	uint64_t genCounter = 0;

	while(true) {
		ss.str("");
		ss << "./NN/gen_" << std::setfill('0') << std::setw(3) << genCounter;
		std::string path = ss.str();
		if ( system(("mkdir -p "+path+"/nets").c_str()) != 0 || 
			 system(("mkdir -p "+path+"/games").c_str()) != 0 ) 
		{
			std::cout << "error creating directories for neural net logging" << std::endl;
			return 0;
		}
		ss.str("");

		for(size_t i = 0; i < nets.size(); ++i) {
			ss.str("");
			ss << path << "/nets/" << std::setfill('0') << std::setw(2) << i;
			ofs.open(ss.str(), std::ofstream::out);
			nets[i]->net->printData(&ofs);
			ofs.close();
			ss.str("");
		}

		std::string startBoard = getRandomStartBoard();

		for(size_t i = 0; i < nets.size(); ++i) {
			for(size_t j = i+1; j < nets.size(); ++j) {
				ss.str("");
				ss << path << "/games/" << std::setfill('0') << std::setw(2) << i << "v" <<std::setfill('0') << std::setw(2) << j;
				ofs.open(ss.str(), std::ofstream::out);
				
				std::cout << ss.str() << std::endl;
				ss.str("");

				// duplicate nets to avoid rewriting everything in a threadsafe style
				// NeuralNet * p1NetCopy = new NeuralNet(NET_SIZE);
				// (*p1NetCopy) = (*nets[i]->net);

				// NeuralNet * p2NetCopy = new NeuralNet(NET_SIZE);
				// (*p2NetCopy) = (*nets[j]->net);


				Player p1(true, clock, nets[i]->net);
				// Player p1copy(true, clock, p1NetCopy);

				Player p2(false, clock, nets[j]->net);
				// Player p2copy(false, clock, p2NetCopy);


				std::mutex mtx;
				Score score = {&mtx, 0, 0, 0};

				// std::stringstream game1ss;
				// std::stringstream game2ss;
				
				playGame(&p1, &p2, startBoard, &score, true, &ofs);
				playGame(&p2, &p1, startBoard, &score, false, &ofs);

				// std::thread game1(playGame, &p1, &p2, startBoard, &score, true, &game1ss );
				// std::thread game2(playGame, &p2copy, &p1copy, startBoard, &score, false, &game2ss );

				// game1.join();
				// game2.join();
				// std::cout << "threads have joined" << std::endl;

				// ofs << game1ss.str() << "\n\n" << game2ss.str() << "\n\n";
				ofs << "\n\n";

				if( score.p1 == score.p2 ) {
					std::cout << "playing game 3" << std::endl;
					Game game3(&p1, &p2, clock, &ofs);
					score.assignScore(game3.run(), true);
					ofs << "\n\n";
				}

				ofs.close();

				nets[i]->score += (score.p1*WIN_VAL) + (score.p2*LOSS_VAL) + (score.draw*DRAW_VAL);
				nets[j]->score += (score.p2*WIN_VAL) + (score.p1*LOSS_VAL) + (score.draw*DRAW_VAL);

				std::cout << i << ":" << nets[i]->score << " " << j << ":" << nets[j]->score << std::endl;
			}
		}

		ss.str("");
		ss << path << "/scores";
		ofs.open(ss.str(), std::ofstream::out);
		for(size_t i = 0; i < nets.size(); ++i)
			ofs << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;
		ofs.close();
		ss.str("");

		std::sort(nets.begin(), nets.end(), [](NetTracker* a, NetTracker* b) {
			return a->score > b->score;
		});

		for(size_t i = 0; i < nets.size(); ++i)
			std::cout << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;

		for(size_t i = 0; i < 15; ++i) 
			(*nets[15+i]->net) = (*nets[i]->net);

		for(size_t i = 15; i < 30; ++i)
			nets[i]->net->evolve();

		for(NetTracker* nt : nets)
			nt->score = 0;

		genCounter++;
	}

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