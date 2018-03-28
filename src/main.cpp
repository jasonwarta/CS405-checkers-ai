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
#include "MatchHandling.h"

int main(int argc, char const *argv[]) {
	
	std::ofstream ofs;
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	
	std::vector<NetTracker*> nets;

	for(size_t i = 0; i < POPULATION_SIZE; ++i)
		nets.push_back(new NetTracker {new std::mutex, new NeuralNet(NET_SIZE), 0});

	uint64_t genCounter = 0;

	

	while(true) {

		std::mutex mtx;
		std::queue<Match*> matchesQueue;
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

				matchesQueue.push(new Match {nets[i], nets[j], &startBoard, ss.str()});
				ss.str("");
			}
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

		std::sort(nets.begin(), nets.end(), [](NetTracker* a, NetTracker* b) {
			return a->score > b->score;
		});

		for(size_t i = 0; i < nets.size(); ++i)
			std::cout << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;

		for(size_t i = 0; i < (POPULATION_SIZE/2); ++i) 
			(*nets[(POPULATION_SIZE/2)+i]->net) = (*nets[i]->net);

		for(size_t i = (POPULATION_SIZE/2); i < POPULATION_SIZE; ++i)
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