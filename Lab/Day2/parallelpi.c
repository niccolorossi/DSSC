#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

double f(double x)
{
  return 1/(1+x*x);
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

  local_result = 4*h*local_result;

  return local_result;
}

int main(int argv, char* argc[])
{

  int N=1000000000;
  double a=0.0;
  double b=1.0;
  double global_result=0.0;
  int nthreads=1;
  double b_time=omp_get_wtime();

  #pragma omp parallel
  {

    nthreads=omp_get_num_threads();
    int tid=omp_get_thread_num();
    int local_n=N/nthreads;
    double h=(b-a)/N;
    double local_a=a+tid*local_n*h;
    double local_b=local_a+local_n*h;
    double local_result=local_sum(local_a,local_b,local_n,h);

    #pragma omp atomic
      global_result += local_result;
  }

  double e_time = omp_get_wtime();

  //printf("The result is %lf\n",global_result);
  //printf("The computation took %lf seconds\n", e_time-b_time);
  printf("%d %lf\n", nthreads, e_time-b_time);
  return 0;
}
