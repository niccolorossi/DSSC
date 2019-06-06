#include <stdio.h>
#define N 8192

__global__ void transpose(const int* a, int* ta, const int size) {
  int ix = blockIdx.x*blockDim.x + threadIdx.x;
  int iy = blockIdx.y*blockDim.y + threadIdx.y; 

  ta[iy+ix*size] = a[ix+iy*size];
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
  block.x = 8;
  block.y = 8;
  grid.x = N / block.x; 
  grid.y = N / block.y;

  for(int i=0; i<N*N; i++)
    h_a[i] = i+1;

  cudaMemcpy(d_a, h_a, num_bytes, cudaMemcpyHostToDevice);

  cudaEventRecord(start);
  transpose<<<grid, block>>>(d_a, td_a, N);
  cudaEventRecord(stop);

  cudaMemcpy(th_a,td_a,num_bytes, cudaMemcpyDeviceToHost);

  int c = check(th_a,h_a,N);
  printf("correctness: %d\n", c);

  free( h_a ); free(th_a);
  cudaFree( d_a ); cudaFree(td_a);

  cudaEventSynchronize(stop);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);
  printf("elapsed time: %f\n", milliseconds);

  return 0;
}

