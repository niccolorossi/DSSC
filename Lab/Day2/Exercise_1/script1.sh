#!/bin/bash

cd ~/PP/DSSC/Lab/Day2

module load intel

icc -qopenmp parallelpi.c

for threads in 1 2 4 8 16 20; do
 export OMP_NUM_THREADS=${threads}
  ./a.out >> scalabilityr.txt
   done
   
exit
