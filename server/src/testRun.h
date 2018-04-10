#ifndef TEST_RUN_INCLUDED
#define TEST_RUN_INCLUDED

#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include <chrono>
#include <memory>
#include <mutex>
#include <ctime>
#include <math.h>
#include <ostream>

#include "Player.h"
#include "consts.h"
#include "defs.h"

char gameLoop(Player *red, Player *black, int blackTeamDepth, std::string theBoard);
void testRun(Player *red, Player *black);

#endif