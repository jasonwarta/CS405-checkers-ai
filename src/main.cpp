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
#include <experimental/filesystem>
#include <stdio.h>

#include "threadUtils.h"
#include "Player.h"
#include "Game.h"
#include "consts.h"
#include "defs.h"
#include "MatchHandling.h"
#include "testRun.h"
#include "json.hpp"
#include "playOnNet.h"

namespace fs = std::experimental::filesystem;

int main(int argc, char const *argv[]) {
	std::random_device rd;
	std::mt19937 random(rd());
	std::default_random_engine generator(rd());

	// std::uniform_real_distribution<> changeKingVal(-0.1, 0.1);
	// for(int i = 0; i < 20; i++){
	// 	std::cout << changeKingVal(random) << std::endl;
	// }

	std::mutex mtx;
	std::ofstream ofs;
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	
	std::vector<std::shared_ptr<NetTracker>> nets;

	if(argc > 1) {
		if (std::string("-nets").compare(argv[1]) == 0) {
			// use `./main -nets path/to/directory/of/nets`
			std::cout << "-nets" << " " << argv[2] << std::endl;
			std::ifstream ifs;
			for(auto& f: fs::directory_iterator(argv[2])) {
				ifs.open(f.path());
				nets.push_back(std::make_shared<NetTracker>(NetTracker {new std::mutex, new NeuralNet(ifs)}));
				ifs.close();
			}
		}
		else if (std::string("-test").compare(argv[1]) == 0) {
			// use `./main -test path/to/network_file`
			std::cout << "-test" << " " << argv[2] << std::endl;
			std::ifstream ifs;
			ifs.open(argv[2]);
			NeuralNet * net = new NeuralNet(ifs);
			ifs.close();

			net->printData();

			Player p1(true, clock, net);
			Player p2(false, clock, nullptr, true);

			testRun(&p1, &p2);

			exit(0);
		}
		else if (std::string("-play").compare(argv[1]) == 0) {
			// use `./main -play path/to/network_file serverAddress gameName color`
			// color can be any one of [R,r,red,B,b,black]
			// example call, from the project root:
			// ./build/main -play NN_185gens/gen_185/nets/00 127.0.0.1:8080 test1 B
			std::cout << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << std::endl;

			// load net
			std::ifstream ifs;
			ifs.open(argv[2]);
			NeuralNet * net = new NeuralNet(ifs);
			ifs.close();

			bool redTeam;
			std::cout << argv[5] << std::endl;
			if (std::string(argv[5]) == "R" || std::string(argv[5]) == "r" || std::string(argv[5]) == "red")
				redTeam = true;
			else if (std::string(argv[5]) == "B" || std::string(argv[5]) == "b" || std::string(argv[5]) == "black")
				redTeam = false;
			else {
				std::cout << "Invalid player color" << std::endl;
				return 0;
			}


			std::cout << "red team: " << redTeam << std::endl;

			NetworkGame ng(net, argv[3], argv[4], redTeam);
			ng.playGame();
			return 0;
		}	
	} else {
		for(size_t i = 0; i < POPULATION_SIZE; ++i)
			nets.push_back(std::make_shared<NetTracker>(NetTracker {new std::mutex, new NeuralNet(NET_SIZE)}));
	}

	std::random_device r;
	std::mt19937 rand(r());
	std::uniform_int_distribution<> randIndex(0,POPULATION_SIZE-1);

	uint64_t genCounter = 0;

	while(true) {
		std::queue<std::unique_ptr<Match>> matchesQueue;

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
			for(size_t j = 0; j < MIN_MATCHES; ++j) {
				if (nets[i]->opponents.size() >= MAX_MATCHES)
					break;

				int opponentIdx = getRandomIndex(i, rand, randIndex, nets);

				nets[i]->opponents.push_back(opponentIdx);
				nets[opponentIdx]->opponents.push_back(i);

				ss.str("");
				ss << path << "/games/" << std::setfill('0') << std::setw(2) << i << "v" <<std::setfill('0') << std::setw(2) << opponentIdx;
				
				matchesQueue.push(std::make_unique<Match>(Match {nets[i], nets[opponentIdx], startBoard, ss.str()}));
				ss.str("");
			}
		}

		std::cout << matchesQueue.size() << std::endl;
		for(size_t i = 0; i < nets.size(); ++i) {
			std::cout << std::setfill('0') << std::setw(2) << i << ": ";
			for(size_t j = 0; j < nets[i]->opponents.size(); ++j)
				std::cout << std::setfill('0') << std::setw(2) << (int)nets[i]->opponents[j] << " ";
			std::cout << std::endl;
		}

		std::vector<std::thread> threads;
		for (int i = 0; i < NUM_THREADS; ++i)
		{
			threads.push_back( std::thread(play, std::ref(mtx), std::ref(matchesQueue)) );
		}
		
		for (int i = 0; i < threads.size(); ++i)
		{
			threads[i].join();
		}

		ss.str("");
		ss << path << "/scores";
		ofs.open(ss.str(), std::ofstream::out);
		for(size_t i = 0; i < nets.size(); ++i)
			ofs << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;
		ofs.close();
		ss.str("");

		std::sort(nets.begin(), nets.end(), [](std::shared_ptr<NetTracker> a, std::shared_ptr<NetTracker> b) {
			return ((float)a->score/a->opponents.size()) > ((float)b->score/b->opponents.size());
		});

		for(size_t i = 0; i < nets.size(); ++i)
			std::cout << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;

		for(size_t i = 0; i < (POPULATION_SIZE/2); ++i) 
			(*nets[(POPULATION_SIZE/2)+i]->net) = (*nets[i]->net);
		
		for(size_t i = (POPULATION_SIZE/2); i < POPULATION_SIZE; ++i)
			nets[i]->net->evolve();
		
		for(auto &nt : nets) {
			nt->score = 0;
			nt->opponents.clear();
		}

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

// given two start/mid/end game NN combos, 'abc' and 'xyz',
// the following combination of nets would assure that every
// possible combination of nets is tested
// 
// abc, abz, ayc, ayz
// xbc, xbz, xyc, xyz