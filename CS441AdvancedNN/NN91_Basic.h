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
#include <ostream>


#include <math.h> // for tanh. may change if we find a faster sigmoid


class NN91_Basic 
{
public:
    
    NN91_Basic(const std::vector<int> &netSize) 
    {
    	setNeuralSizes(netSize);

        // seed randomness
        std::random_device rd;
        std::mt19937 random(rd());

        // Choose a random king value
        std::uniform_real_distribution<> theKingVal(1.0, 3.0);
        kingValue_ = theKingVal(random);
        std::cout << kingValue_ << std::endl;

        // set up each weight
        std::uniform_real_distribution<> startWeightVal(-0.2,0.2);
        for(int i=0; i<edges.size(); ++i)
        {
            edges[i] = startWeightVal(random);
        }

        // set up the sigma
        sigma_.resize(nodes.size(), 0.05f);
    }

    void evaluateNN(const std::string &theBoard)
    {
        // std::cout << "Evaluating net" << std::endl;
        stringToWeightedBoard(theBoard);
        setFirstWeights();
        // NodeCount = 91 && edgecount = 854
        for(int i=1; i<networkSize.size(); ++i)
        {
            for(int j=0; j<networkSize[i]; ++j)
            {
                currNode = 0;
                for(int k=0; k<networkSize[i-1]; ++k) 
                {
                    // Node - j - size(k) + k = previus set of nodes... hopefully
                    currNode += nodes[nodeCount-j-networkSize[i-1]+k] * edges[edgeCount];
                    edgeCount++;
                }
                nodes[nodeCount] = tanh(currNode);
                // nodes[nodeCount] = currNode;
                nodeCount++;

                // layers[nodeCount] = currNode / (1 + std::abs(currNode));
            }
        }
        // std::cout << "done evaluating net" << std::endl;
    }
    void SIMDevaluateNN(const std::string &theBoard)
    {
        stringToWeightedBoard(theBoard);
        setFirstWeights();

    }
    void stringToWeightedBoard(const std::string &theBoard)
    {
        std::cout << "something" << std::endl;
        for(size_t i = 0; i < weightedStartBoard.size(); ++i) {
            switch(theBoard[i]) {
                case 'R':
                    weightedStartBoard[i] = redTeam ? kingValue_ : -kingValue_;
                    break;
                case 'r':
                    weightedStartBoard[i] = redTeam ? 1.0f : -1.0f;
                    break;
                case 'B':
                    weightedStartBoard[i] = redTeam ? -kingValue_ : kingValue_;
                    break;
                case 'b':
                    weightedStartBoard[i] = redTeam ? -1.0f : 1.0f;
                    break;
                default:
                    weightedStartBoard[i] = 0.0f;
                    break;
            }
        }
        std::cout << "done" << std::endl;

        // weightedStartBoard = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        //                       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        //                       -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    }
    
    void setFirstWeights()
    {
        nodeCount = 91;
        edgeCount = 0;
        for(int i=0; i<NN91_NODE_LOCATIONS.size(); ++i)
        {
            for(int j=0; j<NN91_NODE_LOCATIONS[i].size(); ++j)
            {
                nodes[i] += weightedStartBoard[NN91_NODE_LOCATIONS[i][j]] * edges[edgeCount];
                edgeCount++;
            }
            // add function for this later?
            nodes[i] = tanh(nodes[i]);
        }
    }

    void setNeuralSizes(const std::vector<int> &nodeSizes)
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
        for(int i=0; i<networkSize.size(); ++i) 
        {
            totalNodes += networkSize[i];
        }
        nodes.resize(totalNodes, 0.0f);

        int totalEdges = 854; // Edges from first set of nodes to the input vector
        for(int i=0; i<networkSize.size()-1; ++i)
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
    
    void printAll()
    {
        std::cout << std::endl << "------------------NODES----------------" << std::endl;
        nodeCount = 0;
        for(int i=0; i<networkSize.size(); ++i)
        {
            for(int j=0; j<networkSize[i]; ++j)
            {
                std::cout << nodes[nodeCount] << " ";
                nodeCount++;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << "------------------EDGES----------------" << std::endl;
        for(int i=0; i<854; ++i)
        {
            std::cout << edges[i] << " ";
        }
        std::cout << std::endl << std::endl;
        for(int i=854; i<edges.size(); ++i)
        {
            std::cout << edges[i] << " ";
        }
        std::cout << std::endl;
    }

    void printData(std::ostream *os = &std::cout) {
        (*os) << "Edges" << std::endl;
        for(auto &e : edges)
            (*os) << e << ' ';
        (*os) << std::endl;

        (*os) << "Sigma" << std::endl;
        for(auto &s : sigma_)
            (*os) << s << ' ';
        (*os) << std::endl;

        (*os) << "King Weight:\t" << kingValue_ << std::endl;
    }


private:
    std::vector<float> nodes;
    std::vector<float> edges;
    std::vector<float> sigma_;
    std::vector<int> networkSize;
    std::vector<float> weightedStartBoard;
    float kingValue_;

    bool redTeam;

    int edgeCount;
    int nodeCount;
    float currNode;
};





#endif /* NN91_Basic_h */
