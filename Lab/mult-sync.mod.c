#ifdef _POMP
#  undef _POMP
#endif
#define _POMP 200110

#include "mult-sync.c.opari.inc"
#line 1 "mult-sync.c"
#include <stdlib.h>

#define N 1000

int *a;
int *b;
int *c;

int main()
{
  int i, j, k;
  int acc;

  a = (int *)malloc(N*N*sizeof(int));
  b = (int *)malloc(N*N*sizeof(int));
  c = (int *)malloc(N*N*sizeof(int));

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++){
      a[i * N + j] = rand();
      b[i * N + j] = rand();
    }
      

POMP_Parallel_fork(&omp_rd_1);
#line 26 "mult-sync.c"
#pragma omp parallel private(i,j,k) POMP_DLIST_00001
{ POMP_Parallel_begin(&omp_rd_1);
#line 27 "mult-sync.c"
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++){
      acc = 0;
POMP_Barrier_enter(&omp_rd_2);
#line 30 "mult-sync.c"
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_2);
#line 31 "mult-sync.c"
POMP_For_enter(&omp_rd_3);
#line 31 "mult-sync.c"
#pragma omp for reduction(+:acc) nowait
      for(k = 0; k < N; k++)
	acc += a[i * N + k] * b[k * N + j];
POMP_Barrier_enter(&omp_rd_3);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_3);
POMP_For_exit(&omp_rd_3);
#line 34 "mult-sync.c"
POMP_Single_enter(&omp_rd_4);
#line 34 "mult-sync.c"
#pragma omp single nowait
{ POMP_Single_begin(&omp_rd_4);
#line 35 "mult-sync.c"
      c[i * N + j] = acc;
POMP_Single_end(&omp_rd_4); }
POMP_Barrier_enter(&omp_rd_4);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_4);
POMP_Single_exit(&omp_rd_4);
#line 36 "mult-sync.c"
    }
POMP_Barrier_enter(&omp_rd_1);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_1);
POMP_Parallel_end(&omp_rd_1); }
POMP_Parallel_join(&omp_rd_1);
#line 37 "mult-sync.c"
}
