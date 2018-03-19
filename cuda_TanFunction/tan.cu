#include "tan.h"

__global__
void cuda_TanFunction(float *number) {
	atanh(*number);
}
