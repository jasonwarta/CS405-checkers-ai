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
	
	std::vector<std::shared_ptr<NetTracker>> nets;

	if(argc > 1) {
		if (std::string("-nets").compare(argv[1]) == 0) {
			// use `./main -nets path/to/directory/of/nets`
			std::cout << "-nets" << " " << argv[2] << std::endl;
			std::ifstream ifs;
			for(auto& f: fs::directory_iterator(argv[2])) {
				ifs.open(f.path());
				uint8_t netIndex = 0;
				nets.push_back(std::make_shared<NetTracker>(NetTracker{new NeuralNet(ifs), netIndex++}));
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

			Player p1(true, net);
			Player p2(false, nullptr, true);

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
			nets.push_back(std::make_shared<NetTracker>(NetTracker {new NeuralNet(NET_SIZE), i}));
	}

	std::random_device r;
	std::mt19937 rand(r());
	std::uniform_int_distribution<uint> randIndex(0,POPULATION_SIZE-1);
	
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

		std::vector<size_t> matchTracker(POPULATION_SIZE, MATCHES);
		std::vector<size_t> availableMatches(POPULATION_SIZE);
		
		for(uint i = 0; i < POPULATION_SIZE; ++i) {
			availableMatches[i] = i;
		}

		bool validMatchExists = true;
		while(true){

			for (auto &i : availableMatches)
				std::cout << i << " ";
			std::cout << std::endl;
			
			if(availableMatches.size() == 0)
			{
				validMatchExists = false;
				break;
			}
			else if (!validMatchExists)
			{
				size_t num1 = matchesQueue.front()->lhs->self;
				size_t num2 = matchesQueue.front()->rhs->self;

				// std::cout << num1 << " " << num2 << std::endl;

				matchesQueue.front()->lhs->opponents.erase( 
					std::find(
						matchesQueue.front()->lhs->opponents.begin(),
						matchesQueue.front()->lhs->opponents.end(),
						num2
					)
				);

				matchesQueue.front()->rhs->opponents.erase( 
					std::find(
						matchesQueue.front()->rhs->opponents.begin(),
						matchesQueue.front()->rhs->opponents.end(),
						num1
					)
				);

				matchesQueue.pop();

				matchTracker[num1]++;
				matchTracker[num2]++;

				if(std::find(availableMatches.begin(),availableMatches.end(),num1) == availableMatches.end())
					availableMatches.push_back(num1);

				if (std::find(availableMatches.begin(), availableMatches.end(), num2) == availableMatches.end())
					availableMatches.push_back(num2);

				validMatchExists = thereExistsAValidMatch(availableMatches, nets);			
			}

			else
			{
				std::swap(availableMatches.at(RandUint(0, availableMatches.size() - 1)(rand)), availableMatches.back());
				uint p1idx = availableMatches.back();
				uint p2idx = availableMatches.at(RandUint(0, availableMatches.size() - 2)(rand));

				if(std::find(nets[p1idx]->opponents.begin(),nets[p1idx]->opponents.end(), p2idx) != nets[p1idx]->opponents.end())
					continue;

				ss.str("");
				ss << path << "/games/" << std::setfill('0') << std::setw(2) << p1idx << "v" << std::setfill('0') << std::setw(2) << p2idx;

				matchesQueue.push(std::make_unique<Match>(Match{nets[p1idx], nets[p2idx], startBoard, ss.str()}));
				ss.str("");

				matchTracker[p1idx]--;
				matchTracker[p2idx]--;

				nets[p1idx]->opponents.push_back(p2idx);
				nets[p2idx]->opponents.push_back(p1idx);

				if (matchTracker[p1idx] == 0)
					availableMatches.erase(std::find(availableMatches.begin(),availableMatches.end(), p1idx));

				if (matchTracker[p2idx] == 0)
					availableMatches.erase(std::find(availableMatches.begin(), availableMatches.end(), p2idx));

				validMatchExists = thereExistsAValidMatch(availableMatches, nets);
			}
		}

		std::cout << "Total Matches: " << matchesQueue.size() << std::endl;
		for(size_t i = 0; i < nets.size(); ++i) {
			std::cout << std::setfill('0') << std::setw(2) << i << ": ";
			for(size_t j = 0; j < nets[i]->opponents.size(); ++j)
				std::cout << std::setfill('0') << std::setw(2) << int(nets[i]->opponents[j]) << " ";
			std::cout << std::endl;
		}

		std::vector<std::thread> threads;
		for (uint i = 0; i < NUM_THREADS; ++i)
		{
			threads.push_back( std::thread(play, std::ref(mtx), std::ref(matchesQueue)) );
		}
		
		for (uint i = 0; i < threads.size(); ++i)
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
			return a->score > b->score;
		});

		for(size_t i = 0; i < nets.size(); ++i)
			std::cout << std::setfill('0') << std::setw(2) << i << ": " << nets[i]->score << std::endl;

		for(size_t i = 0; i < (POPULATION_SIZE/2); ++i) 
			(*nets[(POPULATION_SIZE/2)+i]->net) = (*nets[i]->net);
		
		for(size_t i = (POPULATION_SIZE/2); i < POPULATION_SIZE; ++i)
			nets[i]->net->evolve();
		
		for(uint i = 0; i < nets.size(); ++i)
		{
			nets[i]->score = 0;
			nets[i]->opponents.clear();
			nets[i]->self = i;
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