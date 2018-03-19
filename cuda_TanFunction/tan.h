#ifndef TAN_H
#define TAN_H
#include <math.h>
#include <iostream>
#include <vector>
using std::vector;
#include <cuda.h>
#include <cuda_runtime_api.h>

__global__
void cuda_TanFunction(float *number);

#endif //TAN_H
