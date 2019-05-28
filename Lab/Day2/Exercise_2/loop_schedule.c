#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

void print_usage( int * a, int N, int nthreads ) {

  int tid, i;
  for( tid = 0; tid < nthreads; ++tid ) {

    fprintf( stdout, "%d: ", tid );

    for( i = 0; i < N; ++i ) {

      if( a[ i ] == tid) fprintf( stdout, "*" );
      else fprintf( stdout, " ");
    }
    printf("\n\n");
  }
}

int main( int argc, char * argv[] ) {

  const int N = 250;
  int a[N];
  int thread_id = 0;
  int nthreads = 1;
  int i=0;

  #pragma omp parallel private(thread_id, i)
    {
    nthreads = omp_get_num_threads();
    thread_id = omp_get_thread_num();

    // dynamic scheduling, chunks of size 10
    #pragma omp for schedule(dynamic,10)
      for( i = 0; i < N; i++ ){
        a[i]=thread_id;
      }

    // dynamic scheduling, chunks of size 1
    /*
    #pragma omp for schedule(dynamic,1)
      for( i = 0; i < N; i++ ){
        a[i]=thread_id;
      }
    */
    // dynamic scheduling, default chunk size
    /*
    #pragma omp for schedule(dynamic)
      for( i = 0; i < N; i++ ){
        a[i]=thread_id;
      }
    */
    // static scheduling, chunks of size 10
    /*
    #pragma omp for schedule(static,10)
      for( i = 0; i < N; i++ ){
        a[i]=thread_id;
      }
    */
    // static scheduling, chunks of size 1
    /*
    #pragma omp for schedule(static,1)
      for( i = 0; i < N; i++ ){
        a[i]=thread_id;
    }
    */
    // static scheduling, default chunk size
    /*
    #pragma omp for schedule(static)
      for( i = 0; i < N; i++ ) {
        a[i]=thread_id;
      }
    */
    }

  print_usage(a, N, nthreads);

  return 0;
}
