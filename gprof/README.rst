Profiling with ``gprof``
==========================

``gprof`` is a profiling tool that used a hybrid of instrumentation and sampling.
The instrumentation of the code is done using ``-pg`` with gcc and icc
::

  gcc -pg collatz.c -o collatz.x

Then run the code
::

  ./collatz.x

This will create a ``gmon.out file``. To analysize this file 
::

  gprof collatz.x gmon.out

The output is soemthing like
::

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ns/call  ns/call  name 
 61.26      0.98     0.98 740343580    1.32     1.32  step
 34.74      1.54     0.56  4999999   111.16   307.20  nseq
  5.05      1.62     0.08                             frame_dummy


It is saying that most of the time is spent in the function ``nseq``, (beside ``frame_dummy``, which we will discuss later), 
but that's only about ``cumulative`` (the time spent in a function and its children), while the time spent in the function itself (``self``) is modest. 
Instead, the time ``cumulative`` and ``self`` time in ``step`` are equal.
``frame_dummy`` has to do with program startup and function call overhead. The way to reduce it (if necessary) is to use inlining of function. However, using inlining (for example with ``-O2``)
completely removes profiling information, since function calls are eliminated and cannot be tracked. 

The call graph and its timing are also reported
::
 
  index % time    self  children    called     name
                  0.56    0.98 4999999/4999999     main [2]
  [1]     95.0    0.56    0.98 4999999         nseq [1]
                  0.98    0.00 740343580/740343580     step [3]
  -----------------------------------------------
                                                 <spontaneous>
  [2]     95.0    0.00    1.54                 main [2]
                  0.56    0.98 4999999/4999999     nseq [1]
  -----------------------------------------------
                  0.98    0.00 740343580/740343580     nseq [1]
  [3]     60.6    0.98    0.00 740343580         step [3]
  -----------------------------------------------
                                                 <spontaneous>
  [4]      5.0    0.08    0.00                 frame_dummy [4]
  -----------------------------------------------
   
Each groups of lines represent the children and (if available) the caller of each function, with the breakdown of ``cumulative`` and ``self`` time.


To visualize the call graph, install if needed gprof2dot and graphviz
::

  sudo pip install gprof2dot 
  sudo apt-get install graphviz


Then run 
::

  gprof collatz.x | gprof2dot | dot -Tpng -o output.png
