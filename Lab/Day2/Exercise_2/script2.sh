#!/bin/bash

cd ~/PP/DSSC/Lab/Day2/Exercise_2

module load intel

icc -qopenmp loop_schedule.c
export OMP_NUM_THREADS=10 
./a.out

exit
