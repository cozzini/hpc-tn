Using auto-vectorization
===========================

In this tutorial we will use the auto-vectorization features of the compiler to speed up the exeution. 

Let's first compile the code without vectorization.
First log into a compute node via ``qsub``
::

  qsub -q jrc -l walltime=2:00:00 -l nodes=1:ppn=24 -I  

Load the intel/18.0 module 
::

  module load intel/18.0

Intel compiler uses vectorization features starting at ``-O2``. 
To prevent it, you could either use lower optimization level (``-O2``) or explicitly 
force no vectorization (``-no-vec``).
So 
::

  icc -O1 -o avx.x avx.c
  icc -O2 -no-vec  -o avx.x avx.c


Then time its execution
::

  time ./avx.x


We expect that the loop in the function ``multiplyValues`` should be a good candidate, since 
it shows no data dependences between indeces, data are packed and indeces are contiguous.

To turn on vectorization, simply compile with ``-O2`` or ``-O3`` . We can also have a report 
about vectorization by using ``-qopt-report-phase=vec``. This produces the file ``avx.optrpt``.
::

  icc -O2 -qopt-report-phase=vec -o avx.x avx.c


The relevant information is 
::

  LOOP BEGIN at avx.c(8,5)
   remark #15300: LOOP WAS VECTORIZED
  LOOP END

Check the execution time
::

  time ./avx.x

``gcc`` is also able to vectorize, although sometimes cannot vectorize loops that intel compiler is able to. Moreover, sometime the vectorization report is not working or it is inaccurate.
Anyway, to enable vectorization you can use flag ``-O3`` or explicitely ``-ftree-vectorizer``
Unload the current modules 
::

  module purge

and load gcc compiler
::

  module load gnu/7.2.0

Then compile 
::

  gcc -O3 -o avx.x avx.c
  gcc -O2 -ftree-vectorize -o avx.x avx.c

Compare the performance of the code with and without optimization (``-O2`` turn off vectorization, or ``-fno-tree-vectorize``). Compare the performance with the Intel results. 
 
