Benchmark HPC-TN cluster using HPL and HPCG 
=====================================


Playing with HPL
-----------------

Get the precompiled version here:

:: 

  ls /lustre/mhpc/eas/hpl/





things  to do:

1.run on 24 mpi processs the mkl-gnu version and finds out the right combination
in term of N Nb P ans Q that allows you to get at least 75% of peak
performance

2.prepare HPL.dat file to run the following combination:
 
  MPI-process =24 Thread 1 (case 1) please set OMP_NUM_THREADS=T
       "       12 T=2
       "        6 T=4
                4 T=6
                2 T=12
                1 T=24 
3. try the highly optimized version of HPL intel  you can find here below  and run just serially:

::
  /opt/cluster/intel/compilers_and_libraries/linux/mkl/benchmarks/linpack

4. Compare and comment performance you obtained with the two different compilers 



HPCG
--------------------

get it from this place: 

:: 

/lustre/eas/hpcg/bin 

get the data from the above dir:

:: 
/lustre/eas/hpcg


1. Run it for at least 300 second with a size enough to reach a decent performance (not less than 5 Gflops)
2. Compare with the performance you obtained on HPL 
3. Try the higlhy optimized Intel version of HPCG you can find here:

::
 /opt/cluster/intel/compilers_and_libraries/linux/mkl/benchmarks/hpcg/bin

 
