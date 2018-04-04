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

#include "threadUtils.h"
#include "Player.h"
#include "Game.h"
#include "consts.h"
#include "defs.h"
#include "MatchHandling.h"

namespace fs = std::experimental::filesystem;

int main(int argc, char const *argv[]) {
	std::ofstream ofs;
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	
	std::vector<std::shared_ptr<NetTracker>> nets;

	if(argc > 1) {
		std::ifstream ifs;
	    for(auto& f: fs::directory_iterator(argv[1])) {
			ifs.open(f.path());
			nets.push_back(std::make_shared<NetTracker>(NetTracker {new std::mutex, new NeuralNet(ifs)}));
			ifs.close();
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
		// std::cout << "in while loop" << std::endl;
		std::mutex mtx;
		std::queue<std::unique_ptr<Match>> matchesQueue;
		// Matches matches {&mtx};

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
		// std::cout << "made dirs" << std::endl;
		for(size_t i = 0; i < nets.size(); ++i) {
			ss.str("");
			ss << path << "/nets/" << std::setfill('0') << std::setw(2) << i;
			ofs.open(ss.str(), std::ofstream::out);
			nets[i]->net->printData(&ofs);
			ofs.close();
			ss.str("");
		}
		// std::cout << "printed stats" << std::endl;
		std::string startBoard = getRandomStartBoard();

		// for(size_t i = 0; i < nets.size(); ++i) {
		// 	for(size_t j = i+1; j < nets.size(); ++j) {
		// 		ss.str("");
		// 		ss << path << "/games/" << std::setfill('0') << std::setw(2) << i << "v" <<std::setfill('0') << std::setw(2) << j;

		// 		matchesQueue.push(std::make_unique<Match>(Match {nets[i], nets[j], startBoard, ss.str()}));
		// 		ss.str("");
		// 	}
		// }

		for(size_t i = 0; i < nets.size(); ++i) {
			for(size_t j = 0; j < MIN_MATCHES; ++j) {
				if (nets[i]->opponents.size() >= MAX_MATCHES)
					break;

				int opponentIdx = getRandomIndex(i, rand, randIndex, nets);
				// if (opponentIdx == -1)
				// 	break;

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

		// std::cout << "set up queue" << std::endl;

		// play(mtx,matchesQueue);
		std::vector<std::thread> threads;
		for (int i = 0; i < NUM_THREADS; ++i)
		{
			threads.push_back( std::thread(play, std::ref(mtx), std::ref(matchesQueue)) );
		}
		
		for (int i = 0; i < threads.size(); ++i)
		{
			threads[i].join();
		}

		// std::thread t1(play, std::ref(mtx), std::ref(matchesQueue));
		// t1.join();

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
		std::cout << "point 1" << std::endl;
		for(size_t i = (POPULATION_SIZE/2); i < POPULATION_SIZE; ++i)
			nets[i]->net->evolve();
		std::cout << "point 2" << std::endl;
		for(auto &nt : nets) {
			nt->score = 0;
			nt->opponents.clear();
		}
		std::cout << "point 3" << std::endl;

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