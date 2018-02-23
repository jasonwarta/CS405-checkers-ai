//

#ifndef NeuralNet_h
#define NeuralNet_h




#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
using std::mt19937;
using std::vector;
using std::uniform_real_distribution;
using std::cout;
using std::endl;


#include <math.h> // for tanh. may change if we find a faster sigmoid


class BasicNN {
public:
    
    BasicNN(const vector<int> &netSize) 
    {
        networkSize = netSize;
    	setNeuralSizes(networkSize);
    }
    void randomizeWeights()
    {
        //randomWeights(layers);
        randomWeights(edges);

    }
    void evaluateNN()
    {
        EdgesUsed = 0;
        NodesUsed = 0;
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
    
    void randomWeights(vector<float> & rando) {
        std::random_device rd;
        std::mt19937 random(rd());
        for(int i = 0; i < rando.size(); ++i) {
            uniform_real_distribution<> randomfloats(-1.0, 1.0);
            rando[i] = randomfloats(random);
        }
    }
    
    float getLastNode(){
   		return layers[layers.size()-1];
   	}

/*
    void printvec(vector<vector<float>> & x) {
        for(int i = 0; i < x.size(); ++i) {
            for(int j=0; j < x[i].size(); ++j) {
                std::cout << x[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
*/    
    
    
    
private:
    vector<float> layers;
    vector<float> edges;
    vector<int> networkSize;
    
    int EdgesUsed;
    int NodesUsed;
    float currNode;
    
};





#endif /* NeuralNet_h */
