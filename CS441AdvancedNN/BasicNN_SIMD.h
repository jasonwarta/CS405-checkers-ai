//

#ifndef BASIC_NN_SIMD_H
#define BASIC_NN_SIMD_H




#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <random>
#include <iomanip>
#include <string>
#include <math.h> // for tanh. may change if we find a faster sigmoid

#include <immintrin.h> // -mavx -march=native?
#include "floats.h"
#include "../src/consts.h"



class BasicNN_SIMD {
public:
    
    BasicNN_SIMD(const std::vector<int> &netSize) 
    {
        networkSize = netSize;
    	setNeuralSizes(netSize);
        additionStorage.resize(8, 0.0f);
    }
    void randomizeWeights()
    {
        randomWeights(edges);
    }

    void evaluateNN(const std::string &theBoard)
    {
        setFirstWeights(theBoard);
        int EdgesUsed = 0;
        int NodesUsed = networkSize[0]; // should also be 32

        for(int i=1; i<networkSize.size(); ++i) 
        {
            for(int j=0; j<networkSize[i]; ++j) 
            {
                std::fill(additionStorage.begin(), additionStorage.end(), 0.0f); // nessessary??
                __m256 addStorage = _mm256_load_ps(&additionStorage[0]);

                for(int k=0; k<networkSize[i-1]; k+=8) 
                {
                    __m256 edgesSIMD = _mm256_load_ps(&edges[EdgesUsed]);
                    __m256 nodeSIMD = _mm256_broadcast_ss(&layers[NodesUsed]);
                    __m256 nodeWeights = _mm256_mul_ps(edgesSIMD, nodeSIMD);

                    int sizeSIMD = std::min(networkSize[i-1]-k, 8);
                    // __m256 magicalMask - _mm256_load_ps()
                    nodeWeights = _mm256_and_ps(nodeWeights, *(__m256*)MASK_INCLUDE_TABLE[sizeSIMD].data()); // .data()?
                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    // currNode += (layers[NodesUsed] * edges[EdgesUsed]);
                    EdgesUsed+=sizeSIMD;
                }
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);

                //_mm256_maskstore_ps(&layers[NodesUsed], *(__m256*)MASK_INCLUDE_TABLE[1].data(), addStorage); // .data()?
                layers[NodesUsed] = tanh(layers[NodesUsed]);
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                NodesUsed++;

            }
        }
    }
    void setFirstWeights(const std::string &theBoard)
    {
        for(int i=0; i<networkSize[0]; ++i)
        {
            layers[i] = 1.0f;
        }
    }
    void setNeuralSizes(const std::vector<int> &layerSizes)
   	{
        int totalNodes = 0;
        for(int i=0; i<layerSizes.size(); ++i) 
        {
            totalNodes += layerSizes[i];
        }
        layers.resize(totalNodes, 0.0f);

        int totalEdges = 0;
        for(int i=0; i<layers.size()-1; ++i)
        {
            totalEdges += layerSizes[i] * layerSizes[i+1];
        }
        edges.resize(totalEdges);
   	}
    
    void randomWeights(std::vector<float> & rando) {
        std::random_device rd;
        std::mt19937 random(rd());
        for(int i = 0; i < rando.size(); ++i) {
            std::uniform_real_distribution<> randomfloats(-1.0, 1.0);
            rando[i] = randomfloats(random);
        }
    }
    
    float getLastNode(){
   		return layers[layers.size()-1];
   	}

  
    
private:
    std::vector<float> layers;
    std::vector<float> edges;
    std::vector<int> networkSize;
    std::vector<float> additionStorage;

};





#endif /* NeuralNet_h */
