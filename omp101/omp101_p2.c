/* very simple program taken from https://github.com/ysh329/OpenMP-101 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef N
#   define N (80)
#endif

int main(int argc, char *argv[])
{
    int nthreads, tid, idx;
    float a[N], b[N], c[N];
    nthreads = omp_get_num_threads();
    printf("Number of threads = %d\n", nthreads);
    #pragma omp parallel for
    for(idx=0; idx<N; ++idx)
    {
        a[idx] = b[idx] = 1.0;
    }
    #pragma omp parallel for
    for(idx=0; idx<N; ++idx)
    {
        c[idx] = a[idx] + b[idx];
        tid = omp_get_thread_num();
        printf("Thread %d: c[%d]=%f\n", tid, idx, c[idx]);
    }
}
