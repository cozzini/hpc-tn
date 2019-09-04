/* =============================================================================
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software; you can redistribute it and/or modify
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
 */




/*
 * DEBUGGING exercises serie.
 *
 * This small codes contain an error. Sometimes their results are not the
 * expected ones, sometimes the code chrashes.
 * You should be able to understand what's wrong using a debugger.
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main ( int argc, char **argv )
{

  double *array;
  int     N;


  if ( argc == 1 )
    {
      printf ( "you should provide me with a unique argument: the number of array components.\n" );
      return 1;
    }

  N = atoi ( *(argv+1) );

  if ( (array = (double*)calloc( N, sizeof(double) )) == NULL )
    {
      printf ( "not enough memory to allocate %5.3e MB\n", (double)N * sizeof(double) / (1024*1024) );
      return 2;
    }

  printf ( " well ok, now enter each number, one per line:\n" );

  int i = 0;

  while ( i < N )
    {
      if ( scanf ( "%g\n", &array[i++] ) < 1 )
	{
	  printf("\tunknown error: last entry has been discarded\n");
	  i--;
	}
    }


  double avg = 0;
  for ( i = 0; i < N; i++ )
    avg += array[i];

  printf ( "the average is: %g\n", avg / N );
  
  free ( array );

  return 0;
}
