#ifndef MATCHHANDLING_H
#define MATCHHANDLING_H

#include "consts.h"
#include "defs.h"

struct Score {
	int8_t draw = 0;
	int8_t p1 = 0;
	int8_t p2 = 0;

	// given Players 1 and 2, the games should be played as following:
	// game 1: 1v2. call with toggle = true
	// game 2: 2v1. call with toggle = false
	void assignScore(char result, bool toggle, std::ostream * os) {
		(*os) << (toggle ? "1v2" : "2v1") << ": " << result << std::endl;
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

struct NetTracker {
	std::mutex * mtx;
	NeuralNet* net;
	int64_t score = 0;

	void assignScore(Score * gameScore, bool p1) {
		std::lock_guard<std::mutex> guard(*(this->mtx));
		if ( p1 )
			score += (gameScore->p1*WIN_VAL) + (gameScore->p2*LOSS_VAL) + (gameScore->draw*DRAW_VAL);
		else
			score += (gameScore->p2*WIN_VAL) + (gameScore->p1*LOSS_VAL) + (gameScore->draw*DRAW_VAL);
	}
};

std::string playGame(Player * p1, Player * p2, std::string startBoard, Score * score, bool toggle, std::ostream * os) 
{
	std::stringstream ss;
	std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
	Game game( p1, p2, clock, os );
	score->assignScore(game.run(startBoard), toggle, &ss);
	return ss.str();
}

struct Match {
	std::shared_ptr<NetTracker> lhs = nullptr;
	std::shared_ptr<NetTracker> rhs = nullptr;
	std::string startBoard = "";
	std::string fname = "";

	void playMatch() {
		std::ofstream ofs;
		std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());

		std::stringstream ss;
		ofs.open(fname, std::ofstream::out);
		ss << fname << std::endl;

		NeuralNet lhs_net = (*lhs->net);
		NeuralNet rhs_net = (*rhs->net);

		Score score = {0, 0, 0};

		Player p1(true, clock, &lhs_net);
		Player p2(false, clock, &rhs_net);

		ss << playGame(&p1, &p2, startBoard, &score, true, &ofs);
		ss << playGame(&p2, &p1, startBoard, &score, false, &ofs);

		ofs << "\n\n";
		if( score.p1 == score.p2 ) {
			Game game3(&p1, &p2, clock, &ofs);
			score.assignScore(game3.run(), true, &ss);
			ofs << "\n\n";
		}
		ofs.close();

		std::cout << ss.str() << std::endl;

		lhs->assignScore(&score,true);
		rhs->assignScore(&score,false);
	}

	void operator=(Match &match) {
		this->lhs = std::move(match.lhs);
		this->rhs = std::move(match.rhs);
		this->startBoard = match.startBoard;
		this->fname = match.fname;
	}
};

void play(std::mutex &mtx, std::queue<std::shared_ptr<Match>> &matches) {
	while(matches.size() > 0) {
		mtx.lock();
		Match m {};
		if (matches.size() > 0) {
			m = (*matches.front());
			matches.pop();
		} else {
			mtx.unlock();
			return;
		}
		mtx.unlock();

		m.playMatch();
	}
}

#endif