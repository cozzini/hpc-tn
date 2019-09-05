# simple MPI program: compute pi in parallel  

Montecarlo estimate of Pi. The idea is to have a circle inscribed inside a square of unit lenght. The
ratio between the area of the square (1) and the circle (pi/4) is
pi/4. Therefore, if we randomly choose N points inside the square,
on average, only M=N*pi/4 points will belong to the circle. From
the last relation we can estimate pi.


## Run Serial Code
```
$ gcc pi.c -o serial_pi
$ ./serial_pi 100000000
```
## Run Parallel Code

On the cluster, first load the module 
```bash
openmpi/3.1.4/gcc/8.2.0-pnmu67a
```

then

```bash
mpicc mpi_pi.c -o parallel_pi
mpirun -np 4 ./parallel_pi 100000000
```
Please not that in the parallel case, the number of trials is split among process. So

```bash 
mpirun -np 4 parallel_pi 1000
```
will do 250 trails per process.

  
# part 2:  analyze the parallel solution and improve it 


# Assignment - analyse scalability 

 1. Determine the CPU time required to calculate PI with the serial calculation using 10000000 iterations (stone throws). Make sure that this is the actual run time and does not include any
system time.

 Hint: use /usr/bin/time serial_pi to time all the application
 
 2. Get the MPI code running for the same number (10000000) of iterations.

 3. Run some running strong scaling test. 
 
    This means keeping the problem size constant, or in other words, keeping Niter = 10000000.
    Start by running the MPI code with only one processor doing the numerical computation. A
    comparison of this to the serial calculation gives you some idea of the overhead associated with 
    MPI.

    Make a plot of run time versus number of nodes from the data you have collected.


 4. Can you think to better way of gathering results from the other nodes?

 5. (optional) Run some week scaling test. This means we need to keep constant the computation load 
    per nodes. 
