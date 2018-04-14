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
#include "BoardEval.h"


class BasicNN 
{
public:
    BasicNN(std::ifstream &ifs) 
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
    }
    /*
    Network start values: 
        K = U(1.0, 3.0) == kingValue_
        w = U(-0.2, 0.2) == edges_[i] where i = 0, 1, ... ,n-1
        s = 0.05 == sigma_[i] where i = 0, 1, ... ,n-1
    */
    BasicNN(const std::vector<uint> &netSize)
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

        // set up the sigma
        sigma_.resize(edges_.size(), 0.05f);
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
        for(uint i=0; i<edges_.size(); ++i)
        {
            edges_[i] = edges_[i] + sigma_[i]*distribute(generator);
            sigma_[i] = sigma_[i] * std::exp(t * distribute(generator));
        }
    }

    float evaluateNN(std::string &theBoard, bool isRedTeam)
    {
        if(theBoard.size() != networkSize_[0])
        {
            std::cout << "Warning: string passed to Network does not match input layer size. resizing string." << std::endl;
            theBoard.resize(networkSize_[0], '_');
        }
        redTeam_ = isRedTeam;

        setFirstWeights(theBoard);
        EdgesUsed_ = 0;
        NodesUsed_ = networkSize_[0]; // should be 32
        for(uint i=1; i<networkSize_.size(); ++i) 
        {
            for(uint j=0; j<networkSize_[i]; ++ j) 
            {
                // tried declaring float inside method and as class
                // both timings were about the same
                float currNode = 0;
                for(uint k=0; k<networkSize_[i-1]; ++k) 
                {
                    currNode += nodes_[NodesUsed_-j-networkSize_[i-1]+k] * edges_[EdgesUsed_];
                    EdgesUsed_++;
                }
                if(i == networkSize_.size()-1)
                {
                    currNode+= weightedBoardEval(theBoard, kingValue_, redTeam_) * edges_[EdgesUsed_];
                    EdgesUsed_++;
                }

                nodes_[NodesUsed_] = tanh(currNode);
                NodesUsed_++;
            }
        }
        return nodes_[nodes_.size()-1];
    }

    void printAll()
    {
        std::cout << std::endl << "------------------NODES----------------" << std::endl;
        uint nodeCount_ = 0;
        for(uint i=0; i<networkSize_.size(); ++i)
        {
            for(uint j=0; j<networkSize_[i]; ++j)
            {
                std::cout << nodes_[nodeCount_] << " ";
                nodeCount_++;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << "------------------EDGES----------------" << std::endl;
        for(uint i=0; i<edges_.size(); ++i)
        {
            std::cout << edges_[i] << " ";
        }
        std::cout << std::endl << std::endl;

        std::cout << std::endl << "------------------SIGMA----------------" << std::endl;
        for(uint i=0; i<sigma_.size(); ++i)
        {
            std::cout << sigma_[i] << " ";
        }
        std::cout << std::endl;
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
    // Looks at the checkerboard, and sets first set of weights
    // based on whose team you're on
    void setFirstWeights(const std::string &theBoard)
    {
        for(size_t i = 0; i < theBoard.size(); ++i) 
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
    void setNeuralSizes(const std::vector<uint> &layerSizes)
   	{
        networkSize_ = layerSizes;

        uint totalNodes = 0;
        for(uint i=0; i<layerSizes.size(); ++i) 
        {
            totalNodes += layerSizes[i];
        }
        nodes_.resize(totalNodes, 0.0f);

        uint totalEdges = layerSizes[layerSizes.size()-1]; // Start at one for the piece count weight
        for(uint i=0; i<layerSizes.size()-1; ++i)
        {
            totalEdges += layerSizes[i] * layerSizes[i+1];
        }
        edges_.resize(totalEdges);
   	}

private:
    std::vector<float, alignocator<float, 32>> nodes_;
    std::vector<float, alignocator<float, 32>> edges_;
    std::vector<float> sigma_;
    std::vector<uint> networkSize_;

    float kingValue_;
    bool redTeam_;
    uint EdgesUsed_;
    uint NodesUsed_;
    
};





#endif /* NeuralNet_h */
