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
    
    NN91_Basic(const std::string &theBoard, const vector<int> &netSize) 
    {
        // netSize not counting first and last layers
    	setNeuralSizes(netSize);
        std::vector<float> weightedStartBoard;
        weightedStartBoard = stringToWeights(theBoard);
        
        for(int i=0; i<nodes.size(); ++i) 
        {
            if(i == 0) 
            {
                //randomWeights(nodes[0]);
                setFirstWeights(weightedStartBoard);
                randomWeights(edges[0]);
            }
            else 
            {
                
                randomWeights(edges[i]);
                for(int j=0; j<nodes[i].size(); ++j) 
                {
                    for(int k=0; k<nodes[i-1].size(); ++k) 
                    {
                        nodes[i][j] += (nodes[i-1][k] * edges[i][nodes[i-1].size()*j+k]);
                    }
                //clamp between -1 and 1
               	nodes[i][j] = tanh(nodes[i][j]);
                }

            }
        }

    }
    std::vector<float> stringToWeights(const std::string &theBoard)
    {
        // put real stringToWeights thingy here later
        std::vector<float> returnMe {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
                                     1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        return returnMe;
    }
    void setNeuralSizes(const std::vector<int> &nodeSizes)
   	{
        // The size passed plus begining and end nodes:
        edges.resize(nodeSizes.size()+2);
        nodes.resize(nodeSizes.size()+2);

        edges[0].resize(854);
        nodes[0].resize(91, 0.0f);
        for(int i=1; i<nodes.size()-1; ++i) 
        {            
            // edges = left.size() * right.size() + right.size()
            edges[i].resize(nodes[i].size() * (nodes[i+1].size()) );
            nodes[i].resize(nodeSizes[i], 0.0f);

        }
        // Last edge, since its going to a single node, is resized to
        // the size of the second to last set of nodes
        edges[edges.size()-1].resize(nodes[nodes.size()-2].size());
        nodes[nodes.size()-1].resize(1, 0.0f);
   	}

    void setFirstWeights(const std::vector<float> &weightedStartBoard)
    {
        if(weightedStartBoard.size() != 32)
        {
            std::cout << "NeuralNet.h Warning: board passed was NOT 32 size. Unknown things about to happen!" << std::endl;
            return;
        }

        int edgeCount = 0;
        for(int i=0; i<nodes[0].size(); ++i)
        {
            for(int j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
            {
                nodes[0][i] += weightedStartBoard[ NN91_NODE_LOCATIONS[i][j] ] * edges[0][edgeCount];
                edgeCount++;
            }
        }
    }

    void randomWeights(vector<float> & rando) 
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
   		return nodes[nodes.size()-1][0];
   	} 
    
    
    
private:
    vector<vector<float>> nodes;
    vector<vector<float>> edges;
    
};





#endif /* NN91_Basic_h */
