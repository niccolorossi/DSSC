## CUDA Exercises
This folder contains the code and the results relative to the last assignment of the course (matrix transpose with CUDA).
fast_transpose_8192.cu and naive_transpose both perform the transpose of an int matrix of size 8192 x 8192, but in different ways: the kernel implemented for the "fast" algorithm saves in a temporary matrix located in the shared GPU memory "blocks" of the original matrix, and then, after threads synchronization, copies back from this shared area in transposed order.

The correctness of this algorithms has been tested via the function check(), and the time spent inside of the kernel (in milliseconds) has been measured via the CUDA function CudaEventRecord(). The results are shown in the files fast_t_results.txt and naive_t_results.txt. The performance of the fast algorithm improves when block size diminishes, eventually outperforming the naive version.

For the fast algorithm, other sizes have been tested as well, and the results are in the file fast_t_results.txt.

In order to generate the executables it is sufficient to load the module cudatoolkit/10.0 and type nvcc <filename.cu>.
