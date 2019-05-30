#include <stdlib.h>
#include <stdio.h>

#include <time.h>

double get_execution_time(const struct timespec b_time,
                          const struct timespec e_time)
{
  return (e_time.tv_sec-b_time.tv_sec) +
    (e_time.tv_nsec-b_time.tv_nsec)/1E9;
}

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
 
  struct timespec b_time, e_time;
  clock_gettime(CLOCK_REALTIME, &b_time);  
  double h=(b-a)/N;
   
  double ls = local_sum(a,b,N,h);

  clock_gettime(CLOCK_REALTIME, &e_time);
  printf("%d\n", get_execution_time(b_time,e_time));
  printf("and the result is %lf\n",ls);
  
 
  return 0;
}
