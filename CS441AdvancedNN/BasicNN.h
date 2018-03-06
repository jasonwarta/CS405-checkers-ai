//

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

#include <math.h> // for tanh. may change if we find a faster sigmoid
#include <immintrin.h> // -mavx -march=native?
#include "../src/consts.h"



class BasicNN 
{
public:

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
        for(int i=0; i<edges.size(); ++i)
        {
            edges[i] = startWeightVal(random);
        }

        // set up the sigma
        sigma_.resize(layers.size(), 0.05f);
        //additionStorage.resize(8, 0.0f);
        //summedStorage.resize(8, 0.0f);
    }

    void evolve()
    {
        std::random_device rd;
        std::mt19937 random(rd());
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<> changeKingVal(-0.1, 0.1);
        kingValue_ += changeKingVal(random);

        int t = 1/sqrt(2*sqrt(layers.size()));
        std::normal_distribution<float> distribute(0.0, 1.0);
        for(int i=0; i<layers.size(); ++i)
        {
            sigma_[i] = sigma_[i] * std::exp(t * distribute(generator));
            layers[i] = layers[i] + sigma_[i]*distribute(generator);
        }
    }

    void evaluateNN(const std::string &theBoard)
    {
        setFirstWeights(theBoard);
        EdgesUsed = 0;
        NodesUsed = networkSize[0]; // should be 32
        for(int i=1; i<networkSize.size(); ++i) 
        {
            for(int j=0; j<networkSize[i]; ++ j) 
            {
                currNode = 0;
                for(int k=0; k<networkSize[i-1]; ++k) 
                {
                    // Node - j - size(k) + k = previus set of nodes... hopefully
                    currNode += layers[NodesUsed-j-networkSize[i-1]+k] * edges[EdgesUsed];
                    EdgesUsed++;
                }
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                layers[NodesUsed] = tanh(currNode);
                // layers[NodesUsed] = currNode;
                NodesUsed++;
            }
        }
    }
    void SIMDevaluateNN(const std::string &theBoard)
    {
        setFirstWeights(theBoard);
        EdgesUsed = 0;
        NodesUsed = networkSize[0]; // should also be 32
        float summedStorage[8] __attribute__ ((aligned (32)));
        float additionStorage[8] __attribute__ ((aligned (32))) {0,0,0,0,0,0,0,0};

        for(int i=1; i<networkSize.size(); ++i) 
        {
            for(int j=0; j<networkSize[i]; ++j) 
            {
                __m256 addStorage = _mm256_load_ps(&additionStorage[0]);

                for(int k=0; k<networkSize[i-1]; k+=8) 
                {
                    
                    __m256 edgesSIMD = _mm256_loadu_ps(&edges[EdgesUsed]);
                    __m256 nodeSIMD = _mm256_loadu_ps(&layers[NodesUsed-j-networkSize[i-1]+k]);
                    __m256 nodeWeights = _mm256_mul_ps(edgesSIMD, nodeSIMD);

                    int sizeSIMD = std::min(networkSize[i-1]-k, 8);
                    // __m256 magicalMask - _mm256_load_ps()
                    nodeWeights = _mm256_and_ps(nodeWeights, *(__m256*)&MASK_INCLUDE_TABLE[sizeSIMD][0]); // .data()?
                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    // currNode += (layers[NodesUsed] * edges[EdgesUsed]);
                    EdgesUsed+=sizeSIMD;

                }
                
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                
                _mm256_store_ps(&summedStorage[0], addStorage);
                //_mm256_maskstore_ps(&layers[NodesUsed], *(__m256*)MASK_INCLUDE_TABLE[1].data(), addStorage); // .data()?
                layers[NodesUsed] = tanh(summedStorage[0]);
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                
                NodesUsed++;

            }
        }
    }
    void setFirstWeights(const std::string &theBoard)
    {
        for(int i=0; i<networkSize[0]; ++i)
        {
            layers[i] = 2.2f; // For now: Node = "2.2"
        }
    }
    
    void setNeuralSizes(const std::vector<int> &layerSizes)
   	{
        networkSize = layerSizes;

        int totalNodes = 0;
        for(int i=0; i<layerSizes.size(); ++i) 
        {
            totalNodes += layerSizes[i];
        }
        layers.resize(totalNodes, 0.0f);

        int totalEdges = 0;
        for(int i=0; i<layerSizes.size()-1; ++i)
        {
            totalEdges += layerSizes[i] * layerSizes[i+1];
        }
        edges.resize(totalEdges);
   	}

    void printAll()
    {
        std::cout << std::endl << "------------------NODES----------------" << std::endl;
        NodesUsed = 0;
        for(int i=0; i<networkSize.size(); ++i)
        {
            for(int j=0; j<networkSize[i]; ++j)
            {
                std::cout << layers[NodesUsed] << " ";
                NodesUsed++;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << "------------------EDGES----------------" << std::endl;
        for(int i=0; i<edges.size(); ++i)
        {
            std::cout << edges[i] << " ";
        }
        std::cout << std::endl;
    }
  
    
private:
    std::vector<float> layers;
    std::vector<float> edges;
    std::vector<float> sigma_;
    std::vector<int> networkSize;
    float kingValue_;

    int EdgesUsed;
    int NodesUsed;
    float currNode;

    // std::vector<float> additionStorage;
    // std::vector<float> summedStorage;
    
};





#endif /* NeuralNet_h */
