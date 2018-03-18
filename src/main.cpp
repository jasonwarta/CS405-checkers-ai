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

#include "../CS441AdvancedNN/BasicNN.h"

const size_t LOSS_VAL = -1;
const size_t DRAW_VAL = 0;
const size_t WIN_VAL = 2;

struct NetTracker {
	BasicNN* net;
	int64_t score = 0;
};

int main(int argc, char const *argv[]) {
	
	std::ofstream ofs;
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	const std::vector<int> netSize {32,40,10,1};
	std::vector<NetTracker*> nets;

	for(size_t i = 0; i < 30; ++i)
		nets.push_back(new NetTracker {new BasicNN(netSize), 0});

	uint64_t genCounter = 0;

	while(true) {
		ss.str("");
		ss << "./NN/gen_" << std::setfill('0') << std::setw(3) << genCounter;
		std::string path = ss.str();
		system(("mkdir -p "+path+"/nets").c_str());
		system(("mkdir -p "+path+"/games").c_str());
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

				Player p1(true, clock, nets[i]->net);
				Player p2(false, clock, nets[j]->net);

				size_t drawCounter = 0, p1Counter = 0, p2Counter = 0;

				Game game1( &p1, &p2, clock, &ofs );
				char result1 = game1.run(startBoard);

				switch(result1) {
					case 'R':
						p1Counter++;
						break;
					case 'B':
						p2Counter++;
						break;
					case 'D':
						drawCounter++;
						break;
					default:
						break;
				}
				ofs << "\n\n";

				Game game2( &p2, &p1, clock, &ofs );
				char result2 = game2.run(startBoard);

				switch(result2) {
					case 'R':
						p2Counter++;
						break;
					case 'B':
						p1Counter++;
						break;
					case 'D':
						drawCounter++;
						break;
					default:
						break;
				}
				ofs << "\n\n";

				if( !(p1Counter == 2 || p2Counter == 2) ) {
					Game game3(&p1, &p2, clock, &ofs);
					char result3 = game3.run();

					switch(result3) {
						case 'R':
							p1Counter++;
							break;
						case 'B':
							p2Counter++;
							break;
						case 'D':
							drawCounter++;
							break;
						default:
							break;
					}
					ofs << "\n\n";
				}

				ofs.close();

				nets[i]->score += (p1Counter*WIN_VAL) + (p2Counter*LOSS_VAL) + (drawCounter*DRAW_VAL);
				nets[j]->score += (p2Counter*WIN_VAL) + (p1Counter*LOSS_VAL) + (drawCounter*DRAW_VAL);

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