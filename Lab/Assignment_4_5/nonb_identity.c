#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 10  // matrix dimension

void print_matrix(int A[][N], int rows) {
  size_t i,j=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<N; j++) {
      fprintf(stdout, "%d\t", A[i][j]);
    }
    fprintf(stdout, "\n");
  }
}

void copy(int m1[][N], int m2[][N], int rows)
{
  size_t i,j=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<N; j++) {
      m1[i][j] = m2[i][j];
    }
  }
}


int main(int argc, char* argv[])
{

  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  
  int N_LOC = N/npes;
  int REST = N%npes;
  size_t gap = 0;
  size_t i,j = 0;

  if(rank < REST) N_LOC++;

  int mat[N_LOC][N];

  if(rank < REST) {
    gap = rank*N_LOC;
  } else {
    gap = REST*(N_LOC+1)+(rank-REST)*N_LOC;
  }

  for(i=0; i<N_LOC; i++) {
    for(j=0; j<N; j++) {
      if (j == i+gap) mat[i][j] = 1;
      else mat[i][j] = 0;
    }
  }

  if(!rank) {

    int buf[N_LOC][N];
    MPI_Request request;

    if(REST) {
      for(i=1; i<REST; i++) {
	MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	copy(mat,buf,N_LOC);
      }

      MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,REST,101,MPI_COMM_WORLD,&request);
      print_matrix(mat,N_LOC);
      MPI_Wait(&request,MPI_STATUS_IGNORE);
      copy(mat,buf,N_LOC-1);

      for(i=REST+1; i<npes; i++) {
	MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC-1);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	copy(mat,buf,N_LOC-1);
      }
      print_matrix(mat,N_LOC-1);
    } 
  
    else {
      for(i=1; i<npes; i++) {
        MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
        print_matrix(mat,N_LOC);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	copy(mat,buf,N_LOC);
      }
      print_matrix(mat,N_LOC);
    }

  }

  else {
    MPI_Send(mat,N_LOC*N,MPI_INT,0,101,MPI_COMM_WORLD);
  }


  MPI_Finalize();
  return 0;
}
