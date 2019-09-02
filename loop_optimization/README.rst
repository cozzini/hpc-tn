Loop optmization 
=================

This exercise shows a series of incremental optimization to be applied to a sample code.

The code considers ``Np`` particles in 3D, and calculates the distances between each particle and a set of grid points, accumilating the value on if the 
distance from the current grid point is below a certain threashold.
The main loop will be written naively ``avoid_avoidable_loop0.c``
::

  for(p = 0; p < Np; p++)
    for(i = 0; i < Ng; i++)
      for(j = 0; j < Ng; j++)
        for(k = 0; k < Ng; k++)
          {
          dist = sqrt( pow(x[p] - (double)i/Ng + half_size, 2) +
                       pow(y[p] - (double)j/Ng + half_size, 2) +
                       pow(z[p] - (double)k/Ng + half_size, 2) );
          if(dist < Rmax)
              dummy += dist;
          }

First log into a compute node via ``qsub``
::

  qsub -q jrc -l walltime=2:00:00 -l nodes=1:ppn=24 -I  

Load the intel/18.0 module 
::

  module load intel/18.0

To compile
::

  icc -O3 avoid_avoidable_loop0.c -o avoid_avoidable_loop0.x

Then run for example with 1000 particles and 200 grid points per axes
::

  ./avoid_avoidable_loop0.x 1000 200

1. Looking at the code, the first optimization is to avoid computing the square root of every particle-gridpoint couple, but to calculate it only 
   when the distance is below cutoff. To this end, we add two new variables called ``dist2`` and ``Rmax2 = Rmax * Rmax``. Then we move the ``sqrt`` inside the if, which should be now
   ::
 
     if(dist2 < Rmax2)

   Compile and check the performance. 

2. Another optimization would be to remove the ``pow`` calls and substitute them with ``dx*dx`` and so on. This idea actually has some interesting results, depending on the compiler and architecture. 
   Compile and check the performance. Compare different compilers, compiler versions and architectures. 

3. The next observation is that floating point divisions are hard, at least harder then multiplications, so they should be avoided if possible. In this case, the idea is to create a new var 
   ``Nginv = 1/Ng`` and substitute the division with a multiplication in the inner loops. 

4. The final step is to try the manual unrolling of the inner loop. This is more tricky. First it is better to move x and y calculation as up as possible in the nested loop.
   ::

     for(p = 0; p < Np; p++)
       for(i = 0; i < Ng; i++)
        {
        double dx = x[p] - (double)i * Ng_inv + half_size;
   
   and similarly for y.
   The we unroll the k loop by 4
   ::

          for(k = 0; k < Ng; k+=4)
              {
              double dz[4], dist2[4];
              dz[0] = z[p] - (double)k * Ng_inv + half_size; //dz2 = dz2*dz2;
              dz[1] = z[p] - (double)(k+1) * Ng_inv + half_size;
              ...
              dist2[0] = dx*dx + dy*dy + dz[0]*dz[0];

              for(c = 0; c < 4; c++)
                if( dist2[c] < Rmax2 )
                  dummy += sqrt(dist2[c]);

   Compile and analyze the performance.


Exercises
===========


- Enabling vector report in the intel compiler (using the ``-opt-report`` flag at compilation time) you will find in .optrpt file that, at least for lower level of optimization, 
  the compiler is able to vectorize the inner loop. Estimate the effect of the vectorization, by comparing vectorized and non-vectorized versions (using the ``-no-vec`` flag).

- Repeat the analysis for gnu/7.2.0. To disable the vectorization, use the ``fno-tree-vectorize`` flag.
