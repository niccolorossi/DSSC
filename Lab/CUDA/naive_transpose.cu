#include <stdio.h>

__global__ void transpose(int* a, int* ta, int size) {
  int ix = blockIdx.x*blockDim.x + threadIdx.x;
  int iy = blockIdx.y*blockDim.y + threadIdx.y; 

  ta[ix+iy*size] = a[iy+ix*size];
}


int main() {

  int size = 16;
  int num_bytes = size*size*sizeof(int);

  int *d_a=0, *h_a=0; // device and host pointers
  int *td_a=0, *th_a=0;
  h_a = (int*)malloc(num_bytes);
  th_a = (int*)malloc(num_bytes);
  cudaMalloc( (void**)&d_a, num_bytes );
  cudaMalloc( (void**)&td_a, num_bytes );

  dim3 grid, block; 
  block.x = 4;
  block.y = 4;
  grid.x = size / block.x; 
  grid.y = size / block.y;

  for(int i=0; i<size*size; i++)
    h_a[i] = i+1;

  cudaMemcpy(d_a, h_a, num_bytes, cudaMemcpyHostToDevice);

  transpose<<<grid, block>>>(d_a, td_a, size);
  cudaMemcpy(th_a,td_a,num_bytes, cudaMemcpyDeviceToHost);

  for(int row=0; row<size; row++) { 
    for(int col=0; col<size; col++) {
      printf("%d ", th_a[row*size+col] ); printf("\n");
    }
  }

  free( h_a ); free(th_a);
  cudaFree( d_a ); cudaFree(td_a);

  return 0;
}

