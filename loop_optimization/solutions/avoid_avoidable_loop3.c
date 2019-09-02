/*
 * authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
 * date      : October 2018
 * copyright : GNU Public License
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include <math.h>




/*
 * The purpose of this code is to show you some incremental
 * optimization in loops.
 * Conceptually, it is a loop over a number Np of particles
 * with tha aim of calculating the distance of each particle
 * from the center of the cells of a grid within a maximum distance
 * Rmax.
 *
 * The grid is actually not allocated.
 */


// look at the lecture about timing for motivations about the following macro
#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)

int main(int argc, char **argv)
{
  
  double           *x, *y, *z, dist;
  double           *Grid, half_size, Rmax;
  int              Np, Ng, i, j, k, p;
  
  
  // timing-related variables
  double          tstart, tstop, ctime;
  struct timespec ts;

  Np = atoi( *(argv + 1) );
  Ng = atoi( *(argv + 2) );
  
  // allocate contiguous memory for particles coordinates
  x    = (double*)calloc(Np * 4, sizeof(double));
  y    = x + Np;
  z    = y + Np;

  // this quantity will be used to calculate the grid
  // center starting from the (corner) grid point
  half_size = 0.5 / Ng;

  // the maximum distance
  Rmax = 0.2;
  
  // initialize random number generator
  //srand48(clock());   // change seed at each call
  srand48(997766);    // same seed to reproduce results

  // initialize mock coordinates in the range [0, 1[
  printf("initialize coordinates..\n");
  for(i = 0; i < 3*Np; i++)
    x[i] = drand48();



  

  // ---------------------------------------------------
  // STEP 3: avoid use of FP division
  // ---------------------------------------------------
  
  printf("LOOP 3 :: "); fflush(stdout);

  double dummy;
  double Rmax2 = Rmax * Rmax;
  double Ng_inv = (double)1.0 / Ng;

  dummy = 0;
  ctime = 0;

  tstart = TCPU_TIME;

  for(p = 0; p < Np; p++)
    for(i = 0; i < Ng; i++)
      for(j = 0; j < Ng; j++)
	    for(k = 0; k < Ng; k++)
	    {
	      double dx, dy, dz;
	      dx = x[p] - (double)i * Ng_inv + half_size; //dx2 = dx2*dx2;
	      dy = y[p] - (double)j * Ng_inv + half_size; //dy2 = dy2*dy2;
	      dz = z[p] - (double)k * Ng_inv + half_size; //dz2 = dz2*dz2;
	    
	      dist = dx*dx + dy*dy + dz*dz;
	    
	      if(dist < Rmax2)
	        dummy += sqrt(dist);
	    }
  
  ctime += TCPU_TIME - tstart;
  printf("\t%g sec [%g]\n", ctime, dummy);
  //printf("\t%g sec [%g]\n", ctime / Iter, dummy / Iter );
  
  free(x);
  
  return 0;
}
