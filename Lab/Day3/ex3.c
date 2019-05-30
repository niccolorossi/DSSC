#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

double f(double x)
{
  return 1./(1.+x*x);
}

double local_sum(double local_a, double local_b, int local_n, double h)
{
  int i=0;
  double local_result=0.0;
  double x_i=0.0;

  for(i=0; i<local_n; ++i)
    {
      x_i = local_a + i*h + h/2;
      local_result += f(x_i);
    }

  local_result = 4.*h*local_result;

  return local_result;
}

int main(int argc, char * argv[])
{
  int N=1000000000;
  double a=0.0;
  double b=1.0;

  int rank = 0; // store the MPI identifier of the process
  int npes = 1; // store the number of MPI processes
  double t1, t2;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &npes );

  t1 = MPI_Wtime();

  double global_result=0.0;
  double ls=0.0;
  int local_n = N/npes;
  double h=(b-a)/N;
  double local_a=a+rank*local_n*h;
  double local_b=local_a+local_n*h;
  
  ls = local_sum(local_a,local_b,local_n,h);

  MPI_Reduce(&ls,&global_result,1,MPI_DOUBLE,MPI_SUM,npes-1,MPI_COMM_WORLD);
  
  t2 = MPI_Wtime();
  
  if(npes > 1 && rank == npes-1)
  {
    MPI_Send(&global_result,1,MPI_DOUBLE,0,101,MPI_COMM_WORLD);
  }
  
  if(npes > 1 && rank == 0)
  {
    MPI_Recv(&global_result,1,MPI_DOUBLE,npes-1,101,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf( "%d\t%f\n", npes, t2 - t1 );
    printf("and the result is %f\n",global_result);
  } else if(rank == 0) {
    printf( "%d\t%f\n", npes, t2 - t1 );
    printf("and the result is %f\n",global_result);
  }
 
  MPI_Finalize();
  return 0;
}
