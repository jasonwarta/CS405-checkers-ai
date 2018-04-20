//Cameron Showalter
//CS405 && CS441
// BasicNN.h
// Verson: too many to remember anymore



#ifndef BASIC_NN_H
#define BASIC_NN_H


#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <random>
#include <iomanip>
#include <cmath> //pow
#include <string>
#include <fstream>
#include <sstream>
#include <math.h> // for tanh. may change if we find a faster sigmoid
#include <immintrin.h> // -mavx

#include "consts.h"
#include "alignocator.h"
#include "BoardEval.h"


class BasicNN
{
public:
    BasicNN(std::ifstream &ifs);
    /*
    Network start values:
        K = U(1.0, 3.0) == kingValue_
        w = U(-0.2, 0.2) == edges_[i] where i = 0, 1, ... ,n-1
        s = 0.05 == sigma_[i] where i = 0, 1, ... ,n-1
    */
    BasicNN(const std::vector<uint> &netSize);

    /*
    Network evolve equasions:
        K' = K + U(-0.1, 0.1)
        s'[i] = s[i] exp(t * N(0, 1))
        w'[i] = w[i] + s'[i] * N(0,1)
        t = 1/sqrt(2* sqrt(n))
        n = weights.size()
    */
    void evolve();

    float evaluateNN(std::string &theBoard, bool isRedTeam);

    void printAll();

    void printData(std::ostream *os = &std::cout);

private:
    // Looks at the checkerboard, and sets first set of weights
    // based on whose team you're on
    void setFirstWeights(const std::string &theBoard);

    // Does resizes on network creation to avoid lots of push_backs later
    void setNeuralSizes(const std::vector<uint> &layerSizes);

    // Fast square root
    float Q_rsqrt(float number);

private:
    std::vector<float, alignocator<float, 32>> nodes_;
    std::vector<float, alignocator<float, 32>> edges_;
    std::vector<float> sigma_;
    std::vector<uint> networkSize_;

    float kingValue_;
    bool redTeam_;
    uint EdgesUsed_;
    uint NodesUsed_;

};





#endif /* NeuralNet_h */
