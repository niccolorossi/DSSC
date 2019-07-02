#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10000000  // number of elements in the buffer

int left(const int rank, const int npes) {
  return (npes+rank-1)%npes;
}

int right(const int rank, const int npes) {
  return (rank+1)%npes;
}

void vector_sum(int* v1, int* v2) {
  size_t i=0;
  for(i=0; i<N; i++) {
    v1[i] += v2[i];
  }
}

void print_vector(int* v) {
  size_t i;
  for(i=0; i<N; i++) {
    printf("%d\t", v[i]);
  }
  printf("\n");
}

void fill_vector(int* v, const int x) {
  size_t i;
  for(i = 0; i<N; i++)
    v[i] = x;  
}

void swap(int** a, int** b) {
  int* tmp = *a;
  *a = *b;
  *b = tmp;
}

int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  double ti = MPI_Wtime();
  int i=0;

  int* v = (int*)malloc(N * sizeof(int));
  fill_vector(v,rank);                    // v is initialized with all the elements equal to the process' rank
                                          // it is the buffer sent around in the ring

  int* sum = (int*)malloc(N * sizeof(int));  // for each process, sum contains the vector sum of v and all the vectors sent around so far 
  fill_vector(sum,rank);  

  int* rec = (int*)malloc(N * sizeof(int)); // receiving buffer

  for(i=0; i<npes-1; i++) { // at every loop iteration each process receives from left, updates sum, swaps v with rec so that at the next iteration 
		            // the buffer passed to right process will contain current left proc rank
    if(rank != 0) {
      MPI_Recv(rec, N, MPI_INT, left(rank,npes), 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(v, N, MPI_INT, right(rank, npes), 101, MPI_COMM_WORLD);
      vector_sum(sum,rec);
      swap(&v,&rec);
    } else {
      MPI_Send(v, N, MPI_INT, right(rank,npes), 101, MPI_COMM_WORLD);
      MPI_Recv(rec, N, MPI_INT, left(rank,npes), 101, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      vector_sum(sum,rec);
      swap(&v,&rec);
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  
  double tf = MPI_Wtime();
  if(!rank) {
    printf("elapsed time: %lf\n",tf-ti);
  }

  MPI_Finalize();
  free(v); free(rec); free(sum);
  return 0;
}
