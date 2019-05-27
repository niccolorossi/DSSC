// transpose in C

for(i=0; i<N; i++) {
  for(j=0; j<N; j++) {
    mat_in[i][j]=mat_out[j][i];
  }
 }

// parallelize in C

/*
#pragma omp for private (i,j) 
for(i=0; i<N; i++) {
  for(j=i; j<N; j++) {
  }
}
*/

///////////////////////////////////////////

#define NTHS 1024





// transpose in CUDA

// these pointers store memory for input and output matrices, for host and device
double *MAT_IN_h, *MAT_OUT_h, *MAT_IN_D, *MAT_OUT_D;

MAT_IN_h = (double *) malloc(N*N*sizeof(double));
MAT_OUT_h = //same as above;

cudaMalloc(&MAT_IN_d, N*N*sizeof(double));
cudaMalloc(&MAT_OUT_d, N*N*sizeof(double));

INIT(MAT_IN_h); // function that initialises host matrix (in CPU)

cudaMemcpy(MAT_IN_h, MAT_IN_d, N*N*sizeof(int)); // copying memory into device

MAT_TRANSP<<<N_BLOCKS, N_Threads>>(MAT_IN_d, MAT_OUT_d); //1024 max number of threads in a GPU (see #define above)
// you can define
N_BLOCKS = N*N/NTHS;
// blocks enclose portions of matrix 
// they ecompass portions of lines...

Idx = ThreadIdx.x + blockIdx.x*blockDim.x;
//aux variables

x = Idx%N;
y = Idx/N;

mat[y+N*x] = mat[x+N*y]; //or something like that

// another possible solution
// seems like we already have two dimensions; block idx and thread idx

MAT_TRANSP<<N,1024>> (MAT_IN_D, MAT_OUT_D)

//kernel definition

__global__ mat_transp(double * M.in, double * M.out) {
	   y = blockIdx.x;
	   x = threadIdx.x;
	   while(x<N) {

	   	   M.out(y*N +x) = M.in(x*n+y);
	  	   x+=blockDim.x;
		   }


}




// matrix multiplication






