#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10  // number of elements in the buffer

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

void fill_vector(int* v, int x) {
  size_t i;
  for(i = 0; i<N; i++)
    v[i] = x;  
}

int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  int i=0;
  int v[N];
  fill_vector(v,rank);
  int rec[N];
  fill_vector(rec,0);

  if(rank != 0) {
    MPI_Recv(rec, N, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    vector_sum(v,rec);
  }

  MPI_Send(v, N, MPI_INT, (rank+1)%npes, rank, MPI_COMM_WORLD);

  if(rank==0) {
    MPI_Recv(rec, N, MPI_INT, npes-1, npes-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    print_vector(rec); 
  }

  MPI_Finalize();
  return 0;
}
