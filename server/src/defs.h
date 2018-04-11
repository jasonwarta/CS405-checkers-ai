#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <chrono>

#include "NN91_Basic.h"
#include "BasicNN.h"

// typedef NN91_Basic NeuralNet;
typedef BasicNN NeuralNet;

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

#endif