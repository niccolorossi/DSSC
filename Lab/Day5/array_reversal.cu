#include <stdio.h>
#include <math.h>

#define N (2048*2048)
#define THREAD_PER_BLOCK 512

__global__ void reverse( int *a, int *b ) {
       int index=threadIdx.x+blockIdx.x*blockDim.x;
       b[index] = a[N-index];
}

void random_ints(int *p, int n) {
	int i;
	for(i=0; i<n; i++) {
		p[i]=rand();
	}
}

int main( void ) {
    int *a, *b, *c;               // host copies of a, b, c
    int *dev_a, *dev_b;           // device copies of a, b, c
    int size = N * sizeof( int ); // we need space for N   									// integers
    int i;

    // allocate device copies of a, b, c
    cudaMalloc( (void**)&dev_a, size );
    cudaMalloc( (void**)&dev_b, size );

    a = (int*)malloc( size ); 
    b = (int*)malloc( size );
    c = (int*)malloc( size );

    random_ints( a, N ); 
    // copy inputs to device
    cudaMemcpy( dev_a, a, size, cudaMemcpyHostToDevice );

    // launch an add() kernel with N threads
    reverse<<< N/THREAD_PER_BLOCK, THREAD_PER_BLOCK >>>( dev_a, dev_b);

    // copy device result back to host copy of c
    cudaMemcpy( b, dev_b, size,   cudaMemcpyDeviceToHost );

    for(i=0; i<N; i++) {
	c[i]=a[N-i];
	if(b[i]!=c[i]) {
	   printf("error: expected %d, got %d!\n",c[i], b[i]);
	   break;
	}
    }
    if(i==N) {
	   printf("correct!\n");
    }    

 
    free( a ); free( b ); free( c );
    cudaFree( dev_a );
    cudaFree( dev_b );
    return 0;
}
