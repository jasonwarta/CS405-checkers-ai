


#include "NeuralNet.h"
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



class Nodes {
public:
    
    Nodes(vector<int> netSize) {
        layers.resize(netSize.size());
        edges.resize(netSize.size());
        for(int i = 0; i < layers.size(); ++i) {
            layers[i].resize(netSize[i], 0.0);
            if(i == 0) {
                randomWeights(layers[0]);
            }
            else {
                edges[i-1].resize(layers[i-1].size() * layers[i].size());
                randomWeights(edges[i-1]);
                for(int j = 0; j < layers[i].size(); ++ j) {
                    for(int k = 0; k < layers[i-1].size(); ++k) {
                        
                        layers[i][j] += (layers[i-1][k] * edges[i-1][layers[i-1].size()*j+k]);
                        //std::cout << "Edge index: " << layers[i-1].size()*j+k << std::endl;
                        //std::cout << "Layers i j: " << i << " " << j << " " << " Math: " << (layers[i-1][k] * edges[i-1][layers[i-1].size()*k+k]) << std::endl;
                    }
                    
                }
            }
        }
        //cout << "edges: " << endl;
        //printvec(edges);
        
        //cout << endl << "layers: " << endl;
        //printvec(layers);
        cout << endl;
        std::cout << layers[layers.size()-1][0] << std::endl;
    }
    
    void randomWeights(vector<float> & rando) {
        std::random_device rd;
        std::mt19937 random(rd());
        for(int i = 0; i < rando.size(); ++i) {
        	//std::setprecision(2);
            uniform_real_distribution<> randomfloats(-1.0, 1.0);
            rando[i] = randomfloats(random);
        }
    }
    
    void printvec(vector<vector<float>> & x) {
        for(int i = 0; i < x.size(); ++i) {
            for(int j=0; j < x[i].size(); ++j) {
                std::cout << x[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    
    
    
    
private:
    vector<vector<float>> layers;
    vector<vector<float>> edges;
    
    
};





int main() {
    
    vector<int> y {1000,1000,10001,420,420,420,1};
    Nodes x(y);
    
    
    return 0;
}





















