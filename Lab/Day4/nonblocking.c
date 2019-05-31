#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 11  // matrix dimension

// routine that prints a matrix on stdout
void print_matrix(int A[][N], const size_t rows, FILE* fp) {
  size_t i,j=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<N; j++) {
      fprintf(fp, "%d\t", A[i][j]);
    }
    fprintf(fp, "\n");
  }
}

// routine to copy matrix m2 in m1
void copy(int m1[][N], int m2[][N], const size_t rows) {
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
  size_t gap = 0; // used for the inizialization of the Id matrix
  size_t i,j = 0;

  if(rank < REST) N_LOC++; // the first "REST" processes receive a portion of the matrix of size (N/npes+1) x N

  int mat[N_LOC][N]; // first buffer (present on each process with different sizes as explained above)

  if(rank < REST) {
    gap = rank*N_LOC;
  } else {
    gap = REST*(N_LOC+1)+(rank-REST)*N_LOC;
  }
  
  // Id matrix initialization
  for(i=0; i<N_LOC; i++) {
    for(j=0; j<N; j++) {
      if (j == i+gap) mat[i][j] = 1;
      else mat[i][j] = 0;
    }
  }

  if(!rank) {

    int buf[N_LOC][N]; // define second buffer if rank>0
    MPI_Request request;

    FILE* fp;
    if(N>10) {
      fp = fopen("matrix.dat", "wb");
    }
    else {
      fp = stdout;
    }

    // this is executed only if REST !=0
    // needed because of different chunk sizes 
    if(REST) {
      for(i=1; i<REST; i++) {
	MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC,fp);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	copy(mat,buf,N_LOC);
      }

      MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,REST,101,MPI_COMM_WORLD,&request);
      print_matrix(mat,N_LOC,fp);
      MPI_Wait(&request,MPI_STATUS_IGNORE);
      copy(mat,buf,N_LOC-1);

      for(i=REST+1; i<npes; i++) {
	MPI_Irecv(buf,(N_LOC-1)*N,MPI_INT,i,101,MPI_COMM_WORLD,&request);
	print_matrix(mat,N_LOC-1,fp);
	MPI_Wait(&request,MPI_STATUS_IGNORE);
	copy(mat,buf,N_LOC-1);
      }
      print_matrix(mat,N_LOC-1,fp);
    } 
  
    else {
      for(i=1; i<npes; i++) {
        MPI_Irecv(buf,N_LOC*N,MPI_INT,i,101,MPI_COMM_WORLD,&request); // rank 0 process receives chunk of i-th process (from 1 to npes-1)
        print_matrix(mat,N_LOC,fp); // root prints the chunk currently saved as "mat"
	MPI_Wait(&request,MPI_STATUS_IGNORE); // root waits until comm is over
	copy(mat,buf,N_LOC); // root transfers received buffer into mat
      }
      print_matrix(mat,N_LOC,fp);
    }

  }

  else {
    MPI_Send(mat,N_LOC*N,MPI_INT,0,101,MPI_COMM_WORLD); // everyone sends their chunk of Id to root process (0)
  }


  MPI_Finalize();
  return 0;
}
