#include <stdio.h>
#define N 8192
#define BLOCK_DIM_X 8
#define BLOCK_DIM_Y 8
#define NUM_BLOCKS_X N/BLOCK_DIM_X
#define NUM_BLOCKS_Y N/BLOCK_DIM_Y

__global__ void fast_transpose(const int* a, int* ta, int size) {
  int ix = blockIdx.x*blockDim.x + threadIdx.x;
  int iy = blockIdx.y*blockDim.y + threadIdx.y; 

   __shared__ int tmp[BLOCK_DIM_Y][BLOCK_DIM_X];

  tmp[threadIdx.y][threadIdx.x] = a[ix*size + iy];
  __syncthreads();

  ta[iy*size + ix] = tmp[threadIdx.y][threadIdx.x];

}

int check(const int* a, const int* ta, const int size) {
  int k=0;
  for(int i=0; i<size; i++) {
    for(int j=0; j<size; j++) {
      if(ta[j+i*size] != a[i+j*size]) break;
      else k++;
    }
  }

  if(k==size*size) return 1;
  else return 0;
}



int main() {

  int num_bytes = N*N*sizeof(int);

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  int *d_a=0, *h_a=0; // device and host pointers
  int *td_a=0, *th_a=0;
  h_a = (int*)malloc(num_bytes);
  th_a = (int*)malloc(num_bytes);
  cudaMalloc( (void**)&d_a, num_bytes );
  cudaMalloc( (void**)&td_a, num_bytes );

  dim3 grid, block; 
  block.x = BLOCK_DIM_X;
  block.y = BLOCK_DIM_Y;
  grid.x = NUM_BLOCKS_X; 
  grid.y = NUM_BLOCKS_Y;

  for(int i=0; i<N*N; i++)
    h_a[i] = i+1;

  cudaMemcpy(d_a, h_a, num_bytes, cudaMemcpyHostToDevice);

  cudaEventRecord(start);
  fast_transpose<<<grid, block>>>(d_a, td_a, N);
  cudaEventRecord(stop);

  cudaMemcpy(th_a,td_a,num_bytes, cudaMemcpyDeviceToHost);

  int c = check(th_a,h_a,N);
  printf("correctness: %d\n", c);
  cudaEventSynchronize(stop);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);
  printf("elapsed time: %f\n", milliseconds);

  free( h_a ); free(th_a);
  cudaFree( d_a ); cudaFree(td_a);

  return 0;
}
