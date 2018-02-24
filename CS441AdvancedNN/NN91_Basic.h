//

#ifndef NN91_Basic_h
#define NN91_Basic_h




#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <iomanip>
#include "../src/consts.h"


#include <math.h> // for tanh. may change if we find a faster sigmoid


class NN91_Basic 
{
public:
    
    NN91_Basic(const std::vector<int> &netSize) 
    {
    	setNeuralSizes(netSize);
    }
    void evaluateNN(const std::string &theBoard)
    {
        stringToWeightedBoard(theBoard);
        setFirstWeights();
        // NodeCount = 91 && edgecount = 854

        for(int i=1; i < networkSize.size(); ++i) 
        {
            for(int j=0; j < networkSize[i]; ++ j) 
            {
                currNode = 0;
                for(int k=0; k<networkSize[i-1]; ++k) 
                {
                    currNode += nodes[nodeCount] * edges[edgeCount];
                    edgeCount++;
                }
                nodes[nodeCount] = tanh(currNode);
                // layers[nodeCount] = currNode / (1 + std::abs(currNode));
                nodeCount++;

            }
        }

    }
    void stringToWeightedBoard(const std::string &theBoard)
    {
        weightedStartBoard = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    }
    void setFirstWeights()
    {
        edgeCount = 0;
        nodeCount = 0;
        for(int i=0; i<NN91_NODE_LOCATIONS.size(); ++i)
        {
            for(int j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
            {
                nodes[i] += weightedStartBoard[NN91_NODE_LOCATIONS[i][j]] * edges[edgeCount];
                edgeCount++;
            }
            nodeCount++;
        }

        if(nodeCount != 91 || edgeCount != 854)
        {
            std::cout << "NODE COUNT: " << nodeCount << " EDGE COUNT: " << edgeCount << " SUPPOSED TO BE 91 and 854!! NN91_Basic.h" << std::endl;
        }
    }

    inline void setNeuralSizes(const std::vector<int> &nodeSizes)
   	{
        // Set up network size: [91, nodeSizes, 1]
        networkSize.resize(nodeSizes.size()+2);
        networkSize[0] = 91;
        for(int i=0; i<nodeSizes.size(); ++i)
        {
            networkSize[i+1] = nodeSizes[i];
        }
        networkSize[networkSize.size()-1] = 1;

        int totalNodes = 0;
        for(int i=0; i<nodeSizes.size(); ++i) 
        {
            totalNodes += networkSize[i];
        }
        nodes.resize(totalNodes, 0.0f);

        int totalEdges = 854; // Edges from first set of nodes to the input vector
        for(int i=0; i<nodes.size()-1; ++i)
        {
            totalEdges += networkSize[i] * networkSize[i+1];
        }
        edges.resize(totalEdges);
   	}

    void randomWeights(std::vector<float> & rando) 
    {
        std::random_device rd;
        std::mt19937 random(rd());
        for(int i = 0; i < rando.size(); ++i) 
        {
            std::uniform_real_distribution<> randomfloats(-1.0f, 1.0f);
            rando[i] = randomfloats(random);
        }
    }
    
    float getLastNode()
    {
   		return nodes[nodes.size()-1];
   	} 
    
    
    
private:
    std::vector<float> nodes;
    std::vector<float> edges;
    std::vector<int> networkSize;
    std::vector<float> weightedStartBoard;
    
    int edgeCount;
    int nodeCount;
    float currNode;
};





#endif /* NN91_Basic_h */
