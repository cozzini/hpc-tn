Using Likwid tools on UNITN cluster
====================================

The goal of this tutorial is to show how to use likwid tools to assess performance on unitn computational nodes of some computational kernels. We will work on one single computational node.

We first submit an interactive job to the queue system

::

  qsub -l walltime=1:00:00,nodes=1:ppn=72 -I -q SHPC2019_cpuQ
  
Load the needed module 

::

  module  load likwid-4.3.4

As first action let us identify the topology of our node

::

  $ likwid-topology -g
  --------------------------------------------------------------------------------
  CPU name:	Intel(R) Xeon(R) Gold 6140M CPU @ 2.30GHz
  CPU type:	Intel Skylake SP processor
  CPU stepping:	4
  ********************************************************************************
  Hardware Thread Topology
  ********************************************************************************
  Sockets:		4
  Cores per socket:	18
  Threads per core:	1 
  --------------------------------------------------------------------------------
  HWThread	Thread		Core		Socket		Available
  0		0		0		0		 
  rest of output omitted


Likwid-perfctr  Measures applications' interaction with the hardware using the hardware performance counters
The Linux MSR module, providing an interface to access model specific registers from user space, allows us to read out hardware performance counters with an unmodified Linux kernel. Moreover, recent Intel systems provide Uncore hardware counter through PCI interfaces.

For gathering information about hardware performance capabilities and performance groups we can use the -a, -g and -H switches.
likwid-perfctr's performance groups are simple text files and can be easily changed or extended. It is simple to create your own performance groups with custom derived metrics.

To check all supported/predefined groups on a processor to stdout:

::

 $ likwid-perfctr -a
 [exact@b21 ~]$ likwid-perfctr -a
    BRANCH	Branch prediction miss rate/ratio
    CACHES	Cache bandwidth in MBytes/s
      CBOX	CBOX related data and metrics
     CLOCK	Power and Energy consumption
      DATA	Load to store ratio
    ENERGY	Power and Energy consumption
 FALSE_SHARE	False sharing
  FLOPS_AVX	Packed AVX MFlops/s
  FLOPS_DP	Double Precision MFlops/s
  FLOPS_SP	Single Precision MFlops/s
    ICACHE	Instruction cache miss rate/ratio
   L2CACHE	L2 cache miss rate/ratio
        L2	L2 cache bandwidth in MBytes/s
   L3CACHE	L3 cache miss rate/ratio
        L3	L3 cache bandwidth in MBytes/s
    MEM_DP	Power and Energy consumption
    MEM_SP	Power and Energy consumption
       MEM	Main memory bandwidth in MBytes/s
      NUMA	Local and remote memory accesses
       QPI	QPI Link Layer data
  RECOVERY	Recovery duration
  TLB_DATA	L1 Data TLB miss rate/ratio
 TLB_INSTR	L1 Instruction TLB miss rate/ratio
 UNCORECLOCK	All Clocks
 UOPS_EXEC	UOPs execution
 UOPS_ISSUE	UOPs issueing
 UOPS_RETIRE	UOPs retirement
 UOPS	UOPs execution info
 [exact@b21 ~]$

To get a list with all supported counter registers and events, call:

::

 $ likwid-perfctr -e | less


To get a list with all supported events and corresponding counter registers that match a string (case insensitive), call:

::

  $ likwid-perfctr -E <string>

A help text explaining a specific event group can be requested with -H together with the -g switch:

::

  $ likwid-perfctr -H -g MEM

This prints the text below LONG in the performance group file. 

To use likwid-perfctr for a serial application execute:

::

  $ likwid-perfctr  -C S0:1  -g BRANCH  ./a.out


We can now use it on a shared openMP application: as an example we use stream application compiled
with intel compiler in the following way: 

::

 icc -Ofast -xHost -qopenmp -fno-alias -nolib-inline  -o stream.exe stream.c

And here is the result we obtained, specifying as performance group flops in double precision:


::

 [exact@b22 stream]$  env OMP_NUM_THREADS=4 likwid-perfctr -C N:0-3  -g FLOPS_DP ./stream.exe
 --------------------------------------------------------------------------------
 CPU name:       Intel(R) Xeon(R) CPU E5-2697 v2 @ 2.70GHz
 CPU type:       Intel Xeon IvyBridge EN/EP/EX processor
 CPU clock:      2.70 GHz
 --------------------------------------------------------------------------------
 -------------------------------------------------------------
 This system uses 8 bytes per DOUBLE PRECISION word.
 -------------------------------------------------------------
 Array size = 40000000, Offset = 0
 Total memory required = 915.5 MB.
 Each test is run 50 times, but only
 the *best* time for each is used.
 -------------------------------------------------------------
 Number of Threads requested = 4
 Number of Threads requested = 4
 Number of Threads requested = 4
 Number of Threads requested = 4
 -------------------------------------------------------------
 Your clock granularity/precision appears to be 1 microseconds.
 Each test below will take on the order of 21778 microseconds.
   (= 21778 clock ticks)
 Increase the size of the arrays if this shows that
 you are not getting at least 20 clock ticks per test.
 -------------------------------------------------------------
 WARNING -- The above is only a rough guideline.
 For best results, please be sure you know the
 precision of your system timer.
 -------------------------------------------------------------
 Function      Rate (MB/s)   Avg time     Min time     Max time
 Copy:       23543.2525       0.0278       0.0272       0.0284
 Scale:      24049.2619       0.0273       0.0266       0.0279
 Add:        30886.1279       0.0319       0.0311       0.0328
 Triad:      30994.7798       0.0318       0.0310       0.0324
 -------------------------------------------------------------
 Solution Validates
 -------------------------------------------------------------
 --------------------------------------------------------------------------------
 Group 1: FLOPS_DP
 +--------------------------------------+---------+-------------+-------------+-------------+-------------+
 |                 Event                | Counter |    Core 0   |    Core 1   |    Core 2   |    Core 3   |
 +--------------------------------------+---------+-------------+-------------+-------------+-------------+
 |           INSTR_RETIRED_ANY          |  FIXC0  |  3696265141 |  3667106550 |  3704347130 |  3690214339 |
 |         CPU_CLK_UNHALTED_CORE        |  FIXC1  | 19651949763 | 19198600845 | 19194659603 | 19197559635 |
 |         CPU_CLK_UNHALTED_REF         |  FIXC2  | 17225055081 | 16688914110 | 16685617248 | 16688523501 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE |   PMC0  |          15 |           0 |           0 |           0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE |   PMC1  |        4471 |          57 |           6 |          26 |
 |       SIMD_FP_256_PACKED_DOUBLE      |   PMC2  |  2224300967 |  2194236515 |  2200481389 |  2201948141 |
 +--------------------------------------+---------+-------------+-------------+-------------+-------------+

 +-------------------------------------------+---------+-------------+-------------+-------------+--------------+
 |                   Event                   | Counter |     Sum     |     Min     |     Max     |      Avg     |
 +-------------------------------------------+---------+-------------+-------------+-------------+--------------+
 |           INSTR_RETIRED_ANY STAT          |  FIXC0  | 14757933160 |  3667106550 |  3704347130 |   3689483290 |
 |         CPU_CLK_UNHALTED_CORE STAT        |  FIXC1  | 77242769846 | 19194659603 | 19651949763 | 1.931069e+10 |
 |         CPU_CLK_UNHALTED_REF STAT         |  FIXC2  | 67288109940 | 16685617248 | 17225055081 |  16822027485 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE STAT |   PMC0  |          15 |           0 |          15 |       3.7500 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE STAT |   PMC1  |        4560 |           6 |        4471 |         1140 |
 |       SIMD_FP_256_PACKED_DOUBLE STAT      |   PMC2  |  8820967012 |  2194236515 |  2224300967 |   2205241753 |
 +-------------------------------------------+---------+-------------+-------------+-------------+--------------+

 +----------------------+-----------+--------------+--------------+--------------+
 |        Metric        |   Core 0  |    Core 1    |    Core 2    |    Core 3    |
 +----------------------+-----------+--------------+--------------+--------------+
 |  Runtime (RDTSC) [s] |    7.1182 |       7.1182 |       7.1182 |       7.1182 |
 | Runtime unhalted [s] |    7.2785 |       7.1106 |       7.1091 |       7.1102 |
 |      Clock [MHz]     | 3080.4173 |    3106.0328 |    3106.0087 |    3105.9370 |
 |          CPI         |    5.3167 |       5.2354 |       5.1817 |       5.2023 |
 |      DP MFLOP/s      | 1249.9214 |    1233.0264 |    1236.5356 |    1237.3598 |
 |    AVX DP MFLOP/s    | 1249.9208 |    1233.0264 |    1236.5356 |    1237.3598 |
 |    Packed MUOPS/s    |  312.4802 |     308.2566 |     309.1339 |     309.3400 |
 |    Scalar MUOPS/s    |    0.0006 | 8.007626e-06 | 8.429080e-07 | 3.652601e-06 |
 |  Vectorization ratio |   99.9998 |     100.0000 |     100.0000 |     100.0000 |
 +----------------------+-----------+--------------+--------------+--------------+

 +---------------------------+------------+--------------+-----------+-----------+
 |           Metric          |     Sum    |      Min     |    Max    |    Avg    |
 +---------------------------+------------+--------------+-----------+-----------+
 |  Runtime (RDTSC) [s] STAT |    28.4728 |       7.1182 |    7.1182 |    7.1182 |
 | Runtime unhalted [s] STAT |    28.6084 |       7.1091 |    7.2785 |    7.1521 |
 |      Clock [MHz] STAT     | 12398.3958 |    3080.4173 | 3106.0328 | 3099.5990 |
 |          CPI STAT         |    20.9361 |       5.1817 |    5.3167 |    5.2340 |
 |      DP MFLOP/s STAT      |  4956.8432 |    1233.0264 | 1249.9214 | 1239.2108 |
 |    AVX DP MFLOP/s STAT    |  4956.8426 |    1233.0264 | 1249.9208 | 1239.2106 |
 |    Packed MUOPS/s STAT    |  1239.2107 |     308.2566 |  312.4802 |  309.8027 |
 |    Scalar MUOPS/s STAT    |     0.0006 | 8.429080e-07 |    0.0006 |    0.0002 |
 |  Vectorization ratio STAT |   399.9998 |      99.9998 |       100 |   99.9999 |
 +---------------------------+------------+--------------+-----------+-----------+


Results give us some interesting things to look at:



We can now use the API...
Let us use again a modified stream version, instrumented appropriately.
 Here the way we can compile the application:

::

  icc -Ofast -xHost -qopenmp -fno-alias -nolib-inline -I/apps/likwid-4.3.4/include/ -DLIKWID_PERFMON stream-mapi.c -o stream-mapi.exe -L/apps/likwid-4.3.4/lib/ -llikwid
  
And we can now run it in the following way: 

::

 [exact@b21 likwid]$ export OMP_NUM_THREADS=4 ; likwid-perfctr -C N:0-3 -m -g FLOPS_DP -f ./stream-mapi.exe
 --------------------------------------------------------------------------------
 CPU name:	Intel(R) Xeon(R) CPU E5-2697 v2 @ 2.70GHz
 CPU type:	Intel Xeon IvyBridge EN/EP/EX processor
 CPU clock:	2.70 GHz
 --------------------------------------------------------------------------------
 stream output omitted.. 
 -------------------------------------------------------------
 --------------------------------------------------------------------------------
 Region COPY, Group 1: FLOPS_DP
 +-------------------+----------+----------+----------+----------+
 |    Region Info    |  Core 0  |  Core 1  |  Core 2  |  Core 3  |
 +-------------------+----------+----------+----------+----------+
 | RDTSC Runtime [s] | 1.425333 | 1.392811 | 1.393284 | 1.393884 |
 |     call count    |       50 |       50 |       50 |       50 |
 +-------------------+----------+----------+----------+----------+

 +--------------------------------------+---------+------------+------------+------------+------------+
 |                 Event                | Counter |   Core 0   |   Core 1   |   Core 2   |   Core 3   |
 +--------------------------------------+---------+------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY          |  FIXC0  |  852601100 |  790263600 |  755046300 |  764659800 |
 |         CPU_CLK_UNHALTED_CORE        |  FIXC1  | 4440771000 | 4373230000 | 4377588000 | 4378341000 |
 |         CPU_CLK_UNHALTED_REF         |  FIXC2  | 3798388000 | 3728948000 | 3732916000 | 3733775000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE |   PMC0  |          0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE |   PMC1  |        649 |        701 |        580 |        609 |
 |       SIMD_FP_256_PACKED_DOUBLE      |   PMC2  |          0 |          0 |          0 |          0 |
 +--------------------------------------+---------+------------+------------+------------+------------+

 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |                   Event                   | Counter |     Sum     |     Min    |     Max    |     Avg    |
 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY STAT          |  FIXC0  |  3162570800 |  755046300 |  852601100 |  790642700 |
 |         CPU_CLK_UNHALTED_CORE STAT        |  FIXC1  | 17569930000 | 4373230000 | 4440771000 | 4392482500 |
 |         CPU_CLK_UNHALTED_REF STAT         |  FIXC2  | 14994027000 | 3728948000 | 3798388000 | 3748506750 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE STAT |   PMC0  |           0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE STAT |   PMC1  |        2539 |        580 |        701 |   634.7500 |
 |       SIMD_FP_256_PACKED_DOUBLE STAT      |   PMC2  |           0 |          0 |          0 |          0 |
 +-------------------------------------------+---------+-------------+------------+------------+------------+

 +----------------------+-----------+-----------+-----------+-----------+ 
 |        Metric        |   Core 0  |   Core 1  |   Core 2  |   Core 3  |
 +----------------------+-----------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] |    1.4253 |    1.3928 |    1.3933 |    1.3939 |
 | Runtime unhalted [s] |    1.6447 |    1.6197 |    1.6213 |    1.6216 | 
 |       Clock [MHz]     | 3156.6011 | 3166.4792 | 3166.2654 | 3166.0815 |
 |          CPI         |    5.2085 |    5.5339 |    5.7978 |    5.7259 |
 |      DP MFLOP/s      |    0.0005 |    0.0005 |    0.0004 |    0.0004 |
 |    AVX DP MFLOP/s    |         0 |         0 |         0 |         0 |
 |    Packed MUOPS/s    |         0 |         0 |         0 |         0 | 
 |    Scalar MUOPS/s    |    0.0005 |    0.0005 |    0.0004 |    0.0004 |
 |  Vectorization ratio |         0 |         0 |         0 |         0 |
 +----------------------+-----------+-----------+-----------+-----------+

 +---------------------------+------------+-----------+-----------+-----------+
 |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
 +---------------------------+------------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] STAT |     5.6053 |    1.3928 |    1.4253 |    1.4013 |
 | Runtime unhalted [s] STAT |     6.5073 |    1.6197 |    1.6447 |    1.6268 |
 |      Clock [MHz] STAT     | 12655.4272 | 3156.6011 | 3166.4792 | 3163.8568 |
 |          CPI STAT         |    22.2661 |    5.2085 |    5.7978 |    5.5665 |
 |      DP MFLOP/s STAT      |     0.0018 |    0.0004 |    0.0005 |    0.0004 |
 |    AVX DP MFLOP/s STAT    |          0 |         0 |         0 |         0 |
 |    Packed MUOPS/s STAT    |          0 |         0 |         0 |         0 |
 |     Scalar MUOPS/s STAT    |     0.0018 |    0.0004 |    0.0005 |    0.0004 |
 |  Vectorization ratio STAT |          0 |         0 |         0 |         0 |
 +---------------------------+------------+-----------+-----------+-----------+

 Region SCALE, Group 1: FLOPS_DP
 +-------------------+----------+----------+----------+----------+
 |    Region Info    |  Core 0  |  Core 1  |  Core 2  |  Core 3  |
 +-------------------+----------+----------+----------+----------+
 | RDTSC Runtime [s] | 1.366011 | 1.366193 | 1.366057 | 1.366119 |
 |     call count    |       50 |       50 |       50 |       50 |
 +-------------------+----------+----------+----------+----------+ 

 +--------------------------------------+---------+------------+------------+------------+------------+
 |                 Event                | Counter |   Core 0   |   Core 1   |   Core 2   |   Core 3   |
 +--------------------------------------+---------+------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY          |  FIXC0  |  758494900 |  766733600 |  766753700 |  758939000 |
 |         CPU_CLK_UNHALTED_CORE        |  FIXC1  | 4279775000 | 4293971000 | 4294353000 | 4295087000 |
 |         CPU_CLK_UNHALTED_REF         |  FIXC2  | 3647409000 | 3659665000 | 3659967000 | 3660603000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE |   PMC0  |          0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE |   PMC1  |        630 |        643 |        581 |        632 |
 |       SIMD_FP_256_PACKED_DOUBLE      |   PMC2  |  684675300 |  686030200 |  689192100 |  688938100 |
 +--------------------------------------+---------+------------+------------+------------+------------+

 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |                   Event                   | Counter |     Sum     |     Min    |     Max    |     Avg    |
 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY STAT          |  FIXC0  |  3050921200 |  758494900 |  766753700 |  762730300 |
 |         CPU_CLK_UNHALTED_CORE STAT        |  FIXC1  | 17163186000 | 4279775000 | 4295087000 | 4290796500 |
 |         CPU_CLK_UNHALTED_REF STAT         |  FIXC2  | 14627644000 | 3647409000 | 3660603000 | 3656911000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE STAT |   PMC0  |           0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE STAT |   PMC1  |        2486 |        581 |        643 |   621.5000 |
 |       SIMD_FP_256_PACKED_DOUBLE STAT      |   PMC2  |  2748835700 |  684675300 |  689192100 |  687208925 |
 +-------------------------------------------+---------+-------------+------------+------------+------------+

 +----------------------+-----------+-----------+-----------+-----------+
 |        Metric        |   Core 0  |   Core 1  |   Core 2  |   Core 3  |
 +----------------------+-----------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] |    1.3660 |    1.3662 |    1.3661 |    1.3661 |
 | Runtime unhalted [s] |    1.5851 |    1.5904 |    1.5905 |    1.5908 |
 |      Clock [MHz]     | 3168.0871 | 3167.9508 | 3167.9712 | 3167.9621 |
 |          CPI         |    5.6425 |    5.6003 |    5.6007 |    5.6593 |
 |      DP MFLOP/s      | 2004.8900 | 2008.5899 | 2018.0483 | 2017.2130 |
 |    AVX DP MFLOP/s    | 2004.8896 | 2008.5894 | 2018.0479 | 2017.2126 |
 |    Packed MUOPS/s    |  501.2224 |  502.1474 |  504.5120 |  504.3031 |
 |    Scalar MUOPS/s    |    0.0005 |    0.0005 |    0.0004 |    0.0005 |
 |  Vectorization ratio |   99.9999 |   99.9999 |   99.9999 |   99.9999 |
 +----------------------+-----------+-----------+-----------+-----------+

 +---------------------------+------------+-----------+-----------+-----------+
 |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
 +---------------------------+------------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] STAT |     5.4644 |    1.3660 |    1.3662 |    1.3661 |
 | Runtime unhalted [s] STAT |     6.3568 |    1.5851 |    1.5908 |    1.5892 |
 |      Clock [MHz] STAT     | 12671.9712 | 3167.9508 | 3168.0871 | 3167.9928 |
 |          CPI STAT         |    22.5028 |    5.6003 |    5.6593 |    5.6257 |
 |      DP MFLOP/s STAT      |  8048.7412 | 2004.8900 | 2018.0483 | 2012.1853 |
 |    AVX DP MFLOP/s STAT    |  8048.7395 | 2004.8896 | 2018.0479 | 2012.1849 |
 |    Packed MUOPS/s STAT    |  2012.1849 |  501.2224 |  504.5120 |  503.0462 |
 |    Scalar MUOPS/s STAT    |     0.0019 |    0.0004 |    0.0005 |    0.0005 |
 |  Vectorization ratio STAT |   399.9996 |   99.9999 |   99.9999 |   99.9999 |
 +---------------------------+------------+-----------+-----------+-----------+

 Region ADD, Group 1: FLOPS_DP
 +-------------------+----------+----------+----------+----------+
 |    Region Info    |  Core 0  |  Core 1  |  Core 2  |  Core 3  |
 +-------------------+----------+----------+----------+----------+
 | RDTSC Runtime [s] | 1.602534 | 1.602778 | 1.602624 | 1.602728 |
 |     call count    |       50 |       50 |       50 |       50 |
 +-------------------+----------+----------+----------+----------+

 +--------------------------------------+---------+------------+------------+------------+------------+
 |                 Event                | Counter |   Core 0   |   Core 1   |   Core 2   |   Core 3   |
 +--------------------------------------+---------+------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY          |  FIXC0  |  771827300 |  788597400 |  765064900 |  757560600 |
 |         CPU_CLK_UNHALTED_CORE        |  FIXC1  | 5012202000 | 5029485000 | 5030629000 | 5030486000 |
 |         CPU_CLK_UNHALTED_REF         |  FIXC2  | 4272469000 | 4287383000 | 4288329000 | 4288219000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE |   PMC0  |          0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE |   PMC1  |        607 |        664 |        569 |        605 |
 |       SIMD_FP_256_PACKED_DOUBLE      |   PMC2  |  459514400 |  459021200 |  462677800 |  462472100 |
 +--------------------------------------+---------+------------+------------+------------+------------+

 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |                   Event                   | Counter |     Sum     |     Min    |     Max    |     Avg    |
 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY STAT          |  FIXC0  |  3083050200 |  757560600 |  788597400 |  770762550 |
 |         CPU_CLK_UNHALTED_CORE STAT        |  FIXC1  | 20102802000 | 5012202000 | 5030629000 | 5025700500 |
 |         CPU_CLK_UNHALTED_REF STAT         |  FIXC2  | 17136400000 | 4272469000 | 4288329000 | 4284100000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE STAT |   PMC0  |           0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE STAT |   PMC1  |        2445 |        569 |        664 |   611.2500 |
 |       SIMD_FP_256_PACKED_DOUBLE STAT      |   PMC2  |  1843685500 |  459021200 |  462677800 |  460921375 |
 +-------------------------------------------+---------+-------------+------------+------------+------------+

 +----------------------+-----------+-----------+-----------+-----------+
 |        Metric        |   Core 0  |   Core 1  |   Core 2  |   Core 3  |
 +----------------------+-----------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] |    1.6025 |    1.6028 |    1.6026 |    1.6027 |
 | Runtime unhalted [s] |    1.8564 |    1.8628 |    1.8632 |    1.8632 |
 |      Clock [MHz]     | 3167.4538 | 3167.3196 | 3167.3411 | 3167.3323 |
 |          CPI         |    6.4939 |    6.3778 |    6.5754 |    6.6404 |
 |      DP MFLOP/s      | 1146.9699 | 1145.5644 | 1154.8010 | 1154.2127 |
 |    AVX DP MFLOP/s    | 1146.9695 | 1145.5640 | 1154.8006 | 1154.2123 |
 |    Packed MUOPS/s    |  286.7424 |  286.3910 |  288.7002 |  288.5531 |
 |    Scalar MUOPS/s    |    0.0004 |    0.0004 |    0.0004 |    0.0004 |
 |  Vectorization ratio |   99.9999 |   99.9999 |   99.9999 |   99.9999 |
 +----------------------+-----------+-----------+-----------+-----------+

 +---------------------------+------------+-----------+-----------+-----------+
 |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
 +---------------------------+------------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] STAT |     6.4106 |    1.6025 |    1.6028 |    1.6027 |
 | Runtime unhalted [s] STAT |     7.4456 |    1.8564 |    1.8632 |    1.8614 |
 |      Clock [MHz] STAT     | 12669.4468 | 3167.3196 | 3167.4538 | 3167.3617 |
 |          CPI STAT         |    26.0875 |    6.3778 |    6.6404 |    6.5219 |
 |      DP MFLOP/s STAT      |  4601.5480 | 1145.5644 | 1154.8010 | 1150.3870 |
 |    AVX DP MFLOP/s STAT    |  4601.5464 | 1145.5640 | 1154.8006 | 1150.3866 |
 |    Packed MUOPS/s STAT    |  1150.3867 |  286.3910 |  288.7002 |  287.5967 |
 |    Scalar MUOPS/s STAT    |     0.0016 |    0.0004 |    0.0004 |    0.0004 |
 |  Vectorization ratio STAT |   399.9996 |   99.9999 |   99.9999 |   99.9999 |
 +---------------------------+------------+-----------+-----------+-----------+

 Region TRIAD, Group 1: FLOPS_DP
 +-------------------+----------+----------+----------+----------+
 |    Region Info    |  Core 0  |  Core 1  |  Core 2  |  Core 3  |
 +-------------------+----------+----------+----------+----------+
 | RDTSC Runtime [s] | 1.584449 | 1.584575 | 1.584468 | 1.584563 |
 |     call count    |       50 |       50 |       50 |       50 |
 +-------------------+----------+----------+----------+----------+ 

 +--------------------------------------+---------+------------+------------+------------+------------+
 |                 Event                | Counter |   Core 0   |   Core 1   |   Core 2   |   Core 3   |
 +--------------------------------------+---------+------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY          |  FIXC0  |  759935500 |  768181100 |  770878700 |  765424800 |
 |         CPU_CLK_UNHALTED_CORE        |  FIXC1  | 4957289000 | 4975577000 | 4975616000 | 4975651000 |
 |         CPU_CLK_UNHALTED_REF         |  FIXC2  | 4222282000 | 4238075000 | 4238092000 | 4238139000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE |   PMC0  |          0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE |   PMC1  |        631 |        640 |        590 |        615 |
 |       SIMD_FP_256_PACKED_DOUBLE      |   PMC2  | 1035312000 | 1036618000 | 1039763000 | 1041733000 |
 +--------------------------------------+---------+------------+------------+------------+------------+

 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |                   Event                   | Counter |     Sum     |     Min    |     Max    |     Avg    |
 +-------------------------------------------+---------+-------------+------------+------------+------------+
 |           INSTR_RETIRED_ANY STAT          |  FIXC0  |  3064420100 |  759935500 |  770878700 |  766105025 |
 |         CPU_CLK_UNHALTED_CORE STAT        |  FIXC1  | 19884133000 | 4957289000 | 4975651000 | 4971033250 |
 |         CPU_CLK_UNHALTED_REF STAT         |  FIXC2  | 16936588000 | 4222282000 | 4238139000 | 4234147000 |
 | FP_COMP_OPS_EXE_SSE_FP_PACKED_DOUBLE STAT |   PMC0  |           0 |          0 |          0 |          0 |
 | FP_COMP_OPS_EXE_SSE_FP_SCALAR_DOUBLE STAT |   PMC1  |        2476 |        590 |        640 |        619 |
 |       SIMD_FP_256_PACKED_DOUBLE STAT      |   PMC2  |  4153426000 | 1035312000 | 1041733000 | 1038356500 |
 +-------------------------------------------+---------+-------------+------------+------------+------------+

 +----------------------+-----------+-----------+-----------+-----------+
 |        Metric        |   Core 0  |   Core 1  |   Core 2  |   Core 3  |
 +----------------------+-----------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] |    1.5844 |    1.5846 |    1.5845 |    1.5846 |
 | Runtime unhalted [s] |    1.8360 |    1.8428 |    1.8428 |    1.8428 |
 |      Clock [MHz]     | 3169.9882 | 3169.8263 | 3169.8384 | 3169.8256 |
 |          CPI         |    6.5233 |    6.4771 |    6.4545 |    6.5005 |
 |      DP MFLOP/s      | 2613.6838 | 2616.7727 | 2624.8890 | 2629.7046 |
 |    AVX DP MFLOP/s    | 2613.6834 | 2616.7723 | 2624.8886 | 2629.7042 |
 |    Packed MUOPS/s    |  653.4208 |  654.1931 |  656.2222 |  657.4261 |
 |    Scalar MUOPS/s    |    0.0004 |    0.0004 |    0.0004 |    0.0004 |
 |  Vectorization ratio |   99.9999 |   99.9999 |   99.9999 |   99.9999 |
 +----------------------+-----------+-----------+-----------+-----------+

 +---------------------------+------------+-----------+-----------+-----------+
 |           Metric          |     Sum    |    Min    |    Max    |    Avg    |
 +---------------------------+------------+-----------+-----------+-----------+
 |  Runtime (RDTSC) [s] STAT |     6.3381 |    1.5844 |    1.5846 |    1.5845 |
 | Runtime unhalted [s] STAT |     7.3644 |    1.8360 |    1.8428 |    1.8411 |
 |      Clock [MHz] STAT     | 12679.4785 | 3169.8256 | 3169.9882 | 3169.8696 |
 |          CPI STAT         |    25.9554 |    6.4545 |    6.5233 |    6.4888 |
 |      DP MFLOP/s STAT      | 10485.0501 | 2613.6838 | 2629.7046 | 2621.2625 |
 |    AVX DP MFLOP/s STAT    | 10485.0485 | 2613.6834 | 2629.7042 | 2621.2621 |
 |    Packed MUOPS/s STAT    |  2621.2622 |  653.4208 |  657.4261 |  655.3156 |
 |    Scalar MUOPS/s STAT    |     0.0016 |    0.0004 |    0.0004 |    0.0004 |
 |  Vectorization ratio STAT |   399.9996 |   99.9999 |   99.9999 |   99.9999 |
 +---------------------------+------------+-----------+-----------+-----------+


We have now the four distinct region we instrumented and for each of them we can check the specific performance results.

Exercises:
----------

Run likwid  on avx codes and matmul code using different metric and compare results with Stream ones.  

