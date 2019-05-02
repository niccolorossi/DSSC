#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>


int main(int argc, char* argv[]) {

  int rank = 0;
  int npes = 1;
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  // int i,j=0;
  int x = rank;
  int xlow = 0;
  // int xright=0;
  int sum=0;

  /*
  for(i=1; i<=npes; i++) {
    if(rank==npes-i) MPI_Send(&x, 1, MPI_INT, npes-(i+1), npes-i, MPI_COMM_WORLD);
    if(rank==npes-(i+1)) {
      MPI_Recv(&xright, 1, MPI_INT, npes-i, npes-i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      sum += xright;
      x += sum;
      printf("after process %d sent %d the values are:\n", npes-i, xright);
      printf("sum: %d\nx: %d\n", sum, x);
    }
  }
  */

  // if(rank==0) printf("%d\n", sum);
  
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
