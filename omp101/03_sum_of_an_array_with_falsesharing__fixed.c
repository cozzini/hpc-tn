/* =============================================================================
 * This file is part of the exercises for the Lectures on 
 *   "Parallel COmputing and OpenMP Introduction"
 * given at 
 *   Scientific and High Performance Computing School 2019"
 *   @ Università di Trento
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software (*); you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 *     
 *     (*) also, let me add, has nothing particularly precious in it; in
 *         other words, do what you want with it, with the hope it may
 *         be useful in some way
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>


#define N_default 100

#define CPU_TIME_R (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec +	\
		    (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +	\
		     (double)myts.tv_nsec * 1e-9)

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)

int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;
  
  struct  timespec ts;
  double *array;

  // check whether some arg has been passed on
  if ( argc > 1 )
    N = atoi( *(argv+1) );


  // allocate memory
  if ( (array = (double*)calloc( N, sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
      return 1;
    }

  // just give notice of what will happen and get the number of threads used
#pragma omp parallel
  {
#pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("omp summation with %d threads\n", nthreads );
    }
  }


  // initialize the array
  srand48( time(NULL) );
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = drand48();


  double S[ 8*nthreads ];                                     // this will store the summation's chunks
  double runtime = 0;                                       // this will be the runtime
  
  double tstart  = CPU_TIME;  

  memset( S, 0, nthreads*sizeof(double) );
  
#pragma omp parallel reduction(+:runtime)
  {    
    struct  timespec myts;
    double mystart = CPU_TIME_th;
    int    me      = omp_get_thread_num()*8;
    
#pragma omp for
    for ( int ii = 0; ii < N; ii++ )
	S[me] += array[ii];
    
    runtime += CPU_TIME_th - mystart;
  }

  if ( nthreads > 1 )
    for ( int ii = 1; ii < nthreads; ii++ )
      S[0] += S[ii*8];
  
  double tend = CPU_TIME;

  printf("Sum is %g, process took %g of wall-clock time, <%g> sec of thread-time \n", S[0], tend - tstart, runtime/nthreads );
  
  free( array );
  return 0;
}
