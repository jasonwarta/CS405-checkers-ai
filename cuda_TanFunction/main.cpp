#include "tan.h"
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

int main() {

  int number = 10;
	size_t size = number * sizeof(int);

  // Allocate memory for host
	float *n_A = (float*)malloc(size);
	float *n_B = (float*)malloc(size);

  // Set logic for it to compute
	*n_A = 1.0;
	*n_B = 2.0;

  // Allocate memory for device
	float *d_A;
	cudaMalloc(&d_A, size);
	float *d_B;
	cudaMalloc(&d_B, size);

  // Copy from host to device
	cudaMemcpy(d_A, n_A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, n_B, size, cudaMemcpyHostToDevice);

  // Call the function
	cuda_TanFunction<<<(1<<20),1>>>(d_A);
	cudaDeviceSynchronize();

  // Copy from device to host
	cudaMemcpy(n_A, d_A, size, cudaMemcpyDeviceToHost);

  // Print what has been copied from device
	float buffer = *n_A;
	cout << buffer << endl;

  // Free memory
	cudaFree(d_A);
	cudaFree(d_B);

	delete n_A;
	delete n_B;

  return 0;
}
