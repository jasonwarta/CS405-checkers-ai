//

#ifndef BASIC_NN_H
#define BASIC_NN_H




#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>



#include <math.h> // for tanh. may change if we find a faster sigmoid


class BasicNN {
public:
    
    BasicNN(const std::vector<int> &netSize) 
    {
        networkSize = netSize;
    	setNeuralSizes(networkSize);
    }
    void randomizeWeights()
    {
        randomWeights(edges);
    }
    void evaluateNN(const std::string &theBoard)
    {
        setFirstWeights(theBoard);
        EdgesUsed = networkSize[0];
        NodesUsed = networkSize[0]; // should be 32
        for(int i=1; i < networkSize.size(); ++i) 
        {
            for(int j=0; j < networkSize[i]; ++ j) 
            {
                currNode = 0;
                for(int k=0; k<networkSize[i-1]; ++k) 
                {
                    currNode += (layers[NodesUsed] * edges[EdgesUsed]);
                    EdgesUsed++;
                }
                layers[NodesUsed] = tanh(currNode);
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                NodesUsed++;

            }
        }
    }
    void setFirstWeights(const std::string &theBoard)
    {
        for(int i=0; i<networkSize[0]; ++i)
        {
            edges[i] = 2.2f;
            layers[i] = 3.0f * 2.2f; // For now: Node = "PieceRank" * edge
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

        int totalEdges = layerSizes[0]; // Should be the first 32
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
    
    int EdgesUsed;
    int NodesUsed;
    float currNode;
    
};





#endif /* NeuralNet_h */
