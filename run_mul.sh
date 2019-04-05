#!/bin/sh

export OMP_NUM_THREADS=$1
{ time ./simpar_omp $2 $3 $4 $5 1> thrash.txt; } 2> test_thread$1.txt