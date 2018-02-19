//

#ifndef NeuralNet_Ref_h
#define NeuralNet_Ref_h




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


class BasicNN_Ref {
public:
    
    BasicNN_Ref(vector<int> &netSize, vector<vector<float>> &layers, vector<vector<float>> &edges) {

    	setNeuralSizes(netSize, layers, edges);

        for(int i = 0; i < layers.size(); ++i) {
            if(i == 0) {
                randomWeights(layers[0]);
            }
            else {
                randomWeights(edges[i-1]);
                for(int j = 0; j < layers[i].size(); ++ j) {
                    for(int k = 0; k < layers[i-1].size(); ++k) {
                        
                        layers[i][j] += (layers[i-1][k] * edges[i-1][layers[i-1].size()*j+k]);
                    }
                //clamp between -1 and 1
               	layers[i][j] = tanh(layers[i][j]);
                }
            }
        }
        totalWeight_= layers[layers.size()-1][0];
    }

    void setNeuralSizes(std::vector<int> &layerSizes, vector<vector<float>> &layers, vector<vector<float>> &edges)
   	{
        layers.resize(layerSizes.size());
        edges.resize(layerSizes.size()-1);

        for(int i=0; i<layers.size(); ++i) {
        	layers[i].resize(layerSizes[i], 0.0f);
        }
        for(int i=0; i<edges.size(); ++i) {
        	edges[i].resize(layers[i].size() * layers[i+1].size());
        }
   	}
    
    void randomWeights(vector<float> & rando) {
        std::random_device rd;
        std::mt19937 random(rd());
        for(int i = 0; i < rando.size(); ++i) {
            uniform_real_distribution<> randomfloats(-1.0, 1.0);
            rando[i] = randomfloats(random);
        }
    }
    
    float getLastWeight(){
   		return totalWeight_;
   	}

private:
    float totalWeight_;  



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
    
    
/*   
private:
    vector<vector<float>> layers;
    vector<vector<float>> edges;
*/  
    
};





#endif /* NeuralNet_Ref_h */
