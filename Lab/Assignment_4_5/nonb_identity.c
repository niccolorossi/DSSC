#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 10 // matrix dimension

void print_matrix(int* A, int rows, FILE* p) {
  size_t i,j=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<N; j++) {
      fprintf(p, "%d\t", *(A+j+i*N));
    }
    fprintf(p, "\n");
  }
}

void swap(int** a, int** b)
{
  int* t = *a;
  *a = *b;
  *b = t;
}


int main(int argc, char* argv[])
{

  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes

  MPI_Init(&argc, &argv);
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  
  int N_LOC = N/npes;
  int REST = N%npes;
  size_t gap = 0;
  size_t i,j = 0;

  if(rank < REST) N_LOC++;

  int* mat =(int*)malloc(N*N_LOC*sizeof(int));

  if(rank < REST) {
    gap = rank*N_LOC;
  } else {
    gap = REST*(N_LOC+1)+(rank-REST)*N_LOC;
  }

  for(i=0; i<N_LOC; i++) {
    for(j=0; j<N; j++) {
      if (j == i+gap) *(mat+j+i*N) = 1;
      else *(mat+j+i*N) = 0;
    }
  }

  if(!rank) {

    FILE* fp = fopen("matrix.bin", "wb");
    if (N < 10) fp = stdout;

    int* buf = (int*)malloc(N*N_LOC*sizeof(int));

    MPI_Request request;

    if(REST) {
      for(i=1; i<REST; i++) {
	MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC, fp);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	swap(&mat,&buf);
      }

      MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,REST,101,MPI_COMM_WORLD,&request);
      print_matrix(mat,N_LOC, fp);
      MPI_Wait(&request,MPI_STATUS_IGNORE);
      swap(&mat,&buf);

      for(i=REST+1; i<npes; i++) {
	MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC-1, fp);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	swap(&mat,&buf);
      }
      print_matrix(mat,N_LOC-1, fp);
    } 
  
    else {
      for(i=1; i<npes; i++) {
        MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
        print_matrix(mat,N_LOC, fp);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	swap(&mat,&buf);
      }
      print_matrix(mat,N_LOC, fp);
    }
    free(buf);
  }

  else {
    MPI_Send(mat,N_LOC*N,MPI_INT,0,101,MPI_COMM_WORLD);
  }


  MPI_Finalize();

  free(mat);
  return 0;
}
