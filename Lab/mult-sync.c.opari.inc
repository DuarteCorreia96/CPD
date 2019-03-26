#include "pomp_lib.h"

struct ompregdescr omp_rd_1 = {
  "parallel", "", 0, "mult-sync.c", 26, 26, 36, 36
};

#define POMP_DLIST_00001 shared(omp_rd_1,omp_rd_2,omp_rd_3,omp_rd_4)

struct ompregdescr omp_rd_2 = {
  "barrier", "", 0, "mult-sync.c", 30, 30, 0, 0
};

struct ompregdescr omp_rd_3 = {
  "for", "", 0, "mult-sync.c", 31, 31, 33, 33
};

struct ompregdescr omp_rd_4 = {
  "single", "", 0, "mult-sync.c", 34, 34, 35, 35
};

