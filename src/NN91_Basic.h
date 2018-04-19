//Cameron Showalter
//CS405 && CS441
// NN91_Basic.h
// Verson: too many to remember anymore

#ifndef NN91_Basic_h
#define NN91_Basic_h




#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include <ostream>
#include <math.h> // for tanh. may change if we find a faster sigmoid
#include <immintrin.h>
#include <fstream>
#include <sstream>

#include "consts.h"
#include "alignocator.h"
#include "BoardEval.h"

class NN91_Basic
{
public:
    NN91_Basic(std::ifstream &ifs);

    /*
    Network start values:
        K = U(1.0, 3.0) == kingValue_
        w = U(-0.2, 0.2) == edges_[i] where i = 0, 1, ... ,n-1
        s = 0.05 == sigma_[i] where i = 0, 1, ... ,n-1
    */
    NN91_Basic(const std::vector<uint> &netSize);

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

    float SLOWevaluateNN(std::string &theBoard, bool isRedTeam);

    // More-so for testing. Actuall program shouldn't need to call this
    void printAll();

    void printData(std::ostream *os = &std::cout);

private:

    // populates weightedStartBoard based on if there is a checker on
    // that square and if that square is yours.
    void stringToWeightedBoard(std::string &theBoard);

    // Converts the weightedBoard to the first 91 Nodes of the Network.
    // Each node looks at a different square. All 3x3, 4x4, 5x5... 8x8. (91 total)
    void setFirstWeights();

    // one call to set all vectors to the right size
    // prevents lots of push_backs
    void setNeuralSizes(const std::vector<uint> &nodeSizes);

    // Mainly for testing
    void randomWeights(std::vector<float> & rando);


private:
    std::vector<float, alignocator<float, 32>> nodes_;
    std::vector<float, alignocator<float, 32>> edges_;
    std::vector<float, alignocator<float, 32>> sigma_;
    std::vector<uint> networkSize_;
    std::vector<float> weightedStartBoard_;

    float kingValue_;
    bool redTeam_;
    uint edgeCount_;
    uint nodeCount_;
};

#endif /* NN91_Basic_h */
