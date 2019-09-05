#include <stdio.h>
#include <stdlib.h>
// if you don ' t have drand48 uncomment the following two lines 10
// #define drand48 1.0/RANDMAXrand
// #define srand48 srand
#define SEED 68111 // seed for number generator

int main (int argc, char ** argv) {

  long long int M = 0 ;
  double pi = 0;
  // point coordinates
  double x , y;

  if (argc<2)
    {
      printf(" Usage: %s number_of_iterations \n",argv[0]);
      return 1;
    }

  long long int N = atoll(argv[1]);

  srand48(SEED) ; // seed the number generator

  unsigned int i;
  for (i = 0 ; i < N ; i++)
    {
      // take a point P(x,y) inside the unit square
      x = drand48(); 
      y = drand48();
      
      // check if the point P(x,y) is inside the circle
      if ((x*x + y*y)<1)
	M++; 
    }
  pi = 4.0*M/N ; // calculate area

  printf ( "\n # of trials = %llu , estimate of pi is %1.9f \n", N, pi ) ;
  return 0;
}
