     
#include <random>
#include <vector>
#include <iostream>


void printVec(std::vector<int> &printMe)
{
    std::cout << std::endl << "-------------------------" << std::endl;
    for(int i=0; i<printMe.size(); ++i)
    {
        std::cout << printMe[i] << " ";
    }
    std::cout << std::endl;
}


int main()
{
    const int numRuns = 100000;
    const int sizeVec = 100;

    std::random_device rd;
    std::mt19937 random(rd());
    std::default_random_engine generator(rd());

    std::vector<int> resultsUniform(sizeVec);
    std::uniform_real_distribution<> uniformRando(0.0, 999.0);

    for(unsigned int i=0; i<numRuns; ++i)
    {
        resultsUniform[(int)uniformRando(generator)]++;
    }

    std::vector<int> resultsNormal(sizeVec);
    std::normal_distribution<float> distribute(0.0, 1.0);
    for(int i=0; i<numRuns; ++i)
    {
        resultsNormal[(int)distribute(generator)+(sizeVec/2)]++;
    }

    printVec(resultsUniform);
    printVec(resultsNormal);
}