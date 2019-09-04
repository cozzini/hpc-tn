Matrix-matrix multiplication optimization
===========================================

In this tutorial we will analise different strategies to optmize a matrix-matrix multiplication.
The code is in the file ``matmul.F90``.

The basic matrix-matrix multiplication routine is in function ``mat_mult`` and implements the basic algortihm 
::

    do i = 1,n
        do j = 1,o
            do k = 1,m
                c(i,j) = c(i,j) + a(i,k) * b(k,j)
            end do
        end do
    end do

Let's compile and analyse the performance. 
::

  module load intel/18.0
  ifort -O0 matmul.F90 -o matmul.x
  ./matmul.x 2000 2000 2000


The performance are remarkably bad, let's try to make something better. The first observation is that the order of the loops forces 
memory access that are non-optimal. Indeed, we accessing matrix b efficiently (since the inner loop runs on k, so we are accessing the columns of 
matrix b, hwich is good in fortran) but matrix a and c are accessed by row in the k and j loop respectively. 


::

    do k = 1,m
        do j = 1,o
            do i = 1,n
                c(i,j) = c(i,j) + a(i,k) * b(k,j)
            end do
        end do
    end do


To enable the optimized version, uncomment lines 44 to 48, You should already be seeing a remarkable speed up. 
To speedup even more, you can now use higher optimization flags ``-O2`` or ``-O3``. Switch on the optimization report.
::

   ifort -O3 -qopt-report matmul.F90 -o matmul.x

checking the ``matmul.optrpt`` file, you should see a number of things: first, that the compiler tries to switch the loop order on his own in the non-optimized version,
moreover, several optmization takes place: vectorization, loop unrolling to quote a few. The performance are now greatly increased. 

The reason why we showed an explict permuted loop is that the compiler may not always be able to permute the loop in more complex cases. 

Let's now try with a BLAS call for the same job (DGEMM). To enable the BLAS call, uncomment lines 50 to 54. To compile with Intel MKL
::

   ifort -O3  matmul.F90 -o matmul.x -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl 

Run the code. You should see that even the most optimized version of the mat_mult is far slower than the BLAS one. That's why BLAS library exists. :)

Please notice that for small matrices (like 50x50) the BLAS are slower than our version. This is due to the overhead in the internal calls of BLAS, which 
becomes less and less relevant increasing the matrixces sizes. 



Exercise
~~~~~~~~~

- Try different permutation of the loop indeces, discuss the different performance you find in terms of memory access. Remember to compile 
  with ``-O0`` or ``-O1`` to prevent loop optmization by the compiler.

- Try to use ``gfortran`` instead of ``ifort``
  To this end, clean up the modules and load the gnu module 
  ::

    module purge
    module load gcc91

  Run a few tests and understand why the intel compiler license is so expensive :)

- (Optional) The folder also contains a C version of the code. Try to repeat the analysis of the 
  loop order to understand memory access, remeber that C stores data in row order
