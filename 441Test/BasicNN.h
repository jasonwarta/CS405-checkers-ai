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


class BasicNN 
{
public:

    /*
    Network start values: 
        K = U(1.0, 3.0) == kingValue_
        w = U(-0.2, 0.2) == edges_[i] where i = 0, 1, ... ,n-1
        s = 0.05 == sigma_[i] where i = 0, 1, ... ,n-1
    */
    BasicNN(const std::vector<int> &netSize)
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
        for(int i=0; i<edges_.size(); ++i)
        {
            edges_[i] = startWeightVal(random);
        }

        // set up the sigma
        sigma_.resize(nodes_.size(), 0.05f);
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
        for(int i=0; i<nodes_.size(); ++i)
        {
            sigma_[i] = sigma_[i] * std::exp(t * distribute(generator));
            nodes_[i] = nodes_[i] + sigma_[i]*distribute(generator);
        }
    }

    // Slow way:
    void evaluateNN(const std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;
        setFirstWeights(theBoard);
        EdgesUsed_ = 0;
        NodesUsed_ = networkSize_[0]; // should be 32
        for(int i=1; i<networkSize_.size(); ++i) 
        {
            for(int j=0; j<networkSize_[i]; ++ j) 
            {
                // tried declaring float inside method and as class
                // both timings were about the same
                float currNode = 0;
                for(int k=0; k<networkSize_[i-1]; ++k) 
                {
                    currNode += nodes_[NodesUsed_-j-networkSize_[i-1]+k] * edges_[EdgesUsed_];
                    EdgesUsed_++;
                }

                nodes_[NodesUsed_] = tanh(currNode);
                NodesUsed_++;
            }
        }
    }

    // ints inside networkSize_ do not have to be mults of 8
    void SIMDevaluateNN(const std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;

        setFirstWeights(theBoard);
        EdgesUsed_ = 0;
        NodesUsed_ = networkSize_[0]; // should also be 32
        float summedStorage[8] __attribute__ ((aligned (32)));
        float additionStorage[8] __attribute__ ((aligned (32))) {0,0,0,0,0,0,0,0};

        for(int i=1; i<networkSize_.size(); ++i) 
        {
            for(int j=0; j<networkSize_[i]; ++j) 
            {
                __m256 addStorage = _mm256_load_ps(&additionStorage[0]);

                for(int k=0; k<networkSize_[i-1]; k+=8) 
                {
                    __m256 edgesSIMD = _mm256_loadu_ps(&edges_[EdgesUsed_]);
                    __m256 nodeSIMD = _mm256_loadu_ps(&nodes_[NodesUsed_-j-networkSize_[i-1]+k]);
                    __m256 nodeWeights = _mm256_mul_ps(edgesSIMD, nodeSIMD);

                    int sizeSIMD = std::min(networkSize_[i-1]-k, 8);
                    nodeWeights = _mm256_and_ps(nodeWeights, *(__m256*)&MASK_INCLUDE_TABLE[sizeSIMD][0]);
                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    EdgesUsed_+=sizeSIMD;
                }
                
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);   
                // Cannot do 3 hadds. Must grab top and bottom numbers manually             
                _mm256_store_ps(&summedStorage[0], addStorage);

                // right next to eachother may increase speed? (vs grabbing 0 and 7?)
                nodes_[NodesUsed_] = tanh(summedStorage[3] + summedStorage[4]);
                
                NodesUsed_++;
            }
        }
    }

    // All ints but the last one in networkSize_ must be mults of 8
    void AlignedEvaluateNN(const std::string &theBoard, bool isRedTeam)
    {
        redTeam_ = isRedTeam;

        setFirstWeights(theBoard);
        EdgesUsed_ = 0;
        NodesUsed_ = networkSize_[0]; // should also be 32
        float summedStorage[8] __attribute__ ((aligned (32)));
        float additionStorage[8] __attribute__ ((aligned (32))) {0,0,0,0,0,0,0,0};

        for(int i=1; i<networkSize_.size(); ++i) 
        {
            for(int j=0; j<networkSize_[i]; ++j) 
            {
                __m256 addStorage = _mm256_load_ps(&additionStorage[0]);

                for(int k=0; k<networkSize_[i-1]; k+=8) 
                {
                    
                    __m256 edgesSIMD = _mm256_load_ps(&edges_[EdgesUsed_]);
                    __m256 nodeSIMD = _mm256_load_ps(&nodes_[NodesUsed_-j-networkSize_[i-1]+k]);
                    __m256 nodeWeights = _mm256_mul_ps(edgesSIMD, nodeSIMD);

                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    EdgesUsed_+=8;
                }
                
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);                
                _mm256_store_ps(&summedStorage[0], addStorage);

                nodes_[NodesUsed_] = tanh(summedStorage[3] + summedStorage[4]);
                
                NodesUsed_++;
            }
        }
    }
    float getLastNode()
    {
        return nodes_[nodes_.size()-1];
    } 


private:
    // Looks at the checkerboard, and sets first set of weights
    // based on whose team you're on
    void setFirstWeights(const std::string &theBoard)
    {
        for(size_t i = 0; i < 32; ++i) 
        {
            switch(theBoard[i]) 
            {
                case 'R':
                    nodes_[i] = redTeam_ ? kingValue_ : -kingValue_;
                    break;
                case 'r':
                    nodes_[i] = redTeam_ ? 1.0f : -1.0f;
                    break;
                case 'B':
                    nodes_[i] = redTeam_ ? -kingValue_ : kingValue_;
                    break;
                case 'b':
                    nodes_[i] = redTeam_ ? -1.0f : 1.0f;
                    break;
                default:
                    nodes_[i] = 0.0f;
                    break;
            }
        }
    }
    
    // Does resizes on network creation to avoid lots of push_backs later
    void setNeuralSizes(const std::vector<int> &layerSizes)
   	{
        networkSize_ = layerSizes;

        int totalNodes = 0;
        for(int i=0; i<layerSizes.size(); ++i) 
        {
            totalNodes += layerSizes[i];
        }
        nodes_.resize(totalNodes, 0.0f);

        int totalEdges = 0;
        for(int i=0; i<layerSizes.size()-1; ++i)
        {
            totalEdges += layerSizes[i] * layerSizes[i+1];
        }
        edges_.resize(totalEdges);
   	}

private:
    std::vector<float, alignocator<float, 32>> nodes_;
    std::vector<float, alignocator<float, 32>> edges_;
    std::vector<float> sigma_;
    std::vector<int> networkSize_;

    float kingValue_;
    bool redTeam_;
    int EdgesUsed_;
    int NodesUsed_;
    
};





#endif /* NeuralNet_h */
