Note, nvcc cannot compile cpp and cu files at the same time.

Heres how you do it:

nvcc -x cu examplefile.cpp examplefile.cu
