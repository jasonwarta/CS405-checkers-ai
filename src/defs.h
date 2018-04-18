
#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <chrono>
#include <vector>

#include "NN91_Basic.h"
#include "BasicNN.h"

// typedef NN91_Basic NeuralNet;
typedef BasicNN NeuralNet;

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

typedef std::uniform_int_distribution<uint> RandUint;

#endif
