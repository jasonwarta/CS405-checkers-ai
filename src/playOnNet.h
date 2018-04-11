#ifndef PLAY_ON_NET_INCLUDED
#define PLAY_ON_NET_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <utility>
#include <algorithm>

#include "json.hpp"
#include "defs.h"
#include "checkers.h"

std::string getStdoutFromCommand(std::string cmd);
typedef std::pair<std::string,std::string> Status;

class NetworkGame
{
public:
    NetworkGame(NeuralNet * net, std::string address, std::string gameName, bool redTeam) :
        net_(net),
        address_(address),
        gameName_(gameName),
        redTeam_(redTeam) {};

    Status getLastMove();
    std::string sendMove(std::string move);
    void playGame();

private:
    NeuralNet *net_;
    std::string address_;
    std::string gameName_;
    bool redTeam_;
};

#endif