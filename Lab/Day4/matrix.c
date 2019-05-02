#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>


int main(int argc, char* argv[])
{

  int N = 9; //dimension of the matrix
  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );
  
  int N_LOC = N/npes;
  int REST = N%npes;
  int sendcounts[npes];
  int displs[npes];
  int i,j =0;
 
  for(i=0; i<npes; i++) {
    if(i < REST) {
      sendcounts[i] = (N_LOC+1)*N;
    }
    else sendcounts[i] = N_LOC*N;
  }
  int rows = sendcounts[rank]/N;

  displs[0] = 0;
  for(i=1; i<npes; i++) {
    displs[i] = displs[i-1] + sendcounts[i-1];
  }

  int gap = 0;
  int mat[rows][N];

  for (i=1; i<=rank; i++) {
    gap += sendcounts[i-1]/N;
  }

  for(i=0; i<rows; i++) {
    for(j=0; j<N; j++) {
      if (j == i+gap) mat[i][j] = 1;
      else mat[i][j] = 0;
    }
  }

  if(N>=10) {
    MPI_Offset offset = sizeof(int)*displs[rank];
    MPI_File file;
    MPI_Status status;

    MPI_File_open(MPI_COMM_WORLD, "matrixpar.bin", MPI_MODE_CREATE|MPI_MODE_WRONLY,
		MPI_INFO_NULL, &file);

    MPI_File_seek(file, offset, MPI_SEEK_SET);
    MPI_File_write(file, mat, sendcounts[rank], MPI_INT, &status);
    MPI_File_close(&file);
  } else {
    int res[N][N];
    MPI_Gatherv(mat, sendcounts[rank], MPI_INT, res, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank==0) {
      for(i=0; i<N; i++) {
	for(j=0; j<N; j++) {
	  fprintf(stdout,"%d ", res[i][j]);
	}
	fprintf(stdout,"\n");
      }
    }
  }

  MPI_Finalize();
  return 0;
}
