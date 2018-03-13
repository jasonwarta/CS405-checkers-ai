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
#include <fstream>
#include <sstream>

#include <math.h> // for tanh. may change if we find a faster sigmoid
#include <immintrin.h> // -mavx -march=native?
#include "../src/consts.h"



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
                            networkSize.clear();
                            int temp;
                            while(is >>temp) {
                                networkSize.push_back(temp);
                            }
                        }
                        break;

                    case 'e':
                        {
                            edges.clear();
                            float temp;
                            while(is >> temp) {
                                edges.push_back(temp);
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
        // this->printData();
    }

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

        float t = 1/sqrt(2*sqrt(layers.size()));
        std::normal_distribution<float> distribute(0.0, 1.0);
        for(int i=0; i<layers.size(); ++i)
        {
            sigma_[i] = sigma_[i] * std::exp(t * distribute(generator));
            layers[i] = layers[i] + sigma_[i]*distribute(generator);
        }
    }

    // void evaluateNN(const std::string &theBoard)
    // {
    //     setFirstWeights(theBoard);
    //     EdgesUsed = 0;
    //     NodesUsed = networkSize[0]; // should be 32
    //     for(int i=1; i<networkSize.size(); ++i) 
    //     {
    //         for(int j=0; j<networkSize[i]; ++ j) 
    //         {
    //             currNode = 0;
    //             for(int k=0; k<networkSize[i-1]; ++k) 
    //             {
    //                 // Node - j - size(k) + k = previus set of nodes... hopefully
    //                 currNode += layers[NodesUsed-j-networkSize[i-1]+k] * edges[EdgesUsed];
    //                 EdgesUsed++;
    //             }
    //             // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
    //             layers[NodesUsed] = tanh(currNode);
    //             // layers[NodesUsed] = currNode;
    //             NodesUsed++;
    //         }
    //     }
    // }

    void printData(std::ostream *os = &std::cout) {
        (*os) << "net_size ";
        for(auto &ns : networkSize)
            (*os) << ns << ' ';
        (*os) << std::endl;

        (*os) << "edges ";
        for(auto &e : edges)
            (*os) << e << ' ';
        (*os) << std::endl;

        (*os) << "sigma ";
        for(auto &s : sigma_)
            (*os) << s << ' ';
        (*os) << std::endl;

        (*os) << "kw " << kingValue_ << std::endl;
    }

    void evaluateNN(const std::string &theBoard)
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
                    nodeWeights = _mm256_and_ps(nodeWeights, *(__m256*)&MASK_INCLUDE_TABLE[sizeSIMD][0]);
                    addStorage = _mm256_add_ps(addStorage, nodeWeights);

                    _mm256_store_ps(&summedStorage[0], addStorage);
                    EdgesUsed+=sizeSIMD;
                }
                
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);                
                _mm256_store_ps(&summedStorage[0], addStorage);

                layers[NodesUsed] = tanh(summedStorage[0] + summedStorage[4]);
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                
                NodesUsed++;
            }
        }
    }
    void AlignedEvaluateNN(const std::string &theBoard)
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
                    _mm256_store_ps(&summedStorage[0], addStorage);

                    EdgesUsed+=sizeSIMD;

                }
                
                // Horizontal add of 8 elements
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                addStorage = _mm256_hadd_ps(addStorage, addStorage);
                // addStorage = _mm256_hadd_ps(addStorage, addStorage);
                
                _mm256_store_ps(&summedStorage[0], addStorage);

                layers[NodesUsed] = tanh(summedStorage[0] + summedStorage[4]);
                // layers[NodesUsed] = currNode / (1 + std::abs(currNode));
                
                NodesUsed++;

            }
        }
    }

    void setFirstWeights(const std::string &theBoard)
    {
        for(size_t i = 0; i < 32; ++i) {
            switch(theBoard[i]) {
                case 'R':
                    layers[i] = redTeam ? kingValue_ : -kingValue_;
                    break;
                case 'r':
                    layers[i] = redTeam ? 1.0f : -1.0f;
                    break;
                case 'B':
                    layers[i] = redTeam ? -kingValue_ : kingValue_;
                    break;
                case 'b':
                    layers[i] = redTeam ? -1.0f : 1.0f;
                    break;
                default:
                    layers[i] = 0.0f;
                    break;
            }
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
    float getLastNode()
    {
        return layers[layers.size()-1];
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

    BasicNN & operator=(BasicNN & other) {
        if (this != &other) {

            std::copy(other.layers.begin(), other.layers.end(), this->layers.begin());
            std::copy(other.edges.begin(), other.edges.end(), this->edges.begin());
            std::copy(other.sigma_.begin(), other.sigma_.end(), this->sigma_.begin());
            std::copy(other.networkSize.begin(), other.networkSize.end(), this->networkSize.begin());

            this->kingValue_ = other.kingValue_;
            this->redTeam = other.redTeam;
            this->EdgesUsed = other.EdgesUsed;
            this->NodesUsed = other.NodesUsed;
            this->currNode = other.currNode;
        }
        return *this;
    }
  
    
private:
    std::vector<float> layers;
    std::vector<float> edges;
    std::vector<float> sigma_;
    std::vector<int> networkSize;
    float kingValue_;

    bool redTeam;
    int EdgesUsed;
    int NodesUsed;
    float currNode;

    // std::vector<float> additionStorage;
    // std::vector<float> summedStorage;
    
};





#endif /* NeuralNet_h */
