


#include "NeuralNet.h"
#include <iostream>
#include <vector>
using std::vector;

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
                        layers[i][j] += layers[i-1][k];
                    }
                }
            }
        }
        std::cout << layers[layers.size()-1][0] << std::endl;
    }
    
    void randomWeights(vector<float> & rando) {
        for(int i = 0; i < rando.size(); ++i) {
            rando[i] = 1.0;
        }
    }
    
    
    
    
private:
    vector<vector<float>> layers;
    vector<vector<float>> edges;
    
    
};

int main() {
    
    vector<int> y {4, 5, 3, 1};
    Nodes x(y);
    
    return 0;
}





















