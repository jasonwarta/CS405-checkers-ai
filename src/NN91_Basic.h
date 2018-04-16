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
    NN91_Basic(std::ifstream &ifs)
    {
        std::string line;
        while(getline(ifs,line)) {
            if(line.c_str()[0]) {
                std::istringstream is;
                is.str(line);

                std::string label;
                is >> label;

                switch(label.c_str()[0]) {
                    case 'n':
                        {
                            networkSize_.clear();
                            uint temp;
                            uint totalTemp = 0;
                            while(is >>temp) {
                                networkSize_.push_back(temp);
                                totalTemp += temp;
                            }
                            nodes_.resize(totalTemp);
                        }
                        break;

                    case 'e':
                        {
                            edges_.clear();
                            float temp;
                            while(is >> temp) {
                                edges_.push_back(temp);
                            }
                        }
                        break;

                    case 's':
                        {
                            sigma_.clear();
                            float temp;
                            while(is >> temp) {
                                sigma_.push_back(temp);
                            }
                        }
                        break;

                    case 'k':
                        is >> kingValue_;

                    default:
                        break;
                }
            }
        }
        weightedStartBoard_.resize(32);
    }

    /*
    Network start values:
        K = U(1.0, 3.0) == kingValue_
        w = U(-0.2, 0.2) == edges_[i] where i = 0, 1, ... ,n-1
        s = 0.05 == sigma_[i] where i = 0, 1, ... ,n-1
    */
    NN91_Basic(const std::vector<uint> &netSize)
    {
    	setNeuralSizes(netSize);

        // seed randomness
        std::random_device rd;
        std::mt19937 random(rd());

        // Choose a random king value
        std::uniform_real_distribution<> theKingVal(1.0, 3.0);
        kingValue_ = theKingVal(random);

        // set up each weight
        std::uniform_real_distribution<> startWeightVal(-0.2,0.2);
        for(uint i=0; i<edges_.size(); ++i)
        {
            edges_[i] = startWeightVal(random);
        }
    }

    /*
    Network evolve equasions:
        K' = K + U(-0.1, 0.1)
        s'[i] = s[i] exp(t * N(0, 1))
        w'[i] = w[i] + s'[i] * N(0,1)
        t = 1/sqrt(2* sqrt(n))
        n = weights.size()
    */
    void evolve()
    {
        std::random_device rd;
        std::mt19937 random(rd());
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<> changeKingVal(-0.1, 0.1);
        kingValue_ += changeKingVal(random);

        float t = 1/sqrt(2*sqrt(nodes_.size()));
        std::normal_distribution<float> distribute(0.0, 1.0);
        for(uint i=0; i<nodes_.size(); ++i)
        {
            edges_[i] = edges_[i] + sigma_[i]*distribute(generator);
            sigma_[i] = sigma_[i] * std::exp(t * distribute(generator));
        }
    }

    float evaluateNN(std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;
        stringToWeightedBoard(theBoard);
        setFirstWeights();

        float summedStorage[8] __attribute__ ((aligned (32)));
        float additionStorage[8] __attribute__ ((aligned (32))) {0,0,0,0,0,0,0,0};

        for(uint i=1; i<networkSize_.size(); ++i)
        {
            for(uint j=0; j<networkSize_[i]; ++j)
            {
                __m256 addStorage = _mm256_load_ps(&additionStorage[0]);

                for(uint k=0; k<networkSize_[i-1]; k+=8)
                {

                    __m256 edgesSIMD = _mm256_load_ps(&edges_[edgeCount_]);
                    __m256 nodeSIMD = _mm256_load_ps(&nodes_[nodeCount_-j-networkSize_[i-1]+k]);
                    __m256 nodeWeights = _mm256_mul_ps(edgesSIMD, nodeSIMD);

                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    edgeCount_+=8;
                }

                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                _mm256_store_ps(&summedStorage[0], addStorage);

                if(i == networkSize_.size()-1)
                {
                    summedStorage[3]+= weightedBoardEval(theBoard, kingValue_, redTeam_) * edges_[edgeCount_];
                }

                nodes_[nodeCount_] = tanh(summedStorage[3] + summedStorage[4]);
                nodeCount_++;
            }
        }
        return nodes_[nodes_.size()-1];
    }
    float SLOWevaluateNN(std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;
        stringToWeightedBoard(theBoard);
        setFirstWeights();
        // NodeCount = 96 && edgecount = 854
        for(uint i=1; i<networkSize_.size(); ++i)
        {
            for(uint j=0; j<networkSize_[i]; ++j)
            {
                float currNode = 0;
                for(uint k=0; k<networkSize_[i-1]; ++k) 
                {
                    // Node - j - size(k) + k = previus set of nodes_... hopefully
                    currNode += nodes_[nodeCount_-j-networkSize_[i-1]+k] * edges_[edgeCount_];
                    edgeCount_++;
                }
                if(i == networkSize_.size()-1)
                {
                    currNode += weightedBoardEval(theBoard, kingValue_, isRedTeam) * edges_[edgeCount_];
                    edgeCount_++;
                }

                nodes_[nodeCount_] = tanh(currNode);
                nodeCount_++;
            }
        }
        return nodes_[nodes_.size()-1];
    }


    // More-so for testing. Actuall program shouldn't need to call this
    void printAll()
    {
        std::cout << std::endl << "------------------NODES----------------" << std::endl;
        nodeCount_ = 0;
        for(uint i=0; i<networkSize_.size(); ++i)
        {
            for(uint j=0; j<networkSize_[i]; ++j)
            {
                std::cout << nodes_[nodeCount_] << " ";
                nodeCount_++;
            }
            std::cout << std::endl;
        }
/*
        std::cout << std::endl << "------------------EDGES----------------" << std::endl;
        for(uint i=0; i<854; ++i)
        {
            std::cout << edges_[i] << " ";
        }
        std::cout << std::endl << std::endl;
        for(uint i=854; i<edges_.size(); ++i)
        {
            std::cout << edges_[i] << " ";
        }
        std::cout << std::endl;
*/
    }

    void printData(std::ostream *os = &std::cout) {
        (*os) << "net_size ";
        for(auto &ns : networkSize_)
            (*os) << ns << ' ';
        (*os) << std::endl;

        (*os) << "edges ";
        for(auto &e : edges_)
            (*os) << e << ' ';
        (*os) << std::endl;

        (*os) << "sigma ";
        for(auto &s : sigma_)
            (*os) << s << ' ';
        (*os) << std::endl;

        (*os) << "kw " << kingValue_ << std::endl;
    }

private:

    // populates weightedStartBoard based on if there is a checker on
    // that square and if that square is yours.
    void stringToWeightedBoard(std::string &theBoard)
    {
        if(theBoard.size() != 32)
        {
            std::cout << "NN91: string Passed != 32. Resizing string to 32" << std::endl;
            theBoard.resize(weightedStartBoard_.size(), '_');
        }
        for(size_t i = 0; i < 32; ++i)
        {
            switch(theBoard.at(i))
            {
                case '_':
                    weightedStartBoard_[i] = 0.0f;
                    break;
                case 'R':
                    weightedStartBoard_[i] = redTeam_ ? kingValue_ : -kingValue_;
                    break;
                case 'r':
                    weightedStartBoard_[i] = redTeam_ ? 1.0f : -1.0f;
                    break;
                case 'B':
                    weightedStartBoard_[i] = redTeam_ ? -kingValue_ : kingValue_;
                    break;
                case 'b':
                    weightedStartBoard_[i] = redTeam_ ? -1.0f : 1.0f;
                    break;
                default:
                    std::cout << "NN91: passed not-valid char for checker at: " << i << std::endl;
                    weightedStartBoard_[i] = 0.0f;
                    break;
            }
        }
    }

    // Converts the weightedBoard to the first 91 Nodes of the Network.
    // Each node looks at a different square. All 3x3, 4x4, 5x5... 8x8. (91 total)
    void setFirstWeights()
    {
        // Only actually use 91. Last 5 to make AVX happy
        nodeCount_ = 96;
        edgeCount_ = 0;
        std::fill(nodes_.begin(), nodes_.end(), 0.0f);
        for(uint i=0; i<NN91_NODE_LOCATIONS.size(); ++i)
        {
            for(uint j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
            {
                nodes_[i] += weightedStartBoard_[NN91_NODE_LOCATIONS[i][j]] * edges_[edgeCount_];
                edgeCount_++;
            }
            nodes_[i] = tanh(nodes_[i]);
        }

        for(uint i=854; i<856; ++i)
        {
            edges_[i] = 0;
            edgeCount_++;
        }
        // Set last 5 nodes_ to 0 for aligned SIMD
        for(uint i=91; i<96; ++i)
        {
            nodes_[i] = 0;
        }
    }

    // one call to set all vectors to the right size
    // prevents lots of push_backs
    void setNeuralSizes(const std::vector<uint> &nodeSizes)
   	{

        // Set up network size: [91, nodeSizes*8, 1]
        networkSize_.resize(nodeSizes.size()+2);
        networkSize_[0] = 96;
        for(uint i=0; i<nodeSizes.size(); ++i)
        {
            networkSize_[i+1] = nodeSizes[i]*8;
        }
        networkSize_[networkSize_.size()-1] = 1;

        // set up nodes_
        uint totalNodes = 0;
        for(uint i=0; i<networkSize_.size(); ++i)
        {
            totalNodes += networkSize_[i];
        }
        nodes_.resize(totalNodes);

        // set up edges_
        uint totalEdges = 856; // 854 Edges from first set of nodes_ to the input vector + 2 for SIMD alignment
        for(uint i=0; i<networkSize_.size()-1; ++i)
        {
            totalEdges += networkSize_[i] * networkSize_[i+1];
        }
        totalEdges++; // for the edge used with board eval
        edges_.resize(totalEdges);

        // set up other vectors
        weightedStartBoard_.resize(32);
        sigma_.resize(edges_.size(), 0.05f);
   	}

    // Mainly for testing
    void randomWeights(std::vector<float> & rando) {
        std::random_device rd;
        std::mt19937 random(rd());
        for(uint i = 0; i < rando.size(); ++i) {
            std::uniform_real_distribution<> randomfloats(-1.0f, 1.0f);
            rando[i] = randomfloats(random);
        }
    }


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










/*


    // generic slow way:
    void evaluateNN(const std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;

        stringToWeightedBoard(theBoard);
        setFirstWeights();
        // NodeCount = 96 && edgecount = 854
        for(int i=1; i<networkSize_.size(); ++i)
        {
            for(int j=0; j<networkSize_[i]; ++j)
            {
                float currNode_ = 0;
                for(int k=0; k<networkSize_[i-1]; ++k)
                {
                    // Node - j - size(k) + k = previus set of nodes_... hopefully
                    currNode_ += nodes_[nodeCount_-j-networkSize_[i-1]+k] * edges_[edgeCount_];
                    edgeCount_++;
                }

                nodes_[nodeCount_] = tanh(currNode_);
                nodeCount_++;
            }
        }
    }


*/
