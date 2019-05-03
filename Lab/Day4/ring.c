#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>


int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  int x = rank;
  int xlow = 0;
  int sum=0;
  
  if(rank != 0) {
    MPI_Recv(&xlow, 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    x += xlow;
  }

  MPI_Send(&x, 1, MPI_INT, (rank+1)%npes, rank, MPI_COMM_WORLD); 

  if(rank == 0) {
    MPI_Recv(&xlow, 1, MPI_INT, npes-1, npes-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d\n", xlow);
  }

  






  MPI_Finalize();
  return 0;
}
