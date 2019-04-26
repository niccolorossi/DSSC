#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

int **allocate_matrix(int rows, int cols)
{

  int **A=(int **)malloc(sizeof(int *)*rows);
  int i=0;
  for (i=0; i<rows; i++) 
  {
    A[i]=(int *)malloc(sizeof(int)*cols);
  }

  return A;
}

void deallocate_matrix(int **A, int rows)
{
  int i=0;
  for (i=0; i<rows; i++) {
    free(A[i]);
  }

  free(A);
}

void print_matrix(int** A, int rows, int cols)
{
  int i,j=0;
  for(i=0; i<rows; i++)
    {
      for(j=0; j<cols; j++)
	{
	  printf("%d\t", A[i][j]);
        }
      printf("\n");
    }
}









int main(int argc, char* argv[])
{

  int N = 10; //dimension of the matrix
  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  int N_LOC = N/npes; 
  int** mat = allocate_matrix(N_LOC,N);

  int i,j = 0;

  for(i=0; i<N_LOC; i++)
  {
    for(j=0; j<N; j++)
    {
      if (j == i+rank*N_LOC) mat[i][j] = 1;
      else mat[i][j] = 0;
    }
  }

  if(rank==1)
    {
      int* send = &mat[0][0];
      MPI_Send(send,N_LOC*N+N_LOC,MPI_INT,0,101,MPI_COMM_WORLD);
    }
  
  if(rank==0)
    {
      int** aux = allocate_matrix(N_LOC,N);
      MPI_Recv(&aux[0][0],N_LOC*N+N_LOC,MPI_INT,1,101,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      print_matrix(aux,N_LOC,N);
      deallocate_matrix(aux, N_LOC);
    }
  
  deallocate_matrix(mat, N_LOC);
  MPI_Finalize();
  return 0;
}
